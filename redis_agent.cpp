/*
 * redis_agent.cpp
 *
 *  Created on: Feb 10, 2015
 *      Author: jimm
 */

#include <stdio.h>
#include <string.h>
#include "redis_agent.h"
#include "redis_glue.h"
#include "redis_session.h"
#include "frame.h"
#include "../logger/logger.h"
#include "../common/common_memmgt.h"

using namespace LOGGER;

FRAME_NAMESPACE_BEGIN

CRedisAgent::CRedisAgent(int32_t nServerID, char *pAddress, uint16_t nPort)
{
	m_pRedisEvtBase = NULL;
	m_pRedisContext = NULL;

	m_nServerID = nServerID;
	strcpy(m_arrAddress, pAddress);
	m_nPort = nPort;
	m_bConnectSuccess = false;
}

CRedisAgent::~CRedisAgent()
{

}

int32_t CRedisAgent::Run()
{
	if(m_pRedisContext == NULL)
	{
		return 0;
	}

	//return event_base_dispatch(m_pRedisEvtBase);
	return event_base_loop(m_pRedisEvtBase, EVLOOP_NONBLOCK);
}

int32_t CRedisAgent::OnConnected()
{
	return 0;
}

void CRedisAgent::OnClosed()
{

}

int32_t CRedisAgent::Connect()
{
	m_pRedisEvtBase = event_base_new();
	m_pRedisContext = redisAsyncConnect(m_arrAddress, m_nPort);
	if(m_pRedisContext->err)
	{
		WRITE_WARN_LOG(MODULE_NAME, "connect redis error!{error=%s, server_id=%d, address=%s, port=%d}\n",
				m_pRedisContext->errstr, m_nServerID, m_arrAddress, m_nPort);

		event_base_free(m_pRedisEvtBase);
		m_pRedisEvtBase = NULL;
		m_pRedisEvtBase = NULL;
		return 1;
	}

	redisLibeventAttach(m_pRedisContext, m_pRedisEvtBase);

	CRedisGlue::RegRedisContext(m_pRedisContext, this);

	g_Frame.AddRunner(this);

	WRITE_INFO_LOG(MODULE_NAME, "connect redis!{server_id=%d, address=%s, port=%d}\n", m_nServerID, m_arrAddress, m_nPort);

	return 0;
}

void CRedisAgent::Close()
{
	redisAsyncDisconnect(m_pRedisContext);
}

bool CRedisAgent::IsConnected()
{
	return m_bConnectSuccess;
}

void CRedisAgent::Connected(const redisAsyncContext *pContext, int32_t nStatus)
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

void CRedisAgent::Closed(const redisAsyncContext *pContext, int32_t nStatus)
{
	OnClosed();

	m_bConnectSuccess = false;

    if (nStatus != REDIS_OK) {
		WRITE_WARN_LOG(MODULE_NAME, "close redis occur error!{error=%s, server_id=%d, address=%s, port=%d}\n",
				pContext->errstr, m_nServerID, m_arrAddress, m_nPort);

        event_base_free(m_pRedisEvtBase);
        CRedisGlue::UnregRedisContext(m_pRedisContext);

        m_pRedisEvtBase = NULL;
        m_pRedisContext = NULL;
        return;
    }

    WRITE_INFO_LOG(MODULE_NAME, "redis connection is closed!{server_id=%d, address=%s, port=%d}\n", m_nServerID, m_arrAddress, m_nPort);

    event_base_free(m_pRedisEvtBase);
    CRedisGlue::UnregRedisContext(m_pRedisContext);

    m_pRedisEvtBase = NULL;
    m_pRedisContext = NULL;
}

void CRedisAgent::OnRedisReply(const redisAsyncContext *pContext, void *pReply, void *pSession)
{
	if(pSession == NULL)
	{
		return;
	}

    if (pReply == NULL)
    {
    	return;
    }

    redisReply *pRedisReply = (redisReply *)pReply;
    RedisSession *pRedisSession = (RedisSession *)pSession;
    CBaseObject *pHandler = pRedisSession->GetHandlerObj();
    HandleRedisReply Proc = pRedisSession->GetHandleRedisReply();

    (pHandler->*Proc)(pContext->err, pReply, pRedisSession);
}

void CRedisAgent::OnUnsubscribeReply(const redisAsyncContext *pContext, void *pReply, void *pSession)
{

}

int32_t CRedisAgent::SendCommand(const char *szCommand, char *pKey, void *pSession/* = NULL*/)
{
	int32_t nStatus;
	if(pSession == NULL)
	{
		nStatus = redisAsyncCommand(m_pRedisContext, NULL, NULL, "%s %b", szCommand, pKey, strlen(pKey));
	}
	else
	{
		nStatus = redisAsyncCommand(m_pRedisContext, &CRedisGlue::CB_RedisReply, pSession, "%s %b", szCommand, pKey, strlen(pKey));
	}

	return nStatus;
}

int32_t CRedisAgent::SendCommand(const char *szCommand, char *szKey, void *pSession, const char *szFormat, va_list ap)
{
    char *pCmd;
    int32_t nLen;
    int32_t nStatus;
    nLen = redisvFormatCommand(&pCmd,szFormat,ap);

    /* We don't want to pass -1 or -2 to future functions as a length. */
    if (nLen < 0)
        return REDIS_ERR;

    char szCmd[10 * 1024];
    char *pEnd = strchr(pCmd, '\r');
    if((pEnd == NULL) && (pCmd != NULL))
    {
    	free(pCmd);
    	return 0;
    }

    int32_t nIndex = pEnd - pCmd;
    pCmd[nIndex] = '\0';
    int32_t nRowCount = atoi(&pCmd[1]);
    int32_t nCmdLen = sprintf(szCmd, "*%d\r\n$%d\r\n%s\r\n$%d\r\n%s", nRowCount + 2, strlen(szCommand), szCommand, strlen(szKey), szKey);
    pCmd[nIndex] = '\r';
    memcpy(&szCmd[nCmdLen], &pCmd[nIndex], nLen - nIndex);

    size_t nTotalSize = nCmdLen + nLen - nIndex;
    szCmd[nTotalSize] = '\0';

    if(pSession == NULL)
    {
    	nStatus = redisAsyncFormattedCommand(m_pRedisContext, NULL, NULL, szCmd, nTotalSize);
    }
    else
    {
		nStatus = redisAsyncFormattedCommand(m_pRedisContext, &CRedisGlue::CB_RedisReply, pSession, szCmd, nTotalSize);
    }

    free(pCmd);

    return nStatus;
}

int32_t CRedisAgent::SendCommand(const char *szCommand, char *szKey, void *pSession, const char *szFormat, ...)
{
    va_list ap;
    va_start(ap, szFormat);
    int32_t nStatus = SendCommand(szCommand, szKey, pSession, szFormat, ap);
    va_end(ap);

	return nStatus;
}

FRAME_NAMESPACE_END
