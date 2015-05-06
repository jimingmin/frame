/*
 * logic_thread.h
 *
 *  Created on: May 3, 2015
 *      Author: jimm
 */

#ifndef LOGIC_THREAD_H_
#define LOGIC_THREAD_H_

#include "../common/common_thread.h"
#include "../netevent/net_handler.h"

using namespace NETEVENT;

typedef int32_t (*InitProc)(CNetHandler *pNetHandler);

class CLogicThread : public CThread
{
public:
	CLogicThread(CriticalSection *pSection, const char *szServerName, InitProc Proc);

	virtual ~CLogicThread();
	//启动线程
	virtual int32_t	Start();
	//结束线程
	virtual int32_t	Terminate();
	//线程入口函数
	virtual void Execute();

protected:
	CriticalSection *m_pSection;
	char 			m_szServerName[1024];
	InitProc 		m_pInitProc;
	CNetHandler		*m_pNetHandler;
};

#endif /* LOGIC_THREAD_H_ */
