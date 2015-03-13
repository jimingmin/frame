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

int32_t CRedisAgent::ConnectSuccess()
{
	return 0;
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

int32_t CRedisAgent::RPush(char *szKey, char *pValue, uint16_t nValueLen, IRedisReplyHandler *pReplyHandler/* = NULL*/, CBaseObject *pParam/* = NULL*/)
{
	RedisCmdSession *pSession = NULL;
	if(pReplyHandler != NULL)
	{
		pSession = NEW(RedisCmdSession);
		pSession->m_strKey = string(szKey);
		pSession->m_pRSCallback = pReplyHandler;
		pSession->m_pParam = pParam;
		pSession->m_nRCSessionType = enmRCSessionType_Short;
	}
	return SendCommand("RPUSH", szKey, pValue, nValueLen, pSession);
}

int32_t CRedisAgent::LPop(char *szKey, IRedisReplyHandler *pReplyHandler/* = NULL*/, CBaseObject *pParam/* = NULL*/)
{
	RedisCmdSession *pSession = NULL;
	if(pReplyHandler != NULL)
	{
		pSession = NEW(RedisCmdSession);
		pSession->m_strKey = string(szKey);
		pSession->m_pRSCallback = pReplyHandler;
		pSession->m_pParam = pParam;
		pSession->m_nRCSessionType = enmRCSessionType_Short;
	}
	return SendCommand("LPOP", szKey, pSession);
}

int32_t CRedisAgent::Subscribe(char *szKey, IRedisReplyHandler *pReplyHandler/* = NULL*/, CBaseObject *pParam/* = NULL*/)
{
	RedisCmdSession *pSession = NULL;
	if(pReplyHandler != NULL)
	{
		pSession = NEW(RedisCmdSession);
		pSession->m_strKey = string(szKey);
		pSession->m_pRSCallback = pReplyHandler;
		pSession->m_pParam = pParam;
		pSession->m_nRCSessionType = enmRCSessionType_Long;
		m_stLongSessionList.push_back(pSession);
	}
	return SendCommand("SUBSCRIBE", szKey, pSession);
}

int32_t CRedisAgent::Unsubscribe(char *szKey, IRedisReplyHandler *pReplyHandler/* = NULL*/, CBaseObject *pParam/* = NULL*/)
{
	RedisCmdSession *pSession = NULL;
	if(pReplyHandler != NULL)
	{
		pSession = NEW(RedisCmdSession);
		pSession->m_strKey = string(szKey);
		pSession->m_pRSCallback = pReplyHandler;
		pSession->m_pParam = pParam;
		pSession->m_nRCSessionType = enmRCSessionType_Short;
		m_stLongSessionList.push_back(pSession);
	}
	return SendCommand(&CRedisGlue::CB_UnsubscribeReply, pSession, "%s %s", "UNSUBSCRIBE", szKey);
}

void CRedisAgent::OnUnsubscribeReply(const redisAsyncContext *pContext, void *pReply, void *pSession)
{
	if(pSession == NULL)
	{
		return;
	}
//
//    if (pReply == NULL)
//    {
//    	return;
//    }
//
//    redisReply *pRedisReply = (redisReply *)pReply;
    RedisCmdSession *pRedisSession = (RedisCmdSession *)pSession;

    //删除订阅到长会话对象
    for(list<RedisCmdSession *>::iterator it = m_stLongSessionList.begin(); it != m_stLongSessionList.end();)
    {
    	if((*it)->m_strKey == pRedisSession->m_strKey)
    	{
    		DELETE((*it));
    		it = m_stLongSessionList.erase(it);
    	}
    	else
    	{
    		++it;
    	}
    }
}

int32_t CRedisAgent::Publish(char *szKey, char *pValue, uint16_t nValueLen, IRedisReplyHandler *pReplyHandler/* = NULL*/, CBaseObject *pParam/* = NULL*/)
{
	RedisCmdSession *pSession = NULL;
	if(pReplyHandler != NULL)
	{
		pSession = NEW(RedisCmdSession);
		pSession->m_pRSCallback = pReplyHandler;
		pSession->m_pParam = pParam;
		pSession->m_nRCSessionType = enmRCSessionType_Short;
	}
	return SendCommand("PUBLISH", szKey, pValue, nValueLen, pSession);
}

void CRedisAgent::OnConnected(const redisAsyncContext *pContext, int32_t nStatus)
{
    if (nStatus != REDIS_OK) {
		WRITE_WARN_LOG(MODULE_NAME, "connect redis failed!{error=%s, server_id=%d, address=%s, port=%d}\n",
				pContext->errstr, m_nServerID, m_arrAddress, m_nPort);
        return;
    }

    WRITE_INFO_LOG(MODULE_NAME, "connect redis success!{server_id=%d, address=%s, port=%d}\n", m_nServerID, m_arrAddress, m_nPort);

    m_bConnectSuccess = true;

    ConnectSuccess();
}

void CRedisAgent::OnClosed(const redisAsyncContext *pContext, int32_t nStatus)
{
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
    //删除与redis保持长会话的对象
    for(list<RedisCmdSession *>::iterator it = m_stLongSessionList.begin(); it != m_stLongSessionList.end(); ++it)
    {
    	DELETE((*it));
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
    RedisCmdSession *pRedisSession = (RedisCmdSession *)pSession;

    pRedisSession->OnSession(pContext->err, pReply);

    if(pRedisSession->m_nRCSessionType == enmRCSessionType_Short)
    {
    	DELETE(pRedisSession);
    }
}

bool CRedisAgent::IsConnected()
{
	return m_bConnectSuccess;
}

int32_t CRedisAgent::SendCommand(redisCallbackFn *pFunc, void *pSession, const char *szFormat, ...)
{
    va_list ap;

    va_start(ap, szFormat);
    int32_t nStatus = redisvAsyncCommand(m_pRedisContext, pFunc, pSession, szFormat, ap);
    va_end(ap);

	return nStatus;
}

int32_t CRedisAgent::SendCommand(const char *szCommand, char *pKey, char *pValue, uint16_t nValueLen,
		redisCallbackFn *pFunc/* = NULL*/, void *pSession/* = NULL*/)
{
	return SendCommand(pFunc, pSession, "%s %b %b", szCommand, pKey, strlen(pKey), pValue, nValueLen);
}

int32_t CRedisAgent::SendCommand(const char *szCommand, char *pKey, void *pSession/* = NULL*/)
{
	return redisAsyncCommand(m_pRedisContext, &CRedisGlue::CB_RedisReply, pSession, "%s %b", szCommand, pKey, strlen(pKey));
}

int32_t CRedisAgent::SendCommand(const char *szCommand, char *pKey, char *pValue, uint16_t nValueLen, void *pSession/* = NULL*/)
{
	return redisAsyncCommand(m_pRedisContext, &CRedisGlue::CB_RedisReply, pSession, "%s %s %b", szCommand, pKey, pValue, (size_t)nValueLen);
}

FRAME_NAMESPACE_END
