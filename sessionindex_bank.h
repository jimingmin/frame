/*
 * sessionindex_bank.h
 *
 *  Created on: Mar 15, 2015
 *      Author: jimm
 */

#ifndef SESSIONINDEX_BANK_H_
#define SESSIONINDEX_BANK_H_

#include "frame_impl.h"
#include <list>
#include <set>

using namespace std;
using namespace FRAME;

#define BANK_SESSION_INDEX			"bank_session_index"
#define MAX_SESSIONINDEX_COUNT		100

class CSessionIndexBank : public IBank
{
public:
	//初始化
	virtual int32_t Init();
	//卸载
	virtual int32_t Uninit();

	uint32_t *CreateSessionIndex();

	void DestroySessionIndex(uint32_t *pSessionIndex);

protected:
	list<uint32_t *>			m_stUnusedSessionIndexList;
	set<uint32_t *>				m_stUsedSessionIndexSet;
};

#endif /* SESSIONINDEX_BANK_H_ */
