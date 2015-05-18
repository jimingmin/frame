/*
 * cmd_handler.h
 *
 *  Created on: Apr 29, 2015
 *      Author: jimm
 */

#ifndef CMD_HANDLER_H_
#define CMD_HANDLER_H_

#include "netevent/net_impl.h"
#include "netevent/net_ioadapter.h"
#include "netevent/net_connector.h"

using namespace NETEVENT;

class CCmdHandler : public CIOHandlerAdapter
{
public:
	CCmdHandler(CriticalSection *pSection, const char *szServiceName, int32_t nServiceType, uint16_t nServiceID);

	virtual int32_t OnOpened(IIOSession *pIoSession);

	virtual int32_t OnRecved(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize);

	virtual int32_t OnSent(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize);

	virtual int32_t OnClosed(IIOSession *pIoSession);

	virtual int32_t OnError(IIOSession *pIoSession);

	virtual int32_t OnTimeout(IIOSession *pIoSession);

	void SetConnector(CConnector *pConnector);

	int32_t LoadConfig(const char *pConfigName, const char *pFileContent);

protected:
	CriticalSection 	*m_pSection;
	char				m_szServiceName[1024];
	int32_t				m_nServiceType;
	uint16_t			m_nServiceID;
	CConnector			*m_pConnector;
	bool				m_bLoadConfigFinish;
};


#endif /* CMD_HANDLER_H_ */
