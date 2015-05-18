/*
 * redis_glue.h
 *
 *  Created on: Feb 10, 2015
 *      Author: jimm
 */

#ifndef REDIS_GLUE_H_
#define REDIS_GLUE_H_

#include "redis_raw.h"
#include "hiredis/hiredis.h"
#include "frame_namespace.h"
#include "frame_impl.h"
#include <map>
using namespace std;

#define MODULE_NAME "redis"

FRAME_NAMESPACE_BEGIN

class CRedisGlue
{
public:
	CRedisGlue();

	~CRedisGlue();

	static void RegRedisContext(redisAsyncContext *pContext, CRedisRaw *pAgent);

	static void UnregRedisContext(redisAsyncContext *pContext);

	static void CB_Connect(const redisAsyncContext *pContext, int32_t nStatus);

	static void CB_Close(const redisAsyncContext *pContext, int32_t nStatus);

	static void CB_RedisReply(redisAsyncContext *pContext, void *pReply, void *pSession);

	static void CB_UnsubscribeReply(redisAsyncContext *pContext, void *pReply, void *pSession);

private:
	static map<const redisAsyncContext*, CRedisRaw *>	m_stRedisContextMap;
};

FRAME_NAMESPACE_END

#endif /* REDIS_GLUE_H_ */
