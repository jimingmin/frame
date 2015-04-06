/*
 * redislock_bank.cpp
 *
 *  Created on: 2015年4月3日
 *      Author: jimm
 */

#include "redislock_bank.h"
#include "../common/common_api.h"
#include "frame.h"
#include "redissession_bank.h"

REGIST_BANK(BANK_REDIS_LOCK, CRedisLockBank)

//初始化
int32_t CRedisLockBank::Init()
{
	m_nLockCounter = Random(100000);

	for(int32_t i = 0; i < MAX_REDISLOCK_COUNT; ++i)
	{
		m_stUnusedLockList.push_back(new RedisLock());
	}
	return 0;
}

//卸载
int32_t CRedisLockBank::Uninit()
{
	m_nLockCounter = 0;

	while(!m_stUnusedLockList.empty())
	{
		RedisLock *pUnusedLock = m_stUnusedLockList.front();
		m_stUnusedLockList.pop_front();
		delete pUnusedLock;
	}

	map<uint32_t, RedisLock *>::iterator it = m_stUsedLockMap.begin();
	for(; it != m_stUsedLockMap.end();)
	{
		delete it->second;
		m_stUsedLockMap.erase(it++);
	}
	return 0;
}

RedisLock *CRedisLockBank::CreateLock(CRedisChannel *pRedisChannel, CBaseObject *pHandlerObj, LockResult Result, int64_t nTimeout/* = 60 * MS_PER_SECOND*/)
{
	if(m_stUnusedLockList.empty())
	{
		return NULL;
	}

	RedisLock *pRedisLock = m_stUnusedLockList.front();
	m_stUnusedLockList.pop_front();

	CRedisSessionBank *pRedisSessionBank = (CRedisSessionBank *)g_Frame.GetBank(BANK_REDIS_SESSION);
	RedisSession *pSession = pRedisSessionBank->CreateSession(pRedisLock, static_cast<RedisReply>(&RedisLock::OnSessionSetNX),
			static_cast<TimerProc>(&CRedisLockBank::OnLockTimeout));

	pRedisLock->SetRedisChannel(pRedisChannel);
	pRedisLock->SetHandlerObj(pHandlerObj);
	pRedisLock->SetLockResultCallback(Result);
	pRedisLock->SetSessionIndex(pSession->GetSessionIndex());

	m_stUsedLockMap[pSession->GetSessionIndex()] = pRedisLock;

	return pRedisLock;
}

RedisLock *CRedisLockBank::GetLock(uint32_t nSessionIndex)
{
	RedisLock *pRedisLock = NULL;
	map<uint32_t, RedisLock *>::iterator it = m_stUsedLockMap.find(nSessionIndex);
	if(it != m_stUsedLockMap.end())
	{
		pRedisLock = it->second;
	}

	return pRedisLock;
}

void CRedisLockBank::DestroyLock(RedisLock *pRedisLock)
{
	pRedisLock->Unlock();

	uint32_t nSessionIndex = pRedisLock->GetSessionIndex();
	map<uint32_t, RedisLock *>::iterator it = m_stUsedLockMap.find(nSessionIndex);
	if(it != m_stUsedLockMap.end())
	{
		m_stUnusedLockList.push_front(it->second);
		m_stUsedLockMap.erase(it);
	}

	CRedisSessionBank *pRedisSessionBank = (CRedisSessionBank *)g_Frame.GetBank(BANK_REDIS_SESSION);
	pRedisSessionBank->DestroySession(nSessionIndex);
}

void CRedisLockBank::DestroyLock(uint32_t nSessionIndex)
{
	RedisLock *pRedisLock = GetLock(nSessionIndex);
	pRedisLock->Unlock();

	map<uint32_t, RedisLock *>::iterator it = m_stUsedLockMap.find(nSessionIndex);
	if(it != m_stUsedLockMap.end())
	{
		m_stUnusedLockList.push_front(it->second);
		m_stUsedLockMap.erase(it);
	}

	CRedisSessionBank *pRedisSessionBank = (CRedisSessionBank *)g_Frame.GetBank(BANK_REDIS_SESSION);
	pRedisSessionBank->DestroySession(nSessionIndex);
}

int32_t CRedisLockBank::OnLockTimeout(void *pTimerData)
{
	RedisSession *pRedisSession = (RedisSession *)pTimerData;
	RedisLock *pRedisLock = GetLock(pRedisSession->GetSessionIndex());
	CBaseObject *pHandler = pRedisLock->GetHandlerObj();
	LockResult Proc = pRedisLock->GetLockResultCallback();

	CRedisSessionBank *pRedisSessionBank = (CRedisSessionBank *)g_Frame.GetBank(BANK_REDIS_SESSION);
	pRedisSessionBank->DestroySession(pRedisSession);

	(pHandler->*Proc)(RedisLock::enmRedisLock_Timeout, pRedisLock);

	return 0;
}

