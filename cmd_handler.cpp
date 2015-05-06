/*
 * cmd_handler.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: jimm
 */

#include "cmd_handler.h"
#include "../common/common_codeengine.h"
#include "../common/common_api.h"
#include "../logger/logger.h"
#include "frame_msghandle.h"
#include "frame_define.h"
#include "frame.h"
#include "cmd_define.h"

#include <string.h>

using namespace LOGGER;
using namespace FRAME;

CCmdHandler::CCmdHandler(CriticalSection *pSection, const char *szServiceName, int32_t nServiceType, uint16_t nServiceID)
{
	m_pSection = pSection;
	memset(m_szServiceName, 0, sizeof(m_szServiceName));
	strcpy(m_szServiceName, szServiceName);
	m_nServiceType = nServiceType;
	m_nServiceID = nServiceID;
	m_pConnector = NULL;
}

int32_t CCmdHandler::OnOpened(IIOSession *pIoSession)
{
	WRITE_DEBUG_LOG(MODULE_NAME, "new session!{peeraddress=%s, peerport=%d}\n",
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());

	CmdRegistReq stCmdRegist(m_szServiceName, m_nServiceType, m_nServiceID);
	string cmd = stCmdRegist.Encode();

	uint8_t arrBuf[1024];
	uint32_t nOffset = 0;
	CCodeEngine::Encode(arrBuf, sizeof(arrBuf), nOffset, cmd.size());
	strncpy((char *)arrBuf + nOffset, cmd.c_str(), cmd.size());

	pIoSession->Write(arrBuf, cmd.size() + nOffset);

	return 0;
}

int32_t CCmdHandler::OnRecved(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize)
{
	WRITE_DEBUG_LOG(MODULE_NAME, "recv message : [size=%d]:%s\n", nBufSize, (char *)pBuf);

	Document document;
	if(document.Parse<0>((char *)pBuf).HasParseError())
	{
		return 0;
	}

	if(!document.IsObject())
	{
		return 0;
	}

	Value &stCmd = document["cmd"];
	Value &stParams = document["params"];

	const char *szCmd = stCmd.GetString();

	if(strcmp(stCmd.GetString(), "load config") == 0)
	{
		if(LoadConfig(stParams["config"].GetString(), stParams["content"].GetString()))
		{

		}
	}
	else if(strcmp(stCmd.GetString(), "load finish") == 0)
	{
		m_pSection->leave();
	}

	return 0;
}

int32_t CCmdHandler::OnSent(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize)
{
	WRITE_DEBUG_LOG(MODULE_NAME, "sent message : [size=%d]:%s\n", nBufSize, (char *)pBuf);

	return 0;
}

int32_t CCmdHandler::OnClosed(IIOSession *pIoSession)
{
	WRITE_DEBUG_LOG(MODULE_NAME, "session closed!{peeraddress=%s, peerport=%d}\n",
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());

	Delay(3 * US_PER_SECOND);
	m_pConnector->Connect(pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
	return 0;
}

int32_t CCmdHandler::OnError(IIOSession *pIoSession)
{
	WRITE_DEBUG_LOG(MODULE_NAME, "session error!{peeraddress=%s, peerport=%d}\n",
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());

	Delay(3 * US_PER_SECOND);
	m_pConnector->Connect(pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
	return 0;
}

int32_t CCmdHandler::OnTimeout(IIOSession *pIoSession)
{
	WRITE_DEBUG_LOG(MODULE_NAME, "session timeout!{peeraddress=%s, peerport=%d}\n",
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());

	m_pConnector->Connect(pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
	return 0;
}

void CCmdHandler::SetConnector(CConnector *pConnector)
{
	m_pConnector = pConnector;
}


int32_t CCmdHandler::LoadConfig(const char *pConfigName, const char *pFileContent)
{
	IConfig *pConfig = g_Frame.GetSafeConfig(pConfigName);
	if(pConfig != NULL)
	{
		pConfig->Parser((char *)pFileContent);
	}
	return 0;
}

