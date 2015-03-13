/*
 * redis_channel.cpp
 *
 *  Created on: Mar 11, 2015
 *      Author: jimm
 */

#include "redis_channel.h"
#include "../include/cachekey_define.h"

CRedisChannel::CRedisChannel(int32_t nServerID, char *pAddress, uint16_t nPort, char *pChannelKey, char *pChannelMode)
:CRedisAgent(nServerID, pAddress, nPort)
{
	m_strChannelKey = string(pChannelKey);
	if(pChannelMode != NULL)
	{
		m_strChannelMode = string(pChannelMode);
	}

	m_pRedisReplyHandler = NULL;
}

int32_t CRedisChannel::ConnectSuccess()
{
	if((m_strChannelMode == string("subscribe")) && m_pRedisReplyHandler != NULL)
	{
		Subscribe(m_pRedisReplyHandler);
	}

	return 0;
}

int32_t CRedisChannel::RPush(char *pValue, uint16_t nValueLen, IRedisReplyHandler *pReplyHandler/* = NULL*/, CBaseObject *pParam/* = NULL*/)
{
	return CRedisAgent::RPush((char *)(m_strChannelKey.c_str()), pValue, nValueLen, pReplyHandler, pParam);
}

int32_t CRedisChannel::LPop(IRedisReplyHandler *pReplyHandler/* = NULL*/, CBaseObject *pParam/* = NULL*/)
{
	return CRedisAgent::LPop((char *)(m_strChannelKey.c_str()), pReplyHandler, pParam);
}

//----------------------------sub/pub--------------------------------------
int32_t CRedisChannel::Subscribe(IRedisReplyHandler *pReplyHandler/* = NULL*/, CBaseObject *pParam/* = NULL*/)
{
	IRedisReplyHandler *pRedisReplyHandler = NULL;
	if(pReplyHandler == NULL)
	{
		pRedisReplyHandler = m_pRedisReplyHandler;
	}
	else
	{
		pRedisReplyHandler = pReplyHandler;
	}

	return CRedisAgent::Subscribe((char *)(m_strChannelKey.c_str()), pRedisReplyHandler, pParam);
}

int32_t CRedisChannel::Unsubscribe(IRedisReplyHandler *pReplyHandler/* = NULL*/, CBaseObject *pParam/* = NULL*/)
{
	return CRedisAgent::Unsubscribe((char *)(m_strChannelKey.c_str()), pReplyHandler, pParam);
}

void CRedisChannel::OnUnsubscribeReply(const redisAsyncContext *pContext, void *pReply, void *pSession)
{
	return CRedisAgent::OnUnsubscribeReply(pContext, pReply, pSession);
}

int32_t CRedisChannel::Publish(char *pValue, uint16_t nValueLen, IRedisReplyHandler *pReplyHandler/* = NULL*/, CBaseObject *pParam/* = NULL*/)
{
	return CRedisAgent::Publish((char *)(m_strChannelKey.c_str()), pValue, nValueLen, pReplyHandler, pParam);
}

void CRedisChannel::AttachReplyHandler(IRedisReplyHandler *pReplyHandler)
{
	m_pRedisReplyHandler = pReplyHandler;
}

