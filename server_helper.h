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

using namespace NETEVENT;

class CServerHelper
{
public:
	static void FillControlHead(ControlHead &stCtlHead, uint16_t nTotalSize, ControlCode nCtlCode, uint32_t nUin, SessionID nSessionID,
			uint32_t nClientAddress, uint16_t nClientPort);

	static int32_t MakeMsg(ICtlHead *pCtlHead, IMsgHead *pMsgHead, IMsgBody *pMsgBody, uint8_t *pOutBuf, int32_t nOutBufSize);

	static int32_t SendMsgToClient(IIOSession *pIoSession, MsgHeadCS *pMsgHeadCS, uint8_t *pBuf, int32_t nBufSize);
};

#endif /* SERVER_HELPER_H_ */
