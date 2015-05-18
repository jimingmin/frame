/*
 * redissession_bank.cpp
 *
 *  Created on: Mar 15, 2015
 *      Author: jimm
 */

#include "redissession_bank.h"
#include "common/common_api.h"
#include "frame.h"

REGIST_BANK(BANK_REDIS_SESSION, CRedisSessionBank)

//初始化
int32_t CRedisSessionBank::Init()
{
	m_nSessionCounter = Random(100000);

	for(int32_t i = 0; i < MAX_REDISSESSION_COUNT; ++i)
	{
		m_stUnusedSessionList.push_back(new RedisSession());
	}
	return 0;
}

//卸载
int32_t CRedisSessionBank::Uninit()
{
	m_nSessionCounter = 0;

	while(!m_stUnusedSessionList.empty())
	{
		RedisSession *pUnusedSession = m_stUnusedSessionList.front();
		m_stUnusedSessionList.pop_front();
		delete pUnusedSession;
	}

	map<uint32_t, RedisSession *>::iterator it = m_stUsedSessionMap.begin();
	for(; it != m_stUsedSessionMap.end();)
	{
		delete it->second;
		m_stUsedSessionMap.erase(it++);
	}
	return 0;
}

RedisSession *CRedisSessionBank::CreateSession(CBaseObject *pHandlerObj, RedisReply HandleReply, TimerProc HandleTimeout, int64_t nTimeout/* = 3 * MS_PER_SECOND*/)
{
	if(m_stUnusedSessionList.empty())
	{
		return NULL;
	}

	RedisSession *pRedisSession = m_stUnusedSessionList.front();
	m_stUnusedSessionList.pop_front();

	pRedisSession->SetHandlerObj(pHandlerObj);
	pRedisSession->SetHandleRedisReply(HandleReply);
	pRedisSession->SetSessionIndex(m_nSessionCounter++);
	if(m_nSessionCounter == 0)
	{
		m_nSessionCounter = 1;
	}

	if(HandleTimeout != NULL)
	{
		TimerIndex nTimerIndex = 0;
		if(g_Frame.CreateTimer(HandleTimeout, pHandlerObj, pRedisSession, nTimeout, false, nTimerIndex) != 0)
		{
			m_stUnusedSessionList.push_front(pRedisSession);
			return NULL;
		}

		pRedisSession->SetTimerIndex(nTimerIndex);
	}
	else
	{
		pRedisSession->SetTimerIndex(0);
	}

	m_stUsedSessionMap[pRedisSession->GetSessionIndex()] = pRedisSession;

	return pRedisSession;
}

RedisSession *CRedisSessionBank::GetSession(uint32_t nSessionIndex)
{
	RedisSession *pRedisSession = NULL;
	map<uint32_t, RedisSession *>::iterator it = m_stUsedSessionMap.find(nSessionIndex);
	if(it != m_stUsedSessionMap.end())
	{
		pRedisSession = it->second;
	}

	return pRedisSession;
}

void CRedisSessionBank::DestroySession(RedisSession *pRedisSession)
{
	DestroySession(pRedisSession->GetSessionIndex());
}

void CRedisSessionBank::DestroySession(uint32_t nSessionIndex)
{
	map<uint32_t, RedisSession *>::iterator it = m_stUsedSessionMap.find(nSessionIndex);
	if(it != m_stUsedSessionMap.end())
	{
		RedisSession *pRedisSession = it->second;
		TimerIndex nTimerIndex = pRedisSession->GetTimerIndex();
		if(nTimerIndex != 0)
		{
			g_Frame.RemoveTimer(nTimerIndex);
		}

		m_stUnusedSessionList.push_front(pRedisSession);
		m_stUsedSessionMap.erase(it);
	}
}
