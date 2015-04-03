/*
 * server_helper.h
 *
 *  Created on: Mar 12, 2015
 *      Author: jimm
 */

#ifndef SERVER_HELPER_H_
#define SERVER_HELPER_H_

#include "../common/common_codeengine.h"
#include "../include/control_head.h"
#include "../include/msg_head.h"
#include "../netevent/net_impl.h"
#include "../rapidjson/document.h"
using namespace rapidjson;

using namespace NETEVENT;

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

	static void FillControlHead(ControlHead &stCtlHead, uint16_t nTotalSize, ControlCode nCtlCode, uint32_t nUin, SessionID nSessionID,
			uint32_t nClientAddress, uint16_t nClientPort);

	static int32_t MakeMsg(ICtlHead *pCtlHead, IMsgHead *pMsgHead, IMsgBody *pMsgBody, uint8_t *pOutBuf, int32_t nOutBufSize);

	static int32_t SendMsgToClient(IIOSession *pIoSession, MsgHeadCS *pMsgHeadCS, uint8_t *pBuf, int32_t nBufSize);
};

#endif /* SERVER_HELPER_H_ */
