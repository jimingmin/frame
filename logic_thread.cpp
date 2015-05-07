/*
 * logic_thread.cpp
 *
 *  Created on: May 3, 2015
 *      Author: jimm
 */

#include "logic_thread.h"
#include "frame.h"

using namespace FRAME;

CLogicThread::CLogicThread(CriticalSection *pSection, const char *szServerName, IInitFrame *pIniter)
{
	m_pSection = pSection;
	memset(m_szServerName, 0, sizeof(m_szServerName));
	strcpy(m_szServerName, szServerName);

	m_pIniter = pIniter;

	//创建网络事件处理器
	m_pNetHandler = new CNetHandler();
}

CLogicThread::~CLogicThread()
{
	if(m_pNetHandler != NULL)
	{
		delete m_pNetHandler;
		m_pNetHandler = NULL;
	}
}

//启动线程
int32_t CLogicThread::Start()
{
//	while(true)
//	{
//		if(m_pSection->try_enter())
//		{
//			m_pSection->enter();
//			break;
//		}
//		else
//		{
//			Delay(100);
//		}
//	}

	m_pSection->enter();

	m_pNetHandler->CreateReactor();
	g_Frame.AddRunner(m_pNetHandler);

	if(g_Frame.Init(m_szServerName) != 0)
	{
		return 1;
	}

	if(m_pIniter != NULL)
	{
		if(m_pIniter->InitFrame(m_pNetHandler) != 0)
		{
			return 1;
		}
	}

	return CThread::Start();
}

//结束线程
int32_t	CLogicThread::Terminate()
{
	CThread::Terminate();

	if(m_pNetHandler != NULL)
	{
		g_Frame.ClearRunner();
		m_pNetHandler->DestoryReactor();
	}
	return 0;
}

void CLogicThread::Execute()
{
	g_Frame.Run(GetTerminated());
	g_Frame.Uninit();
}

