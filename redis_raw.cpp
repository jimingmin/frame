/*
 * redis_raw.cpp
 *
 *  Created on: Feb 10, 2015
 *      Author: jimm
 */

#include <stdio.h>
#include <string.h>
#include "redis_raw.h"
#include "redis_glue.h"
#include "redis_session.h"
#include "redissession_bank.h"
#include "sessionindex_bank.h"
#include "frame.h"
#include "../logger/logger.h"
#include "../common/common_memmgt.h"

using namespace LOGGER;

FRAME_NAMESPACE_BEGIN

CRedisAgent CRedisRaw::m_stRedisAgent;

CRedisRaw::CRedisRaw(int32_t nServerID, char *pAddress, uint16_t nPort)
{
	m_pRedisContext = NULL;

	m_nServerID = nServerID;
	strcpy(m_arrAddress, pAddress);
	m_nPort = nPort;
	m_bConnectSuccess = false;
}

CRedisRaw::~CRedisRaw()
{

}

int32_t CRedisRaw::Init()
{
	return 0;
}

int32_t CRedisRaw::Uninit()
{
	return 0;
}

int32_t CRedisRaw::GetSize()
{
	return sizeof(*this);
}

int32_t CRedisRaw::OnConnected()
{
	return 0;
}

void CRedisRaw::OnClosed()
{

}

int32_t CRedisRaw::Connect()
{
	m_pRedisContext = redisAsyncConnect(m_arrAddress, m_nPort);
	if(m_pRedisContext->err)
	{
		WRITE_WARN_LOG(MODULE_NAME, "connect redis error!{error=%s, server_id=%d, address=%s, port=%d}\n",
				m_pRedisContext->errstr, m_nServerID, m_arrAddress, m_nPort);

		return 1;
	}

	m_stRedisAgent.AttachAsyncEvent(m_pRedisContext);

	CRedisGlue::RegRedisContext(m_pRedisContext, this);

	WRITE_INFO_LOG(MODULE_NAME, "connect redis!{server_id=%d, address=%s, port=%d}\n", m_nServerID, m_arrAddress, m_nPort);

	return 0;
}

void CRedisRaw::Close()
{
	redisAsyncDisconnect(m_pRedisContext);
}

bool CRedisRaw::IsConnected()
{
	return m_bConnectSuccess;
}

void CRedisRaw::Connected(const redisAsyncContext *pContext, int32_t nStatus)
{
    if (nStatus != REDIS_OK) {
		WRITE_WARN_LOG(MODULE_NAME, "connect redis failed!{error=%s, server_id=%d, address=%s, port=%d}\n",
				pContext->errstr, m_nServerID, m_arrAddress, m_nPort);
        return;
    }

    WRITE_INFO_LOG(MODULE_NAME, "connect redis success!{server_id=%d, address=%s, port=%d}\n", m_nServerID, m_arrAddress, m_nPort);

    m_bConnectSuccess = true;

    OnConnected();
}

void CRedisRaw::Closed(const redisAsyncContext *pContext, int32_t nStatus)
{
	OnClosed();

	m_bConnectSuccess = false;

    if (nStatus != REDIS_OK) {
		WRITE_WARN_LOG(MODULE_NAME, "close redis occur error!{error=%s, server_id=%d, address=%s, port=%d}\n",
				pContext->errstr, m_nServerID, m_arrAddress, m_nPort);

        CRedisGlue::UnregRedisContext(m_pRedisContext);

        m_pRedisContext = NULL;
        return;
    }

    WRITE_INFO_LOG(MODULE_NAME, "redis connection is closed!{server_id=%d, address=%s, port=%d}\n", m_nServerID, m_arrAddress, m_nPort);

    CRedisGlue::UnregRedisContext(m_pRedisContext);

    m_pRedisContext = NULL;
}

void CRedisRaw::OnRedisReply(const redisAsyncContext *pContext, void *pReply, void *pSessionIndex)
{
	if(pSessionIndex == NULL)
	{
		return;
	}

    if (pReply == NULL)
    {
    	return;
    }

    CSessionIndexBank *pSessionIndexBank = (CSessionIndexBank *)g_Frame.GetBank(BANK_SESSION_INDEX);
    RedisSession *pRedisSession = ((CRedisSessionBank *)g_Frame.GetBank(BANK_REDIS_SESSION))->GetSession(*((uint32_t *)pSessionIndex));
    if(pRedisSession == NULL)
    {
    	pSessionIndexBank->DestroySessionIndex((uint32_t *)pSessionIndex);
    	return;
    }
    CBaseObject *pHandler = pRedisSession->GetHandlerObj();
    RedisReply Proc = pRedisSession->GetHandleRedisReply();

    (pHandler->*Proc)(pContext->err, pReply, pRedisSession);

    pSessionIndexBank->DestroySessionIndex((uint32_t *)pSessionIndex);
}

void CRedisRaw::OnUnsubscribeReply(const redisAsyncContext *pContext, void *pReply, void *pSessionIndex)
{

}

