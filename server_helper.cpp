/*
 * server_helper.cpp
 *
 *  Created on: Mar 13, 2015
 *      Author: jimm
 */

#include "server_helper.h"
#include "common/common_crypt.h"
#include "common/common_datetime.h"
#include "../include/typedef.h"
#include "../include/account_msg.h"
#include "../include/sync_msg.h"
#include "frame/cachekey_define.h"
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

void CServerHelper::Split(uint8_t *szInputBuf, int32_t nInputSize, uint8_t *szOutputBuf, int32_t &nOutputSize, int32_t nSplitSize)
{
	for (nOutputSize = 0; nOutputSize < nInputSize;)
	{
		if((szInputBuf[nOutputSize] & 0xf0) == 0xe0) /**1110xxxx 10xxxxxx 10xxxxxx*/
		{
			if(nOutputSize + 3 > nSplitSize)
			{
				break;
			}
			nOutputSize += 3;
		}
		else if((szInputBuf[nOutputSize] & 0xe0) == 0xc0)/**110xxxxx 10xxxxxx*/
		{
			if(nOutputSize + 2 > nSplitSize)
			{
				break;
			}
			nOutputSize += 2;
		}
		else if (szInputBuf[nOutputSize] <= 0x7f)
		{
			if(nOutputSize + 1 > nSplitSize)
			{
				break;
			}
			nOutputSize += 1;
		}
		else
		{
			break;
		}
	}

	memcpy(szOutputBuf, szInputBuf, nOutputSize);
}

void CServerHelper::FillControlHead(ControlHead &stCtlHead, uint16_t nTotalSize, ControlCode nCtlCode, uint32_t nUin, SessionID nSessionID,
			uint32_t nClientAddress, uint16_t nClientPort, int32_t nGateID, uint8_t nPhoneType, uint32_t nGateRedisAddress,
			uint16_t nGateRedisPort)
{
	stCtlHead.m_nTotalSize = nTotalSize;
	stCtlHead.m_nControlCode = nCtlCode;
	stCtlHead.m_nUin = nUin;
	stCtlHead.m_nSessionID = nSessionID;
	stCtlHead.m_nClientAddress = nClientAddress;
	stCtlHead.m_nClientPort = nClientPort;
	stCtlHead.m_nGateID = nGateID;
	stCtlHead.m_nTimeStamp = CTimeValue::CurrentTime().Milliseconds();
	stCtlHead.m_nPhoneType = nPhoneType;
	stCtlHead.m_nGateRedisAddress = nGateRedisAddress;
	stCtlHead.m_nGateRedisPort = nGateRedisPort;
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

int32_t CServerHelper::SendSyncNoti(CRedisChannel *pPushClientChannel, ControlHead *pControlHead, uint32_t nUin)
{
	MsgHeadCS stMsgHeadCS;
	stMsgHeadCS.m_nMsgID = MSGID_STATUSSYNC_NOTI;
	stMsgHeadCS.m_nSrcUin = nUin;
	stMsgHeadCS.m_nDstUin = nUin;

	CStatusSyncNoti stStatusSyncNoti;

	uint8_t arrRespBuf[MAX_MSG_SIZE];

	uint16_t nTotalSize = CServerHelper::MakeMsg(pControlHead, &stMsgHeadCS, &stStatusSyncNoti, arrRespBuf, sizeof(arrRespBuf));
	return pPushClientChannel->RPush(NULL, MakeRedisKey(ClientResp::keyname, pControlHead->m_nGateID), (char *)arrRespBuf, nTotalSize);
}

int32_t CServerHelper::PushToAPNS(CRedisChannel *pPushAPNSChannel, uint32_t nSrcUin, uint32_t nDstUin, uint16_t nMsgID, uint8_t *pBuf, int32_t nBufSize)
{
	int32_t nContentLen = (nBufSize >= 50 ? 50 : nBufSize);
	char szApnsParams[1024];
	int32_t nParamsLen = sprintf(szApnsParams, "%u:%u:%d:%d:", nSrcUin, nDstUin, nMsgID, nContentLen);
	memcpy(&szApnsParams[nParamsLen], pBuf, nContentLen);

	return pPushAPNSChannel->RPush(NULL, "client:resp", szApnsParams, nParamsLen + nContentLen);
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


int32_t CServerHelper::KickUser(ControlHead *pControlHead, MsgHeadCS *pMsgHeadCS, CRedisChannel *pRedisChannel, const char *strReason)
{
	ControlHead stCtlHead;
	stCtlHead = *pControlHead;
	stCtlHead.m_nControlCode = enmControlCode_Close;

	MsgHeadCS stMsgHeadCS;
	stMsgHeadCS.m_nMsgID = MSGID_KICKUSER_NOTI;
	stMsgHeadCS.m_nSrcUin = pMsgHeadCS->m_nSrcUin;
	stMsgHeadCS.m_nDstUin = pMsgHeadCS->m_nDstUin;

	CKickUserNoti stKickUserNoti;
	stKickUserNoti.m_strReason = strReason;

	uint8_t arrRespBuf[MAX_MSG_SIZE];

	uint16_t nTotalSize = CServerHelper::MakeMsg(&stCtlHead, &stMsgHeadCS, &stKickUserNoti, arrRespBuf, sizeof(arrRespBuf));
	return pRedisChannel->RPush(NULL, MakeRedisKey(ClientResp::keyname, pControlHead->m_nGateID), (char *)arrRespBuf, nTotalSize);
}

const char *CServerHelper::MakeRedisKey(const char *szKey)
{
	return szKey;
}

const char *CServerHelper::MakeRedisKey(const char *szBaseKey, int32_t nParamKey)
{
	static char szRedisKey[1024];
	int32_t nRedisKeyLen = sprintf(szRedisKey, "%s%d", szBaseKey, nParamKey);
	szRedisKey[nRedisKeyLen] = '\0';
	return szRedisKey;
}

const char *CServerHelper::MakeRedisKey(const char *szBaseKey, uint32_t nParamKey)
{
	static char szRedisKey[1024];
	int32_t nRedisKeyLen = sprintf(szRedisKey, "%s%u", szBaseKey, nParamKey);
	szRedisKey[nRedisKeyLen] = '\0';
	return szRedisKey;
}

const char *CServerHelper::MakeRedisKey(const char *szBaseKey, const char *szParamKey)
{
	static char szRedisKey[1024];
	int32_t nRedisKeyLen = sprintf(szRedisKey, "%s%s", szBaseKey, szParamKey);
	szRedisKey[nRedisKeyLen] = '\0';
	return szRedisKey;
}
