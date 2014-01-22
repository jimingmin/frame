/*
 * frame.cpp
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#include "frame_timermgt.h"
#include "frame_configmgt.h"
#include "frame.h"

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

FRAME_NAMESPACE_END
