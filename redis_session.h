/*
 * redis_session.h
 *
 *  Created on: Mar 14, 2015
 *      Author: jimm
 */

#ifndef REDIS_SESSION_H_
#define REDIS_SESSION_H_

#include "../common/common_object.h"
#include <stddef.h>

typedef int32_t (CBaseObject::*HandleRedisReply)(int32_t nResult, void *pReply, void *pSession);

#define MAX_SESSIONDATA_SIZE		(8 * 1024)

class RedisSession : public CBaseObject
{
public:
	RedisSession(CBaseObject *pHandlerObj, HandleRedisReply Func)
	{
		m_pHandlerObj = pHandlerObj;
		m_pHandleRedisReply = static_cast<HandleRedisReply>(Func);
	}

	virtual ~RedisSession()
	{
		Uninit();
	}

	virtual int32_t Init()
	{
		return 0;
	}
	virtual int32_t Uninit()
	{
		return 0;
	}
	virtual int32_t GetSize()
	{
		return 0;
	}

	CBaseObject *GetHandlerObj()
	{
		return m_pHandlerObj;
	}

	HandleRedisReply GetHandleRedisReply()
	{
		return m_pHandleRedisReply;
	}

	uint8_t *GetSessionData()
	{
		return m_arrSessionData;
	}

protected:
	CBaseObject			*m_pHandlerObj;
	HandleRedisReply	m_pHandleRedisReply;
	uint8_t				m_arrSessionData[MAX_SESSIONDATA_SIZE];
};


#endif /* REDIS_SESSION_H_ */
