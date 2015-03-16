/*
 * redissession_bank.h
 *
 *  Created on: Mar 15, 2015
 *      Author: jimm
 */

#ifndef REDISSESSION_BANK_H_
#define REDISSESSION_BANK_H_

#include "frame_impl.h"
#include <list>
#include <map>
#include "redis_session.h"

using namespace std;
using namespace FRAME;

#define BANK_REDIS_SESSION		"bank_redis_session"
#define MAX_REDISSESSION_COUNT	100

class CRedisSessionBank : public IBank
{
public:
	//初始化
	virtual int32_t Init();
	//卸载
	virtual int32_t Uninit();

	RedisSession *CreateSession(CBaseObject *pHandlerObj, RedisReply HandleReply, TimerProc HandleTimeout, int64_t nTimeout = 60 * MS_PER_SECOND);

	RedisSession *GetSession(uint32_t nSessionIndex);

	void DestroySession(RedisSession *pRedisSession);

	void DestroySession(uint32_t nSessionIndex);

protected:
	uint32_t						m_nSessionCounter;
	list<RedisSession *>			m_stUnusedSessionList;
	map<uint32_t, RedisSession *>	m_stUsedSessionMap;
};


#endif /* REDISSESSION_BANK_H_ */
