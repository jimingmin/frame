/*
 * redis_session.h
 *
 *  Created on: Mar 14, 2015
 *      Author: jimm
 */

#ifndef REDIS_SESSION_H_
#define REDIS_SESSION_H_

#include "../common/common_object.h"
#include "../common/common_datetime.h"
#include "frame_timer.h"
#include "frame.h"
#include <stddef.h>

using namespace FRAME;

typedef int32_t (CBaseObject::*RedisReply)(int32_t nResult, void *pReply, void *pSession);

#define MAX_SESSIONDATA_SIZE		(8 * 1024)

class RedisSession : public CBaseObject
{
public:
	RedisSession()
	{
		m_nSessionIndex = 0;
		m_nTimerIndex = 0;
		m_pHandlerObj = NULL;
		m_pHandleRedisReply = NULL;
	}

	virtual ~RedisSession()
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

	void SetSessionIndex(uint32_t nSessionIndex)
	{
		m_nSessionIndex = nSessionIndex;
	}

	uint32_t GetSessionIndex()
	{
		return m_nSessionIndex;
	}

	void SetTimerIndex(TimerIndex nTimerIndex)
	{
		m_nTimerIndex = nTimerIndex;
	}

	TimerIndex GetTimerIndex()
	{
		return m_nTimerIndex;
	}

	void SetHandlerObj(CBaseObject *pHandlerObj)
	{
		m_pHandlerObj = pHandlerObj;
	}

	CBaseObject *GetHandlerObj()
	{
		return m_pHandlerObj;
	}

	void SetHandleRedisReply(RedisReply HandleRedisReply)
	{
		m_pHandleRedisReply = HandleRedisReply;
	}

	RedisReply GetHandleRedisReply()
	{
		return m_pHandleRedisReply;
	}

	uint8_t *GetSessionData()
	{
		return m_arrSessionData;
	}

	void SetTimerProc(TimerProc Proc, int32_t nCycleTime)
	{
		CTimer *pTimer = g_Frame.GetTimer(m_nTimerIndex);
		if(pTimer != NULL)
		{
			pTimer->SetEndTime(CTimeValue::CurrentTime().Milliseconds() + nCycleTime);
			pTimer->SetEventProc(Proc);
		}
	}

protected:
	uint32_t			m_nSessionIndex;
	TimerIndex			m_nTimerIndex;
	CBaseObject			*m_pHandlerObj;
	RedisReply			m_pHandleRedisReply;
	uint8_t				m_arrSessionData[MAX_SESSIONDATA_SIZE];
};


#endif /* REDIS_SESSION_H_ */
