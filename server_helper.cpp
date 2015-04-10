/*
 * server_helper.cpp
 *
 *  Created on: Mar 13, 2015
 *      Author: jimm
 */

#include "server_helper.h"
#include "../common/common_crypt.h"
#include "../common/common_datetime.h"
#include "../include/typedef.h"
#include "frame.h"

using namespace FRAME;

static char g_arrSSKey[16] = {'v', 'd', 'c', '$', 'a', 'u', 't', 'h', '@', '1', '7','9','.', 'c', 'o', 'm'};

bool CServerHelper::IsExist(Document &document, const char *szKey)
{
	return document.HasMember(szKey);
}

bool CServerHelper::IsExist(Value &value, const char *szKey)
{
	return value.HasMember(szKey);
}

bool CServerHelper::GetInt(Document &document, const char *szKey, int32_t &nValue)
{
	if(!document.HasMember(szKey))
	{
		return false;
	}

	nValue = 0;

	if(!document[szKey].IsInt())
	{
		if(!document[szKey].IsString())
		{
			return false;
		}
		nValue = atoi(document[szKey].GetString());
	}
	else
	{
		nValue = document[szKey].GetInt();
	}

	return true;
}

bool CServerHelper::GetUint(Document &document, const char *szKey, uint32_t &nValue)
{
	if(!document.HasMember(szKey))
	{
		return false;
	}

	nValue = 0;

	if(!document[szKey].IsUint())
	{
		if(!document[szKey].IsString())
		{
			return false;
		}
		nValue = atoi(document[szKey].GetString());
	}
	else
	{
		nValue = document[szKey].GetUint();
	}

	return true;
}

bool CServerHelper::GetString(Document &document, const char *szKey, string &nValue)
{
	if(!document.HasMember(szKey))
	{
		return false;
	}

	nValue = "";
	if(!document[szKey].IsString())
	{
		return false;
	}

	nValue = document[szKey].GetString();
	return true;
}


bool CServerHelper::GetInt(Value &value, const char *szKey, int32_t &nValue)
{
	if(!value.HasMember(szKey))
	{
		return false;
	}

	nValue = 0;

	if(!value[szKey].IsInt())
	{
		if(!value[szKey].IsString())
		{
			return false;
		}
		nValue = atoi(value[szKey].GetString());
	}
	else
	{
		nValue = value[szKey].GetInt();
	}

	return true;
}

bool CServerHelper::GetString(Value &value, const char *szKey, string &nValue)
{
	if(!value.HasMember(szKey))
	{
		return false;
	}

	nValue = "";
	if(!value[szKey].IsString())
	{
		return false;
	}

	nValue = value[szKey].GetString();
	return true;
}

bool CServerHelper::GetValue(Document &document, const char *szKey, Value &nValue)
{
	if(!document.HasMember(szKey))
	{
		return false;
	}

	nValue = "";

	if(!document[szKey].IsObject())
	{
		return false;
	}

	nValue = document[szKey];

	return true;
}

bool CServerHelper::GetValue(Value &value, const char *szKey, Value &nValue)
{
	if(!value.HasMember(szKey))
	{
		return false;
	}

	nValue = "";

	if(!value[szKey].IsObject())
	{
		return false;
	}

	nValue = value[szKey];

	return true;
}

void CServerHelper::FillControlHead(ControlHead &stCtlHead, uint16_t nTotalSize, ControlCode nCtlCode, uint32_t nUin, SessionID nSessionID,
			uint32_t nClientAddress, uint16_t nClientPort, int32_t nGateID)
{
	stCtlHead.m_nTotalSize = nTotalSize;
	stCtlHead.m_nControlCode = nCtlCode;
	stCtlHead.m_nUin = nUin;
	stCtlHead.m_nSessionID = nSessionID;
	stCtlHead.m_nClientAddress = nClientAddress;
	stCtlHead.m_nClientPort = nClientPort;
	stCtlHead.m_nGateID = nGateID;
	stCtlHead.m_nTimeStamp = CTimeValue::CurrentTime().Milliseconds();
}

int32_t CServerHelper::MakeMsg(ICtlHead *pCtlHead, IMsgHead *pMsgHead, IMsgBody *pMsgBody, uint8_t *pOutBuf, int32_t nOutBufSize)
{
//	ControlHead *pControlHead = (ControlHead *)pCtlHead;
	MsgHeadCS *pMsgHeadCS = (MsgHeadCS *)pMsgHead;

	uint32_t nOffset = 0;
	int32_t nRet = pCtlHead->Encode(pOutBuf, nOutBufSize, nOffset);
	if(nRet != 0)
	{
		return 0;
	}

	nRet = pMsgHead->Encode(pOutBuf, nOutBufSize, nOffset);
	if(nRet != 0)
	{
		return 0;
	}

	nRet = pMsgBody->Encode(pOutBuf, nOutBufSize, nOffset);
	if(nRet != 0)
	{
		return 0;
	}

	uint16_t nCtlHeadSize = pCtlHead->GetSize();

	//msghead + msgbody的大小
	uint16_t nMsgTotalSize = nOffset - nCtlHeadSize;
	pMsgHeadCS->m_nTotalSize = nMsgTotalSize;
	nOffset = nCtlHeadSize;
	CCodeEngine::Encode(pOutBuf, nOutBufSize, nOffset, nMsgTotalSize);

	nOffset = 0;
	//ctlhead + msghead + msgbody的大小
	uint16_t nCtlTotalSize = nMsgTotalSize + nCtlHeadSize;
	CCodeEngine::Encode(pOutBuf, nOutBufSize, nOffset, nCtlTotalSize);

	return nCtlTotalSize;
}

int32_t CServerHelper::SendMsgToClient(IIOSession *pIoSession, MsgHeadCS *pMsgHeadCS, uint8_t *pBuf, int32_t nBufSize)
{
	uint8_t arrMsg[MAX_MSG_SIZE];

	uint16_t nHeadSize = pMsgHeadCS->GetSize();
	uint8_t *pBody = pBuf + nHeadSize;
	int32_t nBodySize = CXTEA::Encrypt((char *)pBody, nBufSize - pMsgHeadCS->GetSize(), (char *)&arrMsg[nHeadSize],
			sizeof(arrMsg) - nHeadSize, g_arrSSKey);

	memcpy(arrMsg, pBuf, nHeadSize);

	uint16_t nTotalSize = (uint16_t)(nBodySize + pMsgHeadCS->GetSize());
	uint32_t nOffset = 0;
	CCodeEngine::Encode(arrMsg, sizeof(arrMsg), nOffset, nTotalSize);

	g_Frame.Dump(NULL, pMsgHeadCS, NULL, "send ");
	return pIoSession->Write(arrMsg, nTotalSize);
}
