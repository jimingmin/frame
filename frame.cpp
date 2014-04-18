/*
 * frame.cpp
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#include "frame.h"
#include "frame_configmgt.h"

#include <stdarg.h>
#include <stddef.h>

FRAME_NAMESPACE_BEGIN

CFrame::CFrame()
{

}

CFrame::~CFrame()
{

}

int32_t CFrame::Init()
{
	int32_t nRet = 0;
	//配置初始化
	nRet = g_FrameConfigMgt.Init();
	if(nRet != 0)
	{
		return nRet;
	}

	//定时器初始化
	nRet = g_FrameTimerMgt.Init();
	if(nRet != 0)
	{
		return nRet;
	}

	AddRunner(&g_FrameTimerMgt);

	return 0;
}

int32_t CFrame::Uninit()
{
	//配置卸载
	g_FrameConfigMgt.Uninit();
	//定时器卸载
	g_FrameTimerMgt.Uninit();
	return 0;
}

void CFrame::AddRunner(IRunnable *pRunner)
{
	m_stRunnerList.push_back(pRunner);
}

int32_t CFrame::Run()
{
	list<IRunnable *>::iterator it = m_stRunnerList.begin();
	for(; it != m_stRunnerList.end(); ++it)
	{
		(*it)->Run();
	}

	return 0;
}

int32_t CFrame::CreateTimer(TimerProc Proc, CObject *pTimer, CObject *pTimerData, int64_t nCycleTime, bool bLoop, TimerIndex& timerIndex)
{
	return g_FrameTimerMgt.CreateTimer(Proc, pTimer, pTimerData, nCycleTime, bLoop, timerIndex);
}

int32_t CFrame::RemoveTimer(TimerIndex timerIndex)
{
	return g_FrameTimerMgt.RemoveTimer(timerIndex);
}

void CFrame::RegistConfig(const char *szConfigName, IConfig *pConfig)
{
	return g_FrameConfigMgt.RegistConfig(szConfigName, pConfig);
}

IConfig *CFrame::GetConfig(const char *szConfigName)
{
	return g_FrameConfigMgt.GetConfig(szConfigName);
}

int32_t CFrame::FrameCallBack(int32_t nMsgID, ...)
{
	va_list ap;
	va_start(ap, nMsgID);
	int32_t nRet = FrameMsgCallBack(nMsgID, ap);
	va_end(ap);
	return nRet;
}

regist::regist(const char *szConfigName, IConfig *pConfig)
{
	g_FrameConfigMgt.RegistConfig(szConfigName, pConfig);
}

FRAME_NAMESPACE_END
