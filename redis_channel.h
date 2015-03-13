/*
 * redis_channel.h
 *
 *  Created on: Mar 11, 2015
 *      Author: jimm
 */

#ifndef REDIS_CHANNEL_H_
#define REDIS_CHANNEL_H_

#include "redis_agent.h"
#include <string>
using namespace std;
using namespace FRAME;

class CRedisChannel : public CRedisAgent
{
public:
	CRedisChannel(int32_t nServerID, char *pAddress, uint16_t nPort, char *pChannelKey, char *pChannelMode);

	virtual int32_t ConnectSuccess();

	int32_t RPush(char *pValue, uint16_t nValueLen, IRedisReplyHandler *pReplyHandler = NULL, CBaseObject *pParam = NULL);

	int32_t LPop(IRedisReplyHandler *pReplyHandler = NULL, CBaseObject *pParam = NULL);

	//----------------------------sub/pub--------------------------------------
	int32_t Subscribe(IRedisReplyHandler *pReplyHandler = NULL, CBaseObject *pParam = NULL);

	int32_t Unsubscribe(IRedisReplyHandler *pReplyHandler = NULL, CBaseObject *pParam = NULL);

	void OnUnsubscribeReply(const redisAsyncContext *pContext, void *pReply, void *pSession);

	int32_t Publish(char *pValue, uint16_t nValueLen, IRedisReplyHandler *pReplyHandler = NULL, CBaseObject *pParam = NULL);

	void AttachReplyHandler(IRedisReplyHandler *pReplyHandler);

protected:
	string				m_strChannelKey;
	string				m_strChannelMode;
	IRedisReplyHandler	*m_pRedisReplyHandler;
};


#endif /* REDIS_CHANNEL_H_ */
