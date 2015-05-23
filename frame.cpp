/*
 * frame.cpp
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#include "frame.h"
#include "frame_configmgt.h"
#include "logger/logger_writer.h"
#include "logger/logger.h"
#include "common/common_codeengine.h"
#include "cmd_thread.h"
#include "logic_thread.h"

#include <stdarg.h>
#include <stddef.h>
#ifdef unix
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
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
	m_nServerID = 0;
	m_pTimerTask = new CFrameTimerTask();
	m_pConfigMgt = new CFrameConfigMgt();
	m_pTimerMgt = new CTimerMgt();
	m_pBankMgt = new CFrameBankMgt();
	m_nWorkerCount = 4;//sysconf(_SC_NPROCESSORS_CONF) * 2;

	m_stServerIDPool.Init();
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
	while(!bStop)
	{
		bool bHasData = false;
		list<IRunnable *>::iterator it = m_stRunnerList.begin();
		for(; it != m_stRunnerList.end(); ++it)
		{
			if((*it)->Run() > 0)
			{
				bHasData = true;
			}
		}

		if(!bHasData)
		{
			Delay(10 * US_PER_MS);
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

void CFrame::SetWorkerCount(int32_t nWorkerCount)
{
	if((nWorkerCount <= (5 * m_nWorkerCount)) && (nWorkerCount >= 0))
	{
		m_nWorkerCount = nWorkerCount;
	}
}

int32_t CFrame::Start(const char *szServerName, int32_t nServiceType, char *szHost, uint16_t nPort, IInitFrame *pIniter)
{
	list<SocketFD> stListenHandle = StartWatcherListening(m_stListenEntryList);

//	MakeWorker(m_nWorkerCount);
	if(m_nWorkerCount <= 0)
	{
		m_nServerID = m_stServerIDPool.PopServerID();
	}
	else
	{
		m_nServerID = MakeWorker(m_nWorkerCount);
	}

	CLogger::Start();

	CCmdThread *pCmdThread = new CCmdThread(&m_stLoadConfigLock, szServerName, nServiceType, m_nServerID, szHost, nPort);
	pCmdThread->Start();

	Delay(3 * US_PER_SECOND);

	CNetHandler *pNetHandler = new CNetHandler();
	pNetHandler->CreateReactor();

	list<CAcceptor *> stAcceptorList = StartWorkerListening(stListenHandle, m_stListenEntryList, pNetHandler);

	Working(szServerName, pIniter, pNetHandler);

	StopWorkerListening(stAcceptorList);

	pCmdThread->Terminate();

	Delay(1 * US_PER_SECOND);

	delete pCmdThread;

	StopWatcherListening(stListenHandle);

	for(list<CFrameListenEntry *>::iterator it = m_stListenEntryList.begin(); it != m_stListenEntryList.end(); ++it)
	{
		delete (*it);
	}

	m_stListenEntryList.clear();

	return Uninit();
//	CLogicThread *pLogicThread = new CLogicThread(&m_stLoadConfigLock, szServerName, pIniter, m_stListenEntryList);
//	return pLogicThread->Start();
}

list<SocketFD> CFrame::StartWatcherListening(list<CFrameListenEntry *> stListenEntryList)
{
	list<SocketFD> stListenHandle;
	for(list<CFrameListenEntry *>::iterator it = stListenEntryList.begin(); it != stListenEntryList.end(); ++it)
	{
		int32_t nSocketFD = socket(AF_INET, SOCK_STREAM, 0);

		set_non_block(nSocketFD);
		set_reuse_addr(nSocketFD);

		//填充服务器地址&端口信息
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		if((NULL == (*it)->GetListenAddr()) || (strlen((*it)->GetListenAddr()) == 0))
		{
			addr.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else
		{
			addr.sin_addr.s_addr = inet_addr((*it)->GetListenAddr());
		}
		addr.sin_port = htons((*it)->GetListenPort());

	    int32_t ret = ::bind(nSocketFD, (struct sockaddr *)&addr, sizeof(addr));
		if (0 != ret)
		{
			break;
		}

		//开始监听
		ret = listen(nSocketFD, SOMAXCONN);
		if (0 != ret)
		{
			break;
		}

		stListenHandle.push_back(nSocketFD);
	}

	return stListenHandle;
}

void CFrame::StopWatcherListening(list<SocketFD> stHandleList)
{
	for(list<SocketFD>::iterator it = stHandleList.begin(); it != stHandleList.end(); ++it)
	{
		close((*it));
	}
}

list<CAcceptor *> CFrame::StartWorkerListening(list<SocketFD> stListenHandle, list<CFrameListenEntry *> stListenEntryList, CNetHandler *pNetHandler)
{
	list<CAcceptor *> stAcceptorList;
	for(list<SocketFD>::iterator it = stListenHandle.begin(); it != stListenHandle.end(); ++it)
	{
		CFrameListenEntry *pListenEntry = stListenEntryList.front();
		stListenEntryList.pop_front();

		CAcceptor *pAcceptor = new CAcceptor(pNetHandler, pListenEntry->GetPacketParserFactory(), pListenEntry->GetIOHandler());
		pAcceptor->SetSocketFD((*it));
		pAcceptor->SetLocalAddress(inet_addr(pListenEntry->GetListenAddr()));
		pAcceptor->SetLocalPort(pListenEntry->GetListenPort());
		pAcceptor->SetSessionType(enmSessionType_Listen);
		pAcceptor->SetSessionStatus(enmSessionStatus_Opened);
		pNetHandler->RegistEvent(pAcceptor, mask_read);

		stAcceptorList.push_back(pAcceptor);
	}

	return stAcceptorList;
}

void CFrame::StopWorkerListening(list<CAcceptor *> stAcceptorList)
{
	for(list<CAcceptor *>::iterator it = stAcceptorList.begin(); it != stAcceptorList.end(); ++it)
	{
		delete (*it);
	}
}

uint16_t CFrame::MakeWorker(int32_t nWorkerCount)
{
	uint16_t nServerID = 0;
	int32_t nChild = 0;
	while(!nChild)
	{
		if(nWorkerCount > 0)
		{
			nServerID = m_stServerIDPool.PopServerID();
			switch(fork())
			{
			case -1:
				return -1;
			case 0:
				nServerID = m_stServerIDPool.PopServerID();
				nChild = 1;
				break;
			default:
				--nWorkerCount;
				break;
			}
		}
		else
		{
			int32_t nStatus = 0;
			if(wait(&nStatus) != -1)
			{
				nServerID = m_stServerIDPool.PopServerID();
				++nWorkerCount;
			}
			else
			{
				WRITE_INFO_LOG(m_strServerName.c_str(), "recv signal %d\n", errno);
			}
		}
	}

	return nServerID;
}

int32_t CFrame::Working(const char *szServerName, IInitFrame *pIniter, CNetHandler *pNetHandler)
{
	m_stLoadConfigLock.enter();

	do
	{
		AddRunner(pNetHandler);

		if(Init(szServerName) != 0)
		{
			break;
		}

		if(pIniter != NULL)
		{
			if(pIniter->InitFrame(pNetHandler) != 0)
			{
				break;
			}
		}

		Run();

	}while(false);

	pNetHandler->DestoryReactor();
	delete pNetHandler;
	m_stLoadConfigLock.leave();

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

void CFrame::AddListenHandler(const char *szAddr, uint16_t nPort, IPacketParserFactory *pFactory, IIOHandler *pIOHandler)
{
	m_stListenEntryList.push_back(new CFrameListenEntry(szAddr, nPort, pFactory, pIOHandler));
}

int32_t CFrame::GetServerID()
{
	return m_nServerID;
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
