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
#include "../common/common_codeengine.h"
#include "cmd_thread.h"
#include "logic_thread.h"

#include <stdarg.h>
#include <stddef.h>
#ifdef unix
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif
using namespace LOGGER;
using namespace NETEVENT;

FRAME_NAMESPACE_BEGIN

CFrame &GetFrameInstance()
{
	return g_Frame;
}

CFrame::CFrame()
{
	m_pTimerTask = new CFrameTimerTask();
	m_pConfigMgt = new CFrameConfigMgt();
	m_pTimerMgt = new CTimerMgt();
	m_pBankMgt = new CFrameBankMgt();
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

	nRet = m_pBankMgt->Init();
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
	//
	m_pBankMgt->Uninit();
	//定时器卸载
	m_pTimerMgt->Uninit();

	delete m_pConfigMgt;
	delete m_pTimerMgt;
	delete m_pBankMgt;
	delete m_pTimerTask;
	return 0;
}

void CFrame::AddRunner(IRunnable *pRunner)
{
	m_stRunnerList.push_back(pRunner);
}

void CFrame::DelRunner(IRunnable *pRunner)
{
	m_stRunnerList.remove(pRunner);
}

void CFrame::ClearRunner()
{
	m_stRunnerList.clear();
}

int32_t CFrame::Run(bool bStop/* = false*/)
{
	while(bStop)
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

CTimer *CFrame::GetTimer(TimerIndex nTimerIndex)
{
	CTimer *pTimer = NULL;
	m_pTimerMgt->GetTimer(nTimerIndex, pTimer);
	return pTimer;
}

int32_t CFrame::RemoveTimer(TimerIndex timerIndex)
{
	return m_pTimerMgt->RemoveTimer(timerIndex);
}

void CFrame::RegistConfig(const char *szConfigName, IConfig *pConfig)
{
	m_pConfigMgt->RegistConfig(szConfigName, pConfig);
}

IConfig *CFrame::GetConfig(const char *szConfigName)
{
	return m_pConfigMgt->GetConfig(szConfigName);
}

IConfig *CFrame::GetSafeConfig(const char *szConfigName)
{
	return m_pConfigMgt->GetSafeConfig(szConfigName);
}

void CFrame::RegistBank(const char *szBankName, IBank *pBank)
{
	m_pBankMgt->RegistBank(szBankName, pBank);
}

IBank *CFrame::GetBank(const char *szBankName)
{
	return m_pBankMgt->GetBank(szBankName);
}

int32_t CFrame::StartCmdThread(const char *szServerName, int32_t nServiceType, uint16_t nServiceID, char *szHost, uint16_t nPort)
{
	CCmdThread *pCmdThread = new CCmdThread(&m_stLoadConfigLock, szServerName, nServiceType, nServiceID, szHost, nPort);
	return pCmdThread->Start();
}

int32_t CFrame::StartLogicThread(const char *szServerName)
{
	CLogicThread *pLogicThread = new CLogicThread(&m_stLoadConfigLock, szServerName, NULL);
	pLogicThread->Start();

	return 0;
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

void CFrame::Dump(ICtlHead *pCtlHead, IMsgHead *pMsgHead, IMsgBody *pMsgBody, const char *szPrefix)
{
	uint32_t nOffset = 0;
	char arrLog[enmMaxLogStringLength];

	nOffset = sprintf(arrLog, "%s", szPrefix);

	if(pCtlHead != NULL)
	{
		pCtlHead->Dump(arrLog, sizeof(arrLog), nOffset);
	}

	if(pMsgHead != NULL)
	{
		pMsgHead->Dump(arrLog, sizeof(arrLog), nOffset);
	}

	if(pMsgBody != NULL)
	{
		pMsgBody->Dump(arrLog, sizeof(arrLog), nOffset);
	}

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

int32_t CFrame::SendMsg(IIOSession *pIoSession, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
	uint32_t nOffset = 0;
	uint8_t arrMsgBuf[64 * 1024];
	int32_t nRet = pMsgHead->Encode(arrMsgBuf, sizeof(arrMsgBuf), nOffset);
	if(nRet != 0)
	{
		return 0;
	}

	nRet = pMsgBody->Encode(arrMsgBuf, sizeof(arrMsgBuf), nOffset);
	if(nRet != 0)
	{
		return 0;
	}

	uint16_t nTotalSize = (uint16_t)nOffset;
	nOffset = 0;
	nRet = CCodeEngine::Encode(arrMsgBuf, sizeof(arrMsgBuf), nOffset, nTotalSize);
	if(nRet != 0)
	{
		return 0;
	}

	return pIoSession->Write(arrMsgBuf, nTotalSize);
}

FRAME_NAMESPACE_END
