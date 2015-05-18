/*
 * redis_agent.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: jimm
 */

#include "redis_agent.h"
#include "frame.h"

FRAME_NAMESPACE_BEGIN

CRedisAgent::CRedisAgent()
{
	m_pRedisEvtBase = event_base_new();
	g_Frame.AddRunner(this);
}

CRedisAgent::~CRedisAgent()
{
	event_base_free(m_pRedisEvtBase);
}

int32_t CRedisAgent::Run()
{
	event_base_loop(m_pRedisEvtBase, EVLOOP_NONBLOCK);
	return 0;
}

void CRedisAgent::AttachAsyncEvent(redisAsyncContext *pContext)
{
	redisLibeventAttach(pContext, m_pRedisEvtBase);
}

void CRedisAgent::DetachAsyncEvent(redisAsyncContext *pContext)
{

}

FRAME_NAMESPACE_END

