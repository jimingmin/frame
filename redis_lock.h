/*
 * redis_lock.h
 *
 *  Created on: 2015年4月2日
 *      Author: jimm
 */

#ifndef REDIS_LOCK_H_
#define REDIS_LOCK_H_

#include "common/common_object.h"
#include "frame_timer.h"
#include "frame.h"
#include "redis_channel.h"
#include "redis_session.h"
#include <stddef.h>
#include <string>

using namespace std;
using namespace FRAME;

class RedisLock;

typedef void (CBaseObject::*LockResult)(int32_t nResult, RedisLock *pRedisLock);

class RedisLock : public CBaseObject
{
public:
	enum
	{
		enmRedisLock_Success			= 0x00,
		enmRedisLock_Fail				= 0x01,
		enmRedisLock_RedisError		= 0x02,
		enmRedisLock_Timeout			= 0x03,
	};

	RedisLock()
	{
		m_nSessionIndex = 0;
		m_nLockExpireTime = 0;
		m_pRedisChannel = NULL;
		m_pHandlerObj = NULL;
		m_fcLockResult = NULL;
	}

	virtual ~RedisLock()
	{
		Uninit();
	}

	virtual int32_t Init()
	{
		return 0;
	}
	virtual int32_t Uninit()
	{
		return 0;
	}
	virtual int32_t GetSize()
	{
		return 0;
	}

	void SetSessionIndex(uint32_t nSessionIndex);

	uint32_t GetSessionIndex();

	uint8_t *GetSessionData();

	void SetRedisChannel(CRedisChannel *pRedisChannel);

	CRedisChannel *GetRedisChannel();

	void SetHandlerObj(CBaseObject *pObj);

	CBaseObject *GetHandlerObj();

	void SetLockResultCallback(LockResult result);

	LockResult GetLockResultCallback();

	int32_t Lock(char *szKey);

	int32_t Unlock();

	int32_t OnSessionSetNX(int32_t nResult, void *pReply, void *pSession);

	int32_t OnSessionGet(int32_t nResult, void *pReply, void *pSession);

	int32_t OnSessionGetSet(int32_t nResult, void *pReply, void *pSession);

//	int32_t OnRedisSessionTimeout(void *pTimerData);

protected:
	string				m_strLockName;
	int64_t				m_nLockExpireTime;
	uint32_t				m_nSessionIndex;
	CRedisChannel		*m_pRedisChannel;
	CBaseObject			*m_pHandlerObj;
	LockResult			m_fcLockResult;
};



#endif /* REDIS_LOCK_H_ */
