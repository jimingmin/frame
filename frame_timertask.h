/*
 * frame_timertask.h
 *
 *  Created on: 2014年5月29日
 *      Author: jimm
 */

#ifndef FRAME_TIMERTASK_H_
#define FRAME_TIMERTASK_H_

#include "common/common_object.h"
#include "frame_namespace.h"
#include <string>
using namespace std;

FRAME_NAMESPACE_BEGIN

class CFrameTimerTask : public CBaseObject
{
public:

	class CTimerTaskData : public CBaseObject
	{
	public:
		CTimerTaskData(string strServerName)
		{
			m_strServerName = strServerName;
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
			return sizeof(*this);
		}

		string		m_strServerName;
	};

	CFrameTimerTask()
	{
		Init();
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
		return sizeof(*this);
	}

	int32_t PrintMemInfo(void *pTimerData);
};

FRAME_NAMESPACE_END

#endif /* FRAME_TIMERTASK_H_ */
