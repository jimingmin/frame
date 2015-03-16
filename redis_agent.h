/*
 * redis_agent.h
 *
 *  Created on: Mar 16, 2015
 *      Author: jimm
 */

#ifndef REDIS_AGENT_H_
#define REDIS_AGENT_H_

#include <stdlib.h>
#include "../common/common_runnable.h"
#include "../hiredis/adapters/libevent.h"
#include "frame_namespace.h"

FRAME_NAMESPACE_BEGIN

class CRedisAgent : public IRunnable
{
public:
	CRedisAgent();

	virtual ~CRedisAgent();

	virtual int32_t Run();

	void AttachAsyncEvent(redisAsyncContext *pContext);

	void DetachAsyncEvent(redisAsyncContext *pContext);
protected:
	event_base 				*m_pRedisEvtBase;
};

FRAME_NAMESPACE_END

#endif /* REDIS_AGENT_H_ */
