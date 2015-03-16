/*
 * sessionindex_bank.cpp
 *
 *  Created on: Mar 15, 2015
 *      Author: jimm
 */

#include "sessionindex_bank.h"
#include "frame.h"

using namespace FRAME;

REGIST_BANK(BANK_SESSION_INDEX, CSessionIndexBank)

	//初始化
int32_t CSessionIndexBank::Init()
{
	for(int32_t i = 0; i < MAX_SESSIONINDEX_COUNT; ++i)
	{
		m_stUnusedSessionIndexList.push_back(new uint32_t());
	}
	return 0;
}
	//卸载
int32_t CSessionIndexBank::Uninit()
{
	while(!m_stUnusedSessionIndexList.empty())
	{
		delete m_stUnusedSessionIndexList.front();
		m_stUnusedSessionIndexList.pop_front();
	}

	for(set<uint32_t *>::iterator it = m_stUsedSessionIndexSet.begin(); it != m_stUsedSessionIndexSet.end();)
	{
		delete (*it);
		m_stUsedSessionIndexSet.erase(it++);
	}
	return 0;
}

uint32_t *CSessionIndexBank::CreateSessionIndex()
{
	uint32_t *pSessionIndex = NULL;
	if(!m_stUnusedSessionIndexList.empty())
	{
		pSessionIndex = m_stUnusedSessionIndexList.front();
		m_stUnusedSessionIndexList.pop_front();
	}
	else
	{
		pSessionIndex = new uint32_t();
	}

	m_stUsedSessionIndexSet.insert(pSessionIndex);
	return pSessionIndex;
}

void CSessionIndexBank::DestroySessionIndex(uint32_t *pSessionIndex)
{
	m_stUsedSessionIndexSet.erase(pSessionIndex);
	m_stUnusedSessionIndexList.push_back(pSessionIndex);
}

