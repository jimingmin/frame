/*
 * frame.cpp
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#include "frame.h"
#include "frame_configmgt.h"
#include "../logger/logger_writer.h"
#include "../logger/logger.h"

#include <stdarg.h>
#include <stddef.h>
#ifdef unix
#include <unistd.h>
#endif
using namespace LOGGER;

FRAME_NAMESPACE_BEGIN

CFrame::CFrame()
{
	m_pTimerTask = new CFrameTimerTask();
	m_pConfigMgt = new CFrameConfigMgt();
	m_pTimerMgt = new CTimerMgt();
}

CFrame::~CFrame()
{

}

int32_t CFrame::Init(const char *szServerName, bool bDaemon/* = false*/)
{
	int32_t nRet = 0;

	m_strServerName = szServerName;

	//配置初始化
	nRet = m_pConfigMgt->Init();
	if(nRet != 0)
	{
		return nRet;
	}

	//定时器初始化
	nRet = m_pTimerMgt->Init();
	if(nRet != 0)
	{
		return nRet;
	}

	TimerIndex nTimerIndex = -1;
	CFrameTimerTask::CTimerTaskData *pTimerData = new CFrameTimerTask::CTimerTaskData(m_strServerName);
	nRet = m_pTimerMgt->CreateTimer(static_cast<TimerProc>(&CFrameTimerTask::PrintMemInfo), m_pTimerTask, pTimerData, 60 * MS_PER_SECOND, true, nTimerIndex);
	if(nRet != 0)
	{
		return nRet;
	}

	AddRunner(m_pTimerMgt);

	if(bDaemon)
	{
		SetDaemon();
	}

	InitSig();

	return 0;
}

int32_t CFrame::Uninit()
{
	//配置卸载
	m_pConfigMgt->Uninit();
	//定时器卸载
	m_pTimerMgt->Uninit();

	delete m_pTimerTask;
	delete m_pConfigMgt;
	delete m_pTimerMgt;
	return 0;
}

void CFrame::AddRunner(IRunnable *pRunner)
{
	m_stRunnerList.push_back(pRunner);
}

int32_t CFrame::Run()
{
	while(true)
	{
		list<IRunnable *>::iterator it = m_stRunnerList.begin();
		for(; it != m_stRunnerList.end(); ++it)
		{
			(*it)->Run();
		}
	}

	return 0;
}

int32_t CFrame::CreateTimer(TimerProc Proc, CBaseObject *pTimer, CBaseObject *pTimerData, int64_t nCycleTime, bool bLoop, TimerIndex& timerIndex)
{
	return m_pTimerMgt->CreateTimer(Proc, pTimer, pTimerData, nCycleTime, bLoop, timerIndex);
}

int32_t CFrame::RemoveTimer(TimerIndex timerIndex)
{
	return m_pTimerMgt->RemoveTimer(timerIndex);
}

void CFrame::RegistConfig(const char *szConfigName, IConfig *pConfig)
{
	return m_pConfigMgt->RegistConfig(szConfigName, pConfig);
}

IConfig *CFrame::GetConfig(const char *szConfigName)
{
	return m_pConfigMgt->GetConfig(szConfigName);
}

int32_t CFrame::FrameCallBack(int32_t nMsgID, ...)
{
	va_list ap;
	va_start(ap, nMsgID);
	int32_t nRet = FrameMsgCallBack(this, m_stMsgMap, nMsgID, ap);
	va_end(ap);
	return nRet;
}

CMsgMapDecl &CFrame::GetMsgMap()
{
	return m_stMsgMap;
}

void CFrame::Dump(IMsgHead *pMsgHead, IMsgBody *pMsgBody, const char *szPrefix)
{
	uint32_t nOffset = 0;
	char arrLog[enmMaxLogStringLength];

	nOffset = sprintf(arrLog, "%s", szPrefix);

	pMsgHead->Dump(arrLog, sizeof(arrLog) - nOffset, nOffset);
	pMsgBody->Dump(arrLog, sizeof(arrLog) - nOffset, nOffset);

	int32_t nIndex = nOffset > enmMaxLogStringLength - 1 ? enmMaxLogStringLength - 1 : nOffset;
	arrLog[nIndex] = '\0';

	WRITE_INFO_LOG(m_strServerName.c_str(), "%s\n", arrLog);
}

//regist::regist(const char *szConfigName, IConfig *pConfig)
//{
//	g_Frame.RegistConfig(szConfigName, pConfig);
//}

int32_t CFrame::SetDaemon()
{
#ifdef unix
	pid_t pid = 0;

	if ((pid = fork()) != 0)
	{
		exit(0);
	}

	setsid();

	if ((pid = fork()) != 0)
	{
		exit(0);
	}

	umask(0);
#endif
	return 0;
}

int32_t CFrame::InitSig()
{
#ifdef unix
	signal(SIGINT,  SIG_IGN);
	signal(SIGHUP,  SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGHUP,  SIG_IGN);

	struct sigaction sig;
	sig.sa_handler = SIG_IGN;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	sigaction(SIGPIPE, &sig, NULL);
#endif
	return 0;
}

FRAME_NAMESPACE_END
