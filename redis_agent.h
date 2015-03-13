/*
 * redis_agent.h
 *
 *  Created on: Feb 10, 2015
 *      Author: jimm
 */

#ifndef REDIS_AGENT_H_
#define REDIS_AGENT_H_

#include <stdlib.h>
#include "../common/common_runnable.h"
#include "../common/common_object.h"
#include "../hiredis/hiredis.h"
#include "../hiredis/async.h"
#include "../hiredis/adapters/libevent.h"
#include "frame_namespace.h"
#include "redis_define.h"
#include <list>
using namespace std;

FRAME_NAMESPACE_BEGIN

#define REDIS_CMD_GET			"GET"
#define REDIS_CMD_SET			"SET"

#define REDIS_CMD_RPUSH			"RPUSH"
#define REDIS_CMD_LLEN			"LLEN"

#define REDIS_CMD_SUBSCRIBE		"SUBSCRIBE"
#define REDIS_CMD_UNSUBSCRIBE	"UNSUBSCRIBE"

enum
{
	enmMaxInternetAddrLen		= 128,
};

class CRedisAgent : public IRunnable
{
public:
	CRedisAgent(int32_t nServerID, char *pAddress, uint16_t nPort);

	virtual ~CRedisAgent();

	virtual int32_t Run();

	virtual int32_t ConnectSuccess();

	int32_t Connect();

	void Close();

	//----------------------------list-----------------------------------------
	int32_t RPush(char *szKey, char *pValue, uint16_t nValueLen, IRedisReplyHandler *pReplyHandler = NULL, CBaseObject *pParam = NULL);

	int32_t LPop(char *szKey, IRedisReplyHandler *pReplyHandler = NULL, CBaseObject *pParam = NULL);

	//----------------------------sub/pub--------------------------------------
	int32_t Subscribe(char *szKey, IRedisReplyHandler *pReplyHandler = NULL, CBaseObject *pParam = NULL);

	int32_t Unsubscribe(char *szKey, IRedisReplyHandler *pReplyHandler = NULL, CBaseObject *pParam = NULL);

	void OnUnsubscribeReply(const redisAsyncContext *pContext, void *pReply, void *pSession);

	int32_t Publish(char *szKey, char *pValue, uint16_t nValueLen, IRedisReplyHandler *pReplyHandler = NULL, CBaseObject *pParam = NULL);

	void OnConnected(const redisAsyncContext *pContext, int32_t nStatus);

	void OnClosed(const redisAsyncContext *pContext, int32_t nStatus);

	void OnRedisReply(const redisAsyncContext *pContext, void *pReply, void *pSession);

	bool IsConnected();

protected:
	int32_t SendCommand(redisCallbackFn *pFunc, void *pSession, const char *szFormat, ...);

	int32_t SendCommand(const char *szCommand, char *pKey, char *pValue, uint16_t nValueLen, redisCallbackFn *pFunc = NULL, void *pSession = NULL);

	int32_t SendCommand(const char *szCommand, char *pKey, void *pSession = NULL);

	int32_t SendCommand(const char *szCommand, char *pKey, char *pValue, uint16_t nValueLen, void *pSession = NULL);

private:
	bool					m_bConnectSuccess;
	int32_t					m_nServerID;
	char					m_arrAddress[enmMaxInternetAddrLen];
	uint16_t				m_nPort;
	event_base 				*m_pRedisEvtBase;
	redisAsyncContext		*m_pRedisContext;
	list<RedisCmdSession *>	m_stLongSessionList;
};

FRAME_NAMESPACE_END

#endif /* REDIS_AGENT_H_ */
