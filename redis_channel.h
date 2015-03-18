/*
 * redis_channel.h
 *
 *  Created on: Mar 11, 2015
 *      Author: jimm
 */

#ifndef REDIS_CHANNEL_H_
#define REDIS_CHANNEL_H_

#include "redis_raw.h"
#include "redis_session.h"
#include <string>
using namespace std;
using namespace FRAME;

class CRedisChannel : public CRedisRaw
{
public:
	CRedisChannel(int32_t nServerID, char *pAddress, uint16_t nPort, char *pChannelKey, char *pChannelMode);

	virtual int32_t OnConnected();

	//---------------------------string--------------------------------------
	int32_t IncrBy(RedisSession *pSession, char *szTarget, int64_t nIncrement);

	int32_t Incr(RedisSession *pSession, char *szTarget);

	//---------------------------hash----------------------------------------
	int32_t HMSet(RedisSession *pSession, char *szTarget, const char *szFormat, ...);

	int32_t HMSet(RedisSession *pSession, char *szTarget, int32_t nArgc, const char **Argv, const size_t *ArgvLen);

	int32_t HMGet(RedisSession *pSession, char *szTarget, const char *szFormat, ...);

	int32_t HIncrBy(RedisSession *pSession, char *szTarget, const char *szFormat, ...);

	int32_t HExists(RedisSession *pSession, char *szTarget, const char *szFormat, ...);

	//---------------------------list-----------------------------------------

	int32_t RPush(RedisSession *pSession, char *pValue, uint16_t nValueLen);

	int32_t LPop(RedisSession *pSession = NULL);

	//----------------------------sub/pub--------------------------------------
	int32_t Subscribe(RedisSession *pSession);

	int32_t Unsubscribe(RedisSession *pSession);

	void OnUnsubscribeReply(const redisAsyncContext *pContext, void *pReply, void *pSessionIndex);

	int32_t Publish(RedisSession *pSession, char *pValue, int32_t nValueLen);

	void AttachSession(RedisSession *pSubscribeSession);

protected:
	string				m_strChannelKey;
	string				m_strChannelMode;
	RedisSession		*m_pSubscribeSession;
};


#endif /* REDIS_CHANNEL_H_ */
