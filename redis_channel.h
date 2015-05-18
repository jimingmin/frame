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

	CRedisChannel(int32_t nServerID, char *pAddress, uint16_t nPort);

	virtual int32_t OnConnected();

	virtual void OnClosed();

	//---------------------------transaction--------------------------------
	void Multi();

	void Exec(RedisSession *pSession);

	//---------------------------key-----------------------------------------
	int32_t Del(RedisSession *pSession, const char *szKey);

//	int32_t Del(RedisSession *pSession, const char *szFormat, ...);

	int32_t Expire(RedisSession *pSession, const char *szKey, int32_t nTimeout);

	//---------------------------string--------------------------------------

	int32_t Get(RedisSession *pSession, const char *szKey);

	int32_t IncrBy(RedisSession *pSession, const char *szKey, int64_t nIncrement);

	int32_t Incr(RedisSession *pSession, const char *szKey);

	//SET key value [EX seconds] [PX milliseconds] [NX|XX]
	int32_t Set(RedisSession *pSession, const char *szKey, const char *szValue);

	int32_t Set(RedisSession *pSession, const char *szKey, int64_t nValue);

	int32_t Set(RedisSession *pSession, const char *szKey, const char *szValue, enum TimeUnit nUnit, int64_t nTime);

	int32_t Set(RedisSession *pSession, const char *szKey, const char *szValue, enum TimeUnit nUnit, int64_t nTime, enum SetType nType);

	int32_t Set(RedisSession *pSession, const char *szKey, const char *szValue, enum SetType nType);

	int32_t SetNX(RedisSession *pSession, const char *szKey, const char *szValue);

	int32_t SetNX(RedisSession *pSession, const char *szKey, int64_t nValue);

	int32_t GetSet(RedisSession *pSession, const char *szKey, const char *szValue);

	int32_t GetSet(RedisSession *pSession, const char *szKey, int64_t nValue);

	//---------------------------hash----------------------------------------
	int32_t HDel(RedisSession *pSession, const char *szKey, const char *szFormat, ...);

	int32_t HMSet(RedisSession *pSession, const char *szKey, const char *szFormat, ...);

	int32_t HMSet(RedisSession *pSession, const char *szKey, int32_t nArgc, const char **Argv, const size_t *ArgvLen);

	int32_t HMGet(RedisSession *pSession, const char *szKey, const char *szFormat, ...);

	int32_t HIncrBy(RedisSession *pSession, const char *szKey, const char *szField, int64_t nIncr);

	int32_t HExists(RedisSession *pSession, const char *szKey, const char *szField);

	//---------------------------list-----------------------------------------
	int32_t BLPop(RedisSession *pSession, const char *szKey, int32_t nTimeout);

	int32_t RPush(RedisSession *pSession, const char *szKey, const char *szValue, uint16_t nValueLen);

	int32_t LPop(RedisSession *pSession, const char *szKey);

	int32_t LPush(RedisSession *pSession, const char *szKey, const char *szValue, uint16_t nValueLen);

	//---------------------------set-----------------------------------------
	int32_t SAdd(RedisSession *pSession, const char *szKey, const char *szFormat, ...);

	int32_t SIsMember(RedisSession *pSession, const char *szKey, int64_t nField);

	int32_t SIsMember(RedisSession *pSession, const char *szKey, const char *szField);

	int32_t SRem(RedisSession *pSession, const char *szKey, const char *szFormat, ...);

	//---------------------------sortedset-------------------------------------
	int32_t ZAdd(RedisSession *pSession, const char *szKey, const char *szFormat, ...);

	int32_t ZCount(RedisSession *pSession, const char *szKey, int32_t nMinIndex = -1, int32_t nMaxIndex = -1);

	int32_t ZRem(RedisSession *pSession, const char *szKey, const char *szFormat, ...);

	int32_t ZRangeByScore(RedisSession *pSession, const char *szKey, int32_t nMinIndex = -1, int32_t nMaxIndex = -1,
			bool bWithScores = false, int32_t nOffset = 0, int32_t nCount = -1);

	int32_t ZRank(RedisSession *pSession, const char *szKey, int64_t nField);

	int32_t ZRank(RedisSession *pSession, const char *szKey, const char *szField);

	int32_t ZRemRangeByRank(RedisSession *pSession, const char *szKey, int32_t nStartIndex = 0, int32_t nStopIndex = -1);

	int32_t ZScore(RedisSession *pSession, const char *szKey, int64_t nField);

	int32_t ZScore(RedisSession *pSession, const char *szKey, const char *szField);

	//----------------------------sub/pub--------------------------------------
	int32_t Subscribe(RedisSession *pSession, const char *szKey);

	int32_t Unsubscribe(RedisSession *pSession, const char *szKey);

	void OnUnsubscribeReply(const redisAsyncContext *pContext, void *pReply, void *pSessionIndex);

	int32_t Publish(RedisSession *pSession, const char *szKey, const char *szValue, int32_t nValueLen);

};


#endif /* REDIS_CHANNEL_H_ */
