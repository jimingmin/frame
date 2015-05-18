/*
 * logic_thread.h
 *
 *  Created on: May 3, 2015
 *      Author: jimm
 */

#ifndef LOGIC_THREAD_H_
#define LOGIC_THREAD_H_

#include "common/common_thread.h"
#include "netevent/net_handler.h"
#include "netevent/net_acceptor.h"
#include "frame_impl.h"
#include "frame_listenentry.h"
#include <list>

using namespace std;
using namespace NETEVENT;
using namespace FRAME;

class CLogicThread : public CThread
{
public:
	CLogicThread(CriticalSection *pSection, const char *szServerName, IInitFrame *pIniter, list<CFrameListenEntry *> &stListenEntryList);

	virtual ~CLogicThread();
	//启动线程
	virtual int32_t Start();
	//结束线程
	virtual int32_t	Terminate();
	//线程入口函数
	virtual void Execute();

protected:
	CriticalSection *m_pSection;
	char 			m_szServerName[1024];
	CNetHandler		*m_pNetHandler;
	IInitFrame		*m_pIniter;
	list<CFrameListenEntry *> &m_stListenEntryList;
	list<CAcceptor *>	m_stAcceptorList;
};

#endif /* LOGIC_THREAD_H_ */