int32_t CRedisRaw::FormatCommand(char szCommand[], int32_t &nCmdSize, const char *szFormat, ...)
{
    char *pCmd;
    va_list ap;
    va_start(ap, szFormat);
    int32_t nLen = redisvFormatCommand(&pCmd, szFormat, ap);
    va_end(ap);

    if (nLen < 0)
    {
    	free(pCmd);
        return -1;
    }

    if(nCmdSize < nLen)
    {
    	free(pCmd);
    	return 0;
    }

    nCmdSize = nLen;
    strncpy(szCommand, pCmd, nCmdSize);

    free(pCmd);

    return 0;
}

int32_t CRedisRaw::SendCommand(const char *szCommand, char *pKey, void *pSession/* = NULL*/)
{
	int32_t nStatus;
	if(pSession == NULL)
	{
		nStatus = redisAsyncCommand(m_pRedisContext, NULL, NULL, "%s %b", szCommand, pKey, strlen(pKey));
	}
	else
	{
    	uint32_t *pSessionIndex = ((CSessionIndexBank *)g_Frame.GetBank(BANK_SESSION_INDEX))->CreateSessionIndex();
    	*pSessionIndex = ((RedisSession *)pSession)->GetSessionIndex();
		nStatus = redisAsyncCommand(m_pRedisContext, &CRedisGlue::CB_RedisReply, pSessionIndex, "%s %b", szCommand, pKey, strlen(pKey));
	}

	return nStatus;
}

int32_t CRedisRaw::SendCommand(const char *szCommand, char *szKey, void *pSession, const char *szFormat, va_list ap)
{
    char *pCmd;
    int32_t nLen;
//    int32_t nStatus;
    nLen = redisvFormatCommand(&pCmd, szFormat, ap);

    /* We don't want to pass -1 or -2 to future functions as a length. */
    if (nLen < 0)
        return REDIS_ERR;

    char szCmd[MAX_REDIS_CMD_SIZE];
    char *pEnd = strchr(pCmd, '\r');
    if((pEnd == NULL) && (pCmd != NULL))
    {
    	free(pCmd);
    	return 0;
    }

    int32_t nIndex = pEnd - pCmd;
    pCmd[nIndex] = '\0';
    int32_t nRowCount = atoi(&pCmd[1]);
    int32_t nCmdLen = sprintf(szCmd, "*%d\r\n$%zd\r\n%s\r\n$%zd\r\n%s", nRowCount + 2, strlen(szCommand), szCommand, strlen(szKey), szKey);
    pCmd[nIndex] = '\r';
    memcpy(&szCmd[nCmdLen], &pCmd[nIndex], nLen - nIndex);

    size_t nTotalSize = nCmdLen + nLen - nIndex;
    szCmd[nTotalSize] = '\0';

//    if(pSession == NULL)
//    {
//    	nStatus = redisAsyncFormattedCommand(m_pRedisContext, NULL, NULL, szCmd, nTotalSize);
//    }
//    else
//    {
//    	uint32_t *pSessionIndex = ((CSessionIndexBank *)g_Frame.GetBank(BANK_SESSION_INDEX))->CreateSession();
//    	*pSessionIndex = ((RedisSession *)pSession)->GetSessionIndex();
//		nStatus = redisAsyncFormattedCommand(m_pRedisContext, &CRedisGlue::CB_RedisReply, pSessionIndex, szCmd, nTotalSize);
//    }

    free(pCmd);

    return SendFormatedCommand(pSession, szCmd, nTotalSize);
}

int32_t CRedisRaw::SendCommand(const char *szCommand, char *szKey, void *pSession, const char *szFormat, ...)
{
    va_list ap;
    va_start(ap, szFormat);
    int32_t nStatus = SendCommand(szCommand, szKey, pSession, szFormat, ap);
    va_end(ap);

	return nStatus;
}

int32_t CRedisRaw::SendCommand(const char *szCommand, char *szKey, void *pSession, int32_t nArgc, const char **Argv, const size_t *ArgvLen)
{
	int32_t nStatus;
	if(pSession == NULL)
	{
		nStatus = redisAsyncCommandArgv(m_pRedisContext, NULL, NULL, nArgc, Argv, ArgvLen);
	}
	else
	{
    	uint32_t *pSessionIndex = ((CSessionIndexBank *)g_Frame.GetBank(BANK_SESSION_INDEX))->CreateSessionIndex();
    	*pSessionIndex = ((RedisSession *)pSession)->GetSessionIndex();
		nStatus = redisAsyncCommandArgv(m_pRedisContext, &CRedisGlue::CB_RedisReply, pSessionIndex, nArgc, Argv, ArgvLen);
	}

	return nStatus;
}

int32_t CRedisRaw::SendFormatedCommand(void *pSession, char *szCommand, int32_t nCmdLen)
{
	int32_t nStatus;

    if(pSession == NULL)
    {
    	nStatus = redisAsyncFormattedCommand(m_pRedisContext, NULL, NULL, szCommand, nCmdLen);
    }
    else
    {
    	uint32_t *pSessionIndex = ((CSessionIndexBank *)g_Frame.GetBank(BANK_SESSION_INDEX))->CreateSessionIndex();
    	*pSessionIndex = ((RedisSession *)pSession)->GetSessionIndex();
		nStatus = redisAsyncFormattedCommand(m_pRedisContext, &CRedisGlue::CB_RedisReply, pSessionIndex, szCommand, nCmdLen);
    }

    return nStatus;
}

FRAME_NAMESPACE_END
