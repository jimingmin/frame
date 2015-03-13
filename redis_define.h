/*
 * redis_define.h
 *
 *  Created on: Mar 9, 2015
 *      Author: jimm
 */

#ifndef REDIS_DEFINE_H_
#define REDIS_DEFINE_H_

#include <string>
#include "frame_impl.h"

using namespace FRAME;
using namespace std;

typedef uint8_t			RCSessionType;
enum
{
	enmRCSessionType_Short		= 0x00,
	enmRCSessionType_Long		= 0x01,
};

class RedisCmdSession : public CBaseObject
{
public:
	RedisCmdSession()
	{
		Init();
	}

	virtual ~RedisCmdSession()
	{
		Uninit();
	}

	virtual int32_t Init()
	{
		m_pRSCallback = NULL;
		m_pParam = NULL;
		return 0;
	}
	virtual int32_t Uninit()
	{
		return 0;
	}

	virtual int32_t GetSize()
	{
		return sizeof(*this);
	}

	int32_t OnSession(int32_t nResult, void *pRedisReply)
	{
		return m_pRSCallback->OnRedisReply(nResult, pRedisReply, m_pParam);
	}

	RCSessionType				m_nRCSessionType;
	string						m_strKey;
	IRedisReplyHandler			*m_pRSCallback;
	CBaseObject					*m_pParam;
};


#endif /* REDIS_DEFINE_H_ */
