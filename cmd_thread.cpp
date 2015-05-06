/*
 * cmd_thread.cpp
 *
 *  Created on: May 3, 2015
 *      Author: jimm
 */

#include "cmd_thread.h"
#include "cmdparser_factory.h"
#include "cmd_handler.h"
#include "../netevent/net_handler.h"
using namespace NETEVENT;

CCmdThread::CCmdThread(CriticalSection *pSection, const char *szServiceName, int32_t nServiceType, uint16_t nServiceID, const char *szHost, uint16_t nPort)
{
	m_pSection = pSection;
	m_pSection->enter();

	memset(m_szServiceName, 0, sizeof(m_szServiceName));
	strcpy(m_szServiceName, szServiceName);
	memset(m_szHost, 0, sizeof(m_szHost));
	strcpy(m_szHost, szHost);
	m_nPort = nPort;
	m_nServiceType = nServiceType;
	m_nServiceID = nServiceID;
}

CCmdThread::~CCmdThread()
{

}

void CCmdThread::Execute()
{
	//创建包解析器工厂
	IPacketParserFactory *pParserFactory = new CCmdParserFactory();

	CCmdHandler *pCmdHandler = new CCmdHandler(m_pSection, m_szServiceName, m_nServiceType, m_nServiceID);

	//创建网络事件处理器
	CNetHandler *pNetHandler = new CNetHandler();
	pNetHandler->CreateReactor();

	//创建连接器
	CConnector *pConnector = new CConnector(pNetHandler, pParserFactory, pCmdHandler);
	pCmdHandler->SetConnector(pConnector);

	pConnector->Connect(m_szHost, m_nPort);

	while(!GetTerminated())
	{
		int32_t nHasData = pNetHandler->Run();
		if(nHasData == 0)
		{
			Delay(100);
		}
	}

	pNetHandler->DestoryReactor();

	delete pConnector;
	delete pNetHandler;
	delete pCmdHandler;
	delete pParserFactory;
}

