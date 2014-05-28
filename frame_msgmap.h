/*
 * frame_msgmap.h
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#ifndef FRAME_MSGMAP_H_
#define FRAME_MSGMAP_H_

#include "../common/common_object.h"
#include "../common/common_typedef.h"
#include "frame_msgentry.h"
#include "frame_msghandle.h"
#include "frame_namespace.h"

#include <map>
using namespace std;

FRAME_NAMESPACE_BEGIN

class CMsgMapDecl
{
public:
	CMsgMapDecl()
	{

	}
	~CMsgMapDecl(){}

	MsgEntry *GetMsgEntry(uint32_t nMsgID)
	{
		map<uint32_t, MsgEntry *>::iterator it = m_stMsgMap.find(nMsgID);
		if(it != m_stMsgMap.end())
		{
			return it->second;
		}

		return 0;
	}

	void RegistMsgEntry(uint32_t nMsgID, IMsgHead *pMsgHead, IMsgBody *pMsgBody, CObject *pObj, i32_pco_pmh_pmb Proc)
	{
		MsgEntry *pMsgEntry = new MsgEntry(pObj, pMsgHead, pMsgBody, Proc);
		m_stMsgMap[nMsgID] = pMsgEntry;
	}

	void RegistMsgEntry(uint32_t nMsgID, CObject *pObj, i32_pco_pu8_i32 Proc)
	{
		MsgEntry *pMsgEntry = new MsgEntry(pObj, Proc);
		m_stMsgMap[nMsgID] = pMsgEntry;
	}

	void RegistMsgEntry(uint32_t nMsgID, IMsgHead *pMsgHead, IMsgBody *pMsgBody, CObject *pObj, i32_pco_pmh_pmb_pu8_i32 Proc)
	{
		MsgEntry *pMsgEntry = new MsgEntry(pObj, pMsgHead, pMsgBody, Proc);
		m_stMsgMap[nMsgID] = pMsgEntry;
	}

	void RegistMsgEntry(uint32_t nMsgID, IMsgHead *pMsgHead, CObject *pObj, i32_pco_pmh_pu8_i32 Proc)
	{
		MsgEntry *pMsgEntry = new MsgEntry(pObj, pMsgHead, Proc);
		m_stMsgMap[nMsgID] = pMsgEntry;
	}

	void Clear()
	{
		m_stMsgMap.clear();
	}
protected:
	map<uint32_t, MsgEntry *>		m_stMsgMap;
};

FRAME_NAMESPACE_END

#endif /* FRAME_MSGMAP_H_ */

