/*
 * frame_timertask.cpp
 *
 *  Created on: 2014年5月29日
 *      Author: jimm
 */

#include "frame_timertask.h"
#include "logger/logger.h"
#include "common/common_memmgt.h"

//extern char *GetMemInfo();

using namespace LOGGER;

FRAME_NAMESPACE_BEGIN

int32_t CFrameTimerTask::PrintMemInfo(void *pTimerData)
{
	CTimerTaskData *pTimerTaskData = (CTimerTaskData *)pTimerData;
	//WRITE_INFO_LOG(pTimerData->m_strServerName.c_str(), "%s", GetMemInfo());

	return 0;
}

FRAME_NAMESPACE_END

