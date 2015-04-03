/*
 * redis_lock.cpp
 *
 *  Created on: 2015年4月2日
 *      Author: jimm
 */

#include "redis_lock.h"
#include "../common/common_datetime.h"
#include "redissession_bank.h"

void RedisLock::SetSessionIndex(uint32_t nSessionIndex)
{
	m_nSessionIndex = nSessionIndex;
}

uint32_t RedisLock::GetSessionIndex()
{
	return m_nSessionIndex;
}

uint8_t *RedisLock::GetSessionData()
{
	CRedisSessionBank *pRedisSessionBank = (CRedisSessionBank *)g_Frame.GetBank(BANK_REDIS_SESSION);
	RedisSession *pRedisSession = pRedisSessionBank->GetSession(m_nSessionIndex);

	return pRedisSession->GetSessionData();
}

void RedisLock::SetRedisChannel(CRedisChannel *pRedisChannel)
{
	m_pRedisChannel = pRedisChannel;
}

CRedisChannel *RedisLock::GetRedisChannel()
{
	return m_pRedisChannel;
}

void RedisLock::SetHandlerObj(CBaseObject *pObj)
{
	m_pHandlerObj = pObj;
}

CBaseObject *RedisLock::GetHandlerObj()
{
	return m_pHandlerObj;
}

void RedisLock::SetLockResultCallback(LockResult result)
{
	m_fcLockResult = result;
}

LockResult RedisLock::GetLockResultCallback()
{
	return m_fcLockResult;
}

int32_t RedisLock::Lock(char *szKey)
{
	m_strLockName = string(szKey);

	CRedisSessionBank *pRedisSessionBank = (CRedisSessionBank *)g_Frame.GetBank(BANK_REDIS_SESSION);
	RedisSession *pRedisSession = pRedisSessionBank->GetSession(m_nSessionIndex);

	return m_pRedisChannel->SetNX(pRedisSession, szKey, CDateTime::CurrentDateTime().Seconds());
}

int32_t RedisLock::Unlock(char *szKey)
{
	int64_t nCurMilliTime = CTimeValue::CurrentTime().Seconds();
	if(nCurMilliTime > m_nLockExpireTime)
	{
		return 0;
	}

	return m_pRedisChannel->Del(NULL, (char *)m_strLockName.c_str());
}

int32_t RedisLock::OnSessionSetNX(int32_t nResult, void *pReply, void *pSession)
{
	redisReply *pRedisReply = (redisReply *)pReply;
	RedisSession *pRedisSession = (RedisSession *)pSession;

	int32_t nLockResult = enmRedisLock_Success;
	if(pRedisReply->type == REDIS_REPLY_ERROR)
	{
		nLockResult = enmRedisLock_RedisError;
	}
	else if(pRedisReply->type == REDIS_REPLY_INTEGER)
	{
		if(pRedisReply->integer != 0)
		{
			pRedisSession->SetHandleRedisReply(static_cast<RedisReply>(&RedisLock::OnSessionGet));
			return m_pRedisChannel->Get(pRedisSession, (char *)m_strLockName.c_str());
		}
	}
	else
	{
		ASSERT(0);
	}

	(m_pHandlerObj->*m_fcLockResult)(nLockResult, this);

	return 0;
}

int32_t RedisLock::OnSessionGet(int32_t nResult, void *pReply, void *pSession)
{
	redisReply *pRedisReply = (redisReply *)pReply;
	RedisSession *pRedisSession = (RedisSession *)pSession;

	int32_t nLockResult = enmRedisLock_Success;
	if(pRedisReply->type == REDIS_REPLY_ERROR)
	{
		nLockResult = enmRedisLock_RedisError;
	}
	else if(pRedisReply->type == REDIS_REPLY_STRING)
	{
		int64_t nCurMilliTime = CTimeValue::CurrentTime().Milliseconds();
		int64_t nLockMilliTime = atoi64(pRedisReply->str);

		if(nCurMilliTime > nLockMilliTime + (5 * MS_PER_SECOND))
		{
			m_nLockExpireTime = nCurMilliTime + (30 * MS_PER_SECOND);
			pRedisSession->SetHandleRedisReply(static_cast<RedisReply>(&RedisLock::OnSessionGetSet));

			return m_pRedisChannel->GetSet(pRedisSession, (char *)m_strLockName.c_str(), m_nLockExpireTime);
		}
	}
	else
	{
		ASSERT(0);
	}

	(m_pHandlerObj->*m_fcLockResult)(nLockResult, this);

	return 0;
}

int32_t RedisLock::OnSessionGetSet(int32_t nResult, void *pReply, void *pSession)
{
	redisReply *pRedisReply = (redisReply *)pReply;
	RedisSession *pRedisSession = (RedisSession *)pSession;

	int32_t nLockResult = enmRedisLock_Success;
	if(pRedisReply->type == REDIS_REPLY_ERROR)
	{
		nLockResult = enmRedisLock_RedisError;
	}
	else if(pRedisReply->type == REDIS_REPLY_STRING)
	{
		int64_t nGetMilliTime = atoi64(pRedisReply->str);
		if(nGetMilliTime != m_nLockExpireTime)
		{
			nLockResult = enmRedisLock_Fail;
		}
	}

	(m_pHandlerObj->*m_fcLockResult)(nLockResult, this);

	return 0;
}
int32_t RedisLock::OnRedisSessionTimeout(void *pTimerData)
{
	RedisSession *pRedisSession = (RedisSession *)pTimerData;

	(m_pHandlerObj->*m_fcLockResult)(enmRedisLock_Timeout, this);

	return 0;
}

