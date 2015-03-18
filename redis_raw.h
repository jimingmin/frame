/*
 * redis_raw.h
 *
 *  Created on: Feb 10, 2015
 *      Author: jimm
 */

#ifndef REDIS_RAW_H_
#define REDIS_RAW_H_

#include "../common/common_runnable.h"
#include "../common/common_object.h"
#include "../hiredis/hiredis.h"
#include "../hiredis/async.h"
#include "../include/typedef.h"
#include "redis_agent.h"
#include "frame_namespace.h"
#include <list>
using namespace std;

FRAME_NAMESPACE_BEGIN

#define MAX_REDIS_CMD_SIZE		4 * 1024

class CRedisRaw : public CBaseObject
{
	friend class CRedisGlue;
public:
	CRedisRaw(int32_t nServerID, char *pAddress, uint16_t nPort);

	virtual ~CRedisRaw();

	virtual int32_t Init();

	virtual int32_t Uninit();

	virtual int32_t GetSize();

	virtual int32_t OnConnected();

	virtual void OnClosed();

	virtual void OnUnsubscribeReply(const redisAsyncContext *pContext, void *pReply, void *pSessionIndex);

	int32_t Connect();

	void Close();

	bool IsConnected();

protected:

	void Connected(const redisAsyncContext *pContext, int32_t nStatus);

	void Closed(const redisAsyncContext *pContext, int32_t nStatus);

	void OnRedisReply(const redisAsyncContext *pContext, void *pReply, void *pSessionIndex);

	int32_t FormatCommand(char szCommand[], int32_t &nCmdSize, const char *szFormat, ...);

	int32_t SendCommand(const char *szCommand, char *szKey, void *pSession = NULL);

	int32_t SendCommand(const char *szCommand, char *szKey, void *pSession, const char *szFormat, va_list ap);

	int32_t SendCommand(const char *szCommand, char *szKey, void *pSession, const char *szFormat, ...);

	int32_t SendCommand(const char *szCommand, char *szKey, void *pSession, int32_t nArgc, const char **Argv, const size_t *ArgvLen);

	int32_t SendFormatedCommand(void *pSession, char *szCommand, int32_t nCmdLen);

private:
	static CRedisAgent		m_stRedisAgent;
	bool					m_bConnectSuccess;
	int32_t					m_nServerID;
	char					m_arrAddress[enmMaxInternetAddrLen];
	uint16_t				m_nPort;
	redisAsyncContext		*m_pRedisContext;
};

FRAME_NAMESPACE_END

#endif /* REDIS_RAW_H_ */
