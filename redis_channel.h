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
	enum TimeUnit
	{
		enmTimeUnit_Second,
		enmTimeUnit_MilliSecond,
	};

	enum SetType
	{
		enmSetType_NX,		//只在键不存在时，才对键进行设置操作
		enmSetType_XX,		//只在键已经存在时，才对键进行设置操作
	};

	CRedisChannel(int32_t nServerID, char *pAddress, uint16_t nPort, char *pChannelKey);

	virtual int32_t OnConnected();

	virtual void OnClosed();

	//---------------------------transaction--------------------------------
	void Multi();

	void Exec(RedisSession *pSession);

	//---------------------------key-----------------------------------------
	int32_t Del(RedisSession *pSession, char *szKey);

	int32_t Del(RedisSession *pSession, const char *szFormat, ...);

	//---------------------------string--------------------------------------

	int32_t Get(RedisSession *pSession, char *szKey);

	int32_t IncrBy(RedisSession *pSession, char *szTarget, int64_t nIncrement);

	int32_t Incr(RedisSession *pSession, char *szTarget);

	//SET key value [EX seconds] [PX milliseconds] [NX|XX]
	int32_t Set(RedisSession *pSession, char *szTarget, char *szValue);

	int32_t Set(RedisSession *pSession, char *szTarget, int64_t nValue);

	int32_t Set(RedisSession *pSession, char *szTarget, char *szValue, enum TimeUnit nUnit, int64_t nTime);

	int32_t Set(RedisSession *pSession, char *szTarget, char *szValue, enum TimeUnit nUnit, int64_t nTime, enum SetType nType);

	int32_t Set(RedisSession *pSession, char *szTarget, char *szValue, enum SetType nType);

	int32_t SetNX(RedisSession *pSession, char *szKey, char *szValue);

	int32_t SetNX(RedisSession *pSession, char *szKey, int64_t nValue);

	int32_t GetSet(RedisSession *pSession, char *szKey, char *szValue);

	int32_t GetSet(RedisSession *pSession, char *szKey, int64_t nValue);

	//---------------------------hash----------------------------------------
	int32_t HDel(RedisSession *pSession, char *szTarget, const char *szFormat, ...);

	int32_t HMSet(RedisSession *pSession, char *szTarget, const char *szFormat, ...);

	int32_t HMSet(RedisSession *pSession, char *szTarget, int32_t nArgc, const char **Argv, const size_t *ArgvLen);

	int32_t HMGet(RedisSession *pSession, char *szTarget, const char *szFormat, ...);

	int32_t HIncrBy(RedisSession *pSession, char *szTarget, const char *szFormat, ...);

	int32_t HExists(RedisSession *pSession, char *szTarget, const char *szFormat, ...);

	//---------------------------list-----------------------------------------
	int32_t BLPop(RedisSession *pSession, int32_t nTimeout);

	int32_t RPush(RedisSession *pSession, char *pValue, uint16_t nValueLen);

	int32_t RPush(RedisSession *pSession, char *szTarget, char *pValue, uint16_t nValueLen);

	int32_t LPop(RedisSession *pSession = NULL);

	int32_t LPush(RedisSession *pSession, char *pValue, uint16_t nValueLen);

	int32_t LPush(RedisSession *pSession, char *szTarget, char *pValue, uint16_t nValueLen);

	//---------------------------set-----------------------------------------
	int32_t SAdd(RedisSession *pSession, char *szTarget, const char *szFormat, ...);

	int32_t SRem(RedisSession *pSession, char *szTarget, const char *szFormat, ...);

	//---------------------------sortedset-------------------------------------
	int32_t ZAdd(RedisSession *pSession, char *szTarget, const char *szFormat, ...);

	int32_t ZCount(RedisSession *pSession, char *szTarget, int32_t nMinIndex = -1, int32_t nMaxIndex = -1);

	int32_t ZRem(RedisSession *pSession, char *szTarget, const char *szFormat, ...);

	int32_t ZRangeByScore(RedisSession *pSession, char *szTarget, int32_t nMinIndex = -1, int32_t nMaxIndex = -1,
			bool bWithScores = false, int32_t nOffset = 0, int32_t nCount = -1);

	int32_t ZRemRangeByRank(RedisSession *pSession, char *szTarget, int32_t nStartIndex = 0, int32_t nStopIndex = -1);

	//----------------------------sub/pub--------------------------------------
	int32_t Subscribe(RedisSession *pSession);

	int32_t Unsubscribe(RedisSession *pSession);

	void OnUnsubscribeReply(const redisAsyncContext *pContext, void *pReply, void *pSessionIndex);

	int32_t Publish(RedisSession *pSession, char *pValue, int32_t nValueLen);

protected:
	string				m_strChannelKey;
};


#endif /* REDIS_CHANNEL_H_ */
