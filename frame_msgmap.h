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
#include "../common/common_singleton.h"
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

	void Clear()
	{
		m_stMsgMap.clear();
	}
protected:
	map<uint32_t, MsgEntry *>		m_stMsgMap;
};

#define g_MsgMapDecl				CSingleton<CMsgMapDecl>::GetInstance()

#define MSGMAP_BEGIN(entity)	\
class CMsgMapDecl_##entity	\
{	\
public:	\
	CMsgMapDecl_##entity()	\
	{	\
		DeclMsgMap();	\
	}	\
	~CMsgMapDecl_##entity()	\
	{	\
	}	\
	void DeclMsgMap()	\
	{

#define ON_PROC_PMH_PMB(id, msghead, msgbody, obj, msgproc)	\
		g_MsgMapDecl.RegistMsgEntry(id, new msghead(), new msgbody(), new obj(), static_cast<i32_pco_pmh_pmb>(&msgproc));

#define ON_PROC_PMH_PMB_PU8_I32(id, msghead, msgbody, obj, msgproc)	\
		g_MsgMapDecl.RegistMsgEntry(id, new msghead(), new msgbody(), new obj(), static_cast<i32_pco_pmh_pmb_pu8_i32>(&msgproc));

#define ON_STREAMEVENT(id, obj, msgproc)	\
		g_MsgMapDecl.RegistMsgEntry(id, new obj(), static_cast<i32_pco_pu8_i32>(&msgproc));

#define MSGMAP_END(entity)	\
	}	\
};	\
static CMsgMapDecl_##entity l_##entity;

FRAME_NAMESPACE_END

#endif /* FRAME_MSGMAP_H_ */

