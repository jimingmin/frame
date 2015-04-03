/*
 * redislock_bank.h
 *
 *  Created on: 2015年4月3日
 *      Author: jimm
 */

#ifndef REDISLOCK_BANK_H_
#define REDISLOCK_BANK_H_

#include "frame_impl.h"
#include <list>
#include <map>
#include "redis_lock.h"
#include "redis_channel.h"

using namespace std;
using namespace FRAME;

#define BANK_REDIS_LOCK				"bank_redis_lock"
#define MAX_REDISLOCK_COUNT		100

class CRedisLockBank : public IBank
{
public:
	//初始化
	virtual int32_t Init();
	//卸载
	virtual int32_t Uninit();

	RedisLock *CreateLock(CRedisChannel *pRedisChannel, CBaseObject *pHandlerObj, LockResult Result, int64_t nTimeout = 60 * MS_PER_SECOND);

	RedisLock *GetLock(uint32_t nSessionIndex);

	void DestroyLock(RedisLock *pRedisLock);

	void DestroyLock(uint32_t nSessionIndex);

	int32_t OnLockTimeout(CTimer *pTimer);

protected:
	uint32_t						m_nLockCounter;
	list<RedisLock *>			m_stUnusedLockList;
	map<uint32_t, RedisLock *>	m_stUsedLockMap;
};


#endif /* REDISLOCK_BANK_H_ */
