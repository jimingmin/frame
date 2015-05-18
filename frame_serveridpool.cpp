/*
 * frame_serveridpool.cpp
 *
 *  Created on: May 16, 2015
 *      Author: jimm
 */

#include "frame_serveridpool.h"

int32_t CFrameServerIDPool::Init()
{
	m_nCurIndex = 0;
	m_nCapacity = sizeof(m_arrServerID) / sizeof(uint16_t);
	for(int32_t i = 0; i < m_nCapacity; ++i)
	{
		m_arrServerID[i] = i;
	}

	return 0;
}

int32_t CFrameServerIDPool::Uninit()
{
	return 0;
}

int32_t CFrameServerIDPool::GetSize()
{
	return sizeof(*this);
}

uint16_t CFrameServerIDPool::PopServerID()
{
	if(m_nCurIndex > (m_nCapacity - 1))
	{
		m_nCurIndex = 0;
	}

	return  m_arrServerID[m_nCurIndex++];
}


