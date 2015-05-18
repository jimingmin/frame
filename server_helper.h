/*
 * server_helper.h
 *
 *  Created on: Mar 12, 2015
 *      Author: jimm
 */

#ifndef SERVER_HELPER_H_
#define SERVER_HELPER_H_

#include "common/common_codeengine.h"
#include "../include/control_head.h"
#include "../include/msg_head.h"
#include "netevent/net_impl.h"
#include "rapidjson/document.h"
#include "redis_channel.h"
using namespace rapidjson;

using namespace NETEVENT;

#define	KickReason_AnotherLogin		"帐号在另一个设备登录"
#define KickReason_AccountLocked	"帐号被锁定"
#define	KickReason_NotLogined		"帐号未登录"
#define	KickReason_Unknown			"服务器未知错误"

class CServerHelper
{
public:
	static bool IsExist(Document &document, const char *szKey);

	static bool GetInt(Document &document, const char *szKey, int32_t &nValue);

	static bool GetUint(Document &document, const char *szKey, uint32_t &nValue);

	static bool GetString(Document &document, const char *szKey, string &nValue);

	static bool GetValue(Document &document, const char *szKey, Value &nValue);

	static bool IsExist(Value &value, const char *szKey);

	static bool GetInt(Value &value, const char *szKey, int32_t &nValue);

	static bool GetString(Value &value, const char *szKey, string &nValue);

	static bool GetValue(Value &value, const char *szKey, Value &nValue);

	static void Split(uint8_t *szInputBuf, int32_t nInputSize, uint8_t *szOutputBuf, int32_t &nOutputSize, int32_t nSplitSize);

	static void FillControlHead(ControlHead &stCtlHead, uint16_t nTotalSize, ControlCode nCtlCode, uint32_t nUin, SessionID nSessionID,
			uint32_t nClientAddress, uint16_t nClientPort, int32_t nGateID, uint8_t nPhoneType, uint32_t nGateRedisAddress,
			uint16_t nGateRedisPort);

	static int32_t MakeMsg(ICtlHead *pCtlHead, IMsgHead *pMsgHead, IMsgBody *pMsgBody, uint8_t *pOutBuf, int32_t nOutBufSize);

	static int32_t SendSyncNoti(CRedisChannel *pPushClientChannel, ControlHead *pControlHead, uint32_t nUin);

	static int32_t PushToAPNS(CRedisChannel *pPushAPNSChannel, uint32_t nSrcUin, uint32_t nDstUin, uint16_t nMsgID, uint8_t *pBuf, int32_t nBufSize);

	static int32_t SendMsgToClient(IIOSession *pIoSession, MsgHeadCS *pMsgHeadCS, uint8_t *pBuf, int32_t nBufSize);

	static int32_t KickUser(ControlHead *pControlHead, MsgHeadCS *pMsgHeadCS, CRedisChannel *pRedisChannel, const char *szReason);

	static const char *MakeRedisKey(const char *szKey);

	static const char *MakeRedisKey(const char *szBaseKey, int32_t nParamKey);

	static const char *MakeRedisKey(const char *szBaseKey, uint32_t nParamKey);

	static const char *MakeRedisKey(const char *szBaseKey, const char *szParamKey);
};

#endif /* SERVER_HELPER_H_ */
