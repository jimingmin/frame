/*
 * frame_listenentry.h
 *
 *  Created on: May 12, 2015
 *      Author: jimm
 */

#ifndef FRAME_LISTENENTRY_H_
#define FRAME_LISTENENTRY_H_

#include "common/common_object.h"
#include "netevent/net_impl.h"
#include "frame_namespace.h"

using namespace NETEVENT;

FRAME_NAMESPACE_BEGIN

class CFrameListenEntry : public CBaseObject
{
public:
	CFrameListenEntry(const char *szAddr, uint16_t nPort, IPacketParserFactory *pParserFactory, IIOHandler *pIOHandler)
	{
		memset(m_szListenAddr, 0, sizeof(m_szListenAddr));
		if(szAddr != NULL)
		{
			strcpy(m_szListenAddr, szAddr);
		}
		m_nListenPort = nPort;
		m_pParserFactory = pParserFactory;
		m_pIOHandler = pIOHandler;
	}

	virtual ~CFrameListenEntry(){}

	virtual int32_t Init()
	{
		return 0;
	}

	virtual int32_t Uninit()
	{
		return 0;
	}

	virtual int32_t GetSize()
	{
		return 0;
	}

	const char *GetListenAddr()
	{
		return m_szListenAddr;
	}

	uint16_t GetListenPort()
	{
		return m_nListenPort;
	}

	IPacketParserFactory *GetPacketParserFactory()
	{
		return m_pParserFactory;
	}

	IIOHandler *GetIOHandler()
	{
		return m_pIOHandler;
	}

protected:
	char						m_szListenAddr[64];
	uint16_t					m_nListenPort;
	IPacketParserFactory		*m_pParserFactory;
	IIOHandler					*m_pIOHandler;
};

FRAME_NAMESPACE_END

#endif /* FRAME_LISTENENTRY_H_ */
