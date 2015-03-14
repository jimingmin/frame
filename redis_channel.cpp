/*
 * redis_channel.cpp
 *
 *  Created on: Mar 11, 2015
 *      Author: jimm
 */

#include "redis_channel.h"
#include "../common/common_memmgt.h"
#include "../include/cachekey_define.h"

CRedisChannel::CRedisChannel(int32_t nServerID, char *pAddress, uint16_t nPort, char *pChannelKey, char *pChannelMode)
:CRedisAgent(nServerID, pAddress, nPort)
{
	m_strChannelKey = string(pChannelKey);
	if(pChannelMode != NULL)
	{
		m_strChannelMode = string(pChannelMode);
	}

	m_pSubscribeSession = NULL;
}

int32_t CRedisChannel::OnConnected()
{
	if((m_strChannelMode == string("subscribe")) && m_pSubscribeSession != NULL)
	{
		Subscribe(m_pSubscribeSession);
	}

	return 0;
}

//---------------------------hash----------------------------------------
int32_t CRedisChannel::HMSet(RedisSession *pSession, char *szTarget, const char *szFormat, ...)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("HMSET", (char *)strKey.c_str(), pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

int32_t CRedisChannel::HMGet(RedisSession *pSession, char *szTarget, const char *szFormat, ...)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("HMGET", (char *)strKey.c_str(), pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

int32_t CRedisChannel::HIncrBy(RedisSession *pSession, char *szTarget, const char *szFormat, ...)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("HINCRBY", (char *)strKey.c_str(), pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

//---------------------------list-----------------------------------------
int32_t CRedisChannel::RPush(RedisSession *pSession, char *pValue, uint16_t nValueLen)
{
	return SendCommand("RPush", (char *)m_strChannelKey.c_str(), pSession, "%b", pValue, nValueLen);
}

int32_t CRedisChannel::LPop(RedisSession *pSession)
{
	return SendCommand("LPOP", (char *)m_strChannelKey.c_str(), pSession);
}

//----------------------------sub/pub--------------------------------------
int32_t CRedisChannel::Subscribe(RedisSession *pSession)
{
	return SendCommand("SUBSCRIBE", (char *)m_strChannelKey.c_str(), pSession);
}

int32_t CRedisChannel::Unsubscribe(RedisSession *pSession)
{
	return SendCommand("UNSUBSCRIBE", (char *)m_strChannelKey.c_str(), pSession);
}

void CRedisChannel::OnUnsubscribeReply(const redisAsyncContext *pContext, void *pReply, void *pSession)
{
	RedisSession *pRedisSession = (RedisSession *)pSession;
	delete pRedisSession->GetHandlerObj();
	delete pRedisSession;
}

int32_t CRedisChannel::Publish(RedisSession *pSession, char *pValue, int32_t nValueLen)
{
	return SendCommand("PUBLISH", (char *)m_strChannelKey.c_str(), pSession, "%b", pValue, (size_t)nValueLen);
}

void CRedisChannel::AttachSession(RedisSession *pSubscribeSession)
{
	m_pSubscribeSession = pSubscribeSession;
}

