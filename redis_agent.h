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
#include <list>
using namespace std;

FRAME_NAMESPACE_BEGIN

enum
{
	enmMaxInternetAddrLen		= 128,
};

class CRedisAgent : public IRunnable
{
	friend class CRedisGlue;
public:
	CRedisAgent(int32_t nServerID, char *pAddress, uint16_t nPort);

	virtual ~CRedisAgent();

	virtual int32_t Run();

	virtual int32_t OnConnected();

	virtual void OnClosed();

	virtual void OnUnsubscribeReply(const redisAsyncContext *pContext, void *pReply, void *pSession);

	int32_t Connect();

	void Close();

	bool IsConnected();

protected:

	void Connected(const redisAsyncContext *pContext, int32_t nStatus);

	void Closed(const redisAsyncContext *pContext, int32_t nStatus);

	void OnRedisReply(const redisAsyncContext *pContext, void *pReply, void *pSession);

	int32_t SendCommand(const char *szCommand, char *szKey, void *pSession = NULL);

	int32_t SendCommand(const char *szCommand, char *szKey, void *pSession, const char *szFormat, va_list ap);

	int32_t SendCommand(const char *szCommand, char *szKey, void *pSession, const char *szFormat, ...);

private:
	bool					m_bConnectSuccess;
	int32_t					m_nServerID;
	char					m_arrAddress[enmMaxInternetAddrLen];
	uint16_t				m_nPort;
	event_base 				*m_pRedisEvtBase;
	redisAsyncContext		*m_pRedisContext;
};

FRAME_NAMESPACE_END

#endif /* REDIS_AGENT_H_ */
