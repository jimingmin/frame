/*
 * frame_timer.h
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#ifndef FRAME_TIMER_H_
#define FRAME_TIMER_H_

#include "../common/common_typedef.h"
#include "../common/common_errordef.h"
#include "../common/common_object.h"
#include "../common/common_complextype.h"
#include "../common/common_export.h"

#include "frame_namespace.h"

#include <stddef.h>

FRAME_NAMESPACE_BEGIN

class CTimer;

typedef int32_t					TimerIndex;
typedef TimerIndex				TimerID;

//定时器回调例程
typedef int32_t (CObject::*TimerProc)(CTimer *pTimer);

class EXPORT Timer
{
public:
	Timer()
	{

		pData = NULL;
		nTimerSeq = 0;
		pTimerHandler = NULL;
		pTimerProc = NULL;
		nStartTime = 0;
		nCycleTime = 0;
		nEndTime = 0;
		bLoop = false;
		nFiredCount = 0;
		attachedDataA = 0;
		attachedDataB = 0;
	}

	void					*pData;
	uint32_t				nTimerSeq;
	CObject					*pTimerHandler;
	TimerProc				pTimerProc;
	int64_t					nStartTime;
	int64_t					nCycleTime;
	int64_t					nEndTime;
	bool					bLoop;
	uint32_t				nFiredCount;
	uint32_t                attachedDataA;
	uint32_t                attachedDataB;
};

class EXPORT CTimer : public CObject
{
public:
	CTimer()
	{
	}
	virtual ~CTimer()
	{
	}

public:

	virtual int32_t Init()
	{
		return S_OK;
	}

	virtual int32_t Uninit()
	{
		memset(&m_stTimer, 0, sizeof(m_stTimer));
		return S_OK;
	}

	virtual int32_t GetSize()
	{
		return sizeof(*this);
	}

public:
	void SetTimer(const Timer& stTimer)
	{
		memcpy(&m_stTimer, &stTimer, sizeof(m_stTimer));
	}

	void GetTimer(Timer& stTimer)
	{
		memcpy(&stTimer, &m_stTimer, sizeof(m_stTimer));
	}

	void *GetTimerData()
	{
		return m_stTimer.pData;
	}

	int64_t GetEndTime()
	{
		return m_stTimer.nEndTime;
	}

	uint32_t GetTimerSeq()
	{
		return m_stTimer.nTimerSeq;
	}

	bool IsLoop()
	{
		return m_stTimer.bLoop;
	}

	uint32_t GetFiredCount()
	{
		return m_stTimer.nFiredCount;
	}

	TimerProc GetEventProc()
	{
		return m_stTimer.pTimerProc;
	}

	CObject* GetTimerHandler()
	{
		return m_stTimer.pTimerHandler;
	}

protected:
	Timer					m_stTimer;
};

FRAME_NAMESPACE_END

CCOMPLEXTYPE_SPECIFICALLY(FRAME::CTimer)


#endif /* FRAME_TIMER_H_ */

