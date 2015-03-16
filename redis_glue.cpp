/*
 * redis_bank.cpp
 *
 *  Created on: Feb 10, 2015
 *      Author: jimm
 */

#include "redis_glue.h"
#include "../logger/logger.h"

using namespace LOGGER;

FRAME_NAMESPACE_BEGIN

map<const redisAsyncContext*, CRedisRaw *> CRedisGlue::m_stRedisContextMap;

CRedisGlue::CRedisGlue()
{

}

CRedisGlue::~CRedisGlue()
{

}

void CRedisGlue::RegRedisContext(redisAsyncContext *pContext, CRedisRaw *pAgent)
{
	CRedisGlue::m_stRedisContextMap[pContext] = pAgent;

	redisAsyncSetConnectCallback(pContext, &CRedisGlue::CB_Connect);
	redisAsyncSetDisconnectCallback(pContext, &CRedisGlue::CB_Close);
}

void CRedisGlue::UnregRedisContext(redisAsyncContext *pContext)
{
	CRedisGlue::m_stRedisContextMap.erase(pContext);
}

void CRedisGlue::CB_Connect(const redisAsyncContext *pContext, int32_t nStatus)
{
	map<const redisAsyncContext*, CRedisRaw *>::iterator it = CRedisGlue::m_stRedisContextMap.find(pContext);
	if(it != CRedisGlue::m_stRedisContextMap.end())
	{
		it->second->Connected(pContext, nStatus);
	}
	else
	{
		WRITE_WARN_LOG(MODULE_NAME, "redis connected!but it's not found redis context!\n");
	}
}

void CRedisGlue::CB_Close(const redisAsyncContext *pContext, int32_t nStatus)
{
	map<const redisAsyncContext*, CRedisRaw *>::iterator it = CRedisGlue::m_stRedisContextMap.find(pContext);
	if(it != CRedisGlue::m_stRedisContextMap.end())
	{
		it->second->Closed(pContext, nStatus);
	}
	else
	{
		WRITE_WARN_LOG(MODULE_NAME, "redis closed!but it's not found redis context!\n");
	}
}

void CRedisGlue::CB_RedisReply(redisAsyncContext *pContext, void *pReply, void *pSession)
{
	map<const redisAsyncContext*, CRedisRaw *>::iterator it = CRedisGlue::m_stRedisContextMap.find(pContext);
	if(it != CRedisGlue::m_stRedisContextMap.end())
	{
		it->second->OnRedisReply(pContext, pReply, pSession);
	}
	else
	{
		WRITE_WARN_LOG(MODULE_NAME, "recv redis reply!but it's not found redis context!\n");
	}
}

void CRedisGlue::CB_UnsubscribeReply(redisAsyncContext *pContext, void *pReply, void *pSession)
{
	map<const redisAsyncContext*, CRedisRaw *>::iterator it = CRedisGlue::m_stRedisContextMap.find(pContext);
	if(it != CRedisGlue::m_stRedisContextMap.end())
	{
		it->second->OnUnsubscribeReply(pContext, pReply, pSession);
	}
	else
	{
		WRITE_WARN_LOG(MODULE_NAME, "recv subscribe reply!but it's not found redis context!\n");
	}
}

FRAME_NAMESPACE_END

