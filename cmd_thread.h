/*
 * cmd_thread.h
 *
 *  Created on: May 3, 2015
 *      Author: jimm
 */

#ifndef CMD_THREAD_H_
#define CMD_THREAD_H_

#include "../common/common_thread.h"
#include "../common/common_mutex.h"

class CCmdThread : public CThread
{
public:
	CCmdThread(CriticalSection *pSection, const char *szServiceName, int32_t nServiceType, uint16_t nServerID, const char *szHost, uint16_t nPort);

	virtual ~CCmdThread();
	//线程入口函数
	virtual void Execute();

protected:
	CriticalSection	*m_pSection;
	char			m_szServiceName[1024];
	int32_t			m_nServiceType;
	uint16_t		m_nServiceID;
	char			m_szHost[1024];
	uint16_t		m_nPort;
};


#endif /* CMD_THREAD_H_ */
