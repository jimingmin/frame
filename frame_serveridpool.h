/*
 * frame_serveridpool.h
 *
 *  Created on: May 16, 2015
 *      Author: jimm
 */

#ifndef FRAME_SERVERIDPOOL_H_
#define FRAME_SERVERIDPOOL_H_

#include "common/common_object.h"

class CFrameServerIDPool : CBaseObject
{
public:
	virtual ~CFrameServerIDPool(){}

	virtual int32_t Init();
	virtual int32_t Uninit();
	virtual int32_t GetSize();

	uint16_t PopServerID();

protected:
	int32_t			m_nCurIndex;
	int32_t			m_nCapacity;
	uint16_t		m_arrServerID[1024];
};


#endif /* FRAME_SERVERIDPOOL_H_ */
