/*
 * frame_msgentry.h
 *
 *  Created on: 2014年1月23日
 *      Author: jimm
 */

#ifndef FRAME_MSGENTRY_H_
#define FRAME_MSGENTRY_H_

#include "../common/common_object.h"
#include "frame_typedef.h"
#include "frame_msghandle.h"
#include "frame_namespace.h"

FRAME_NAMESPACE_BEGIN

class MsgEntry
{
public:
	MsgEntry(CObject *pObj, IMsgHead *pMsgHead, IMsgBody *pMsgBody, i32_pco_pmh_pmb Proc)
	{
		m_nProcCodeFlag = enmProcCodeFlag_i32_pco_pmh_pmb;
		m_stEntryParam.EP_i32_pco_pmh_pmb.m_pObject = pObj;
		m_stEntryParam.EP_i32_pco_pmh_pmb.m_pMsgHead = pMsgHead;
		m_stEntryParam.EP_i32_pco_pmh_pmb.m_pMsgBody = pMsgBody;
		m_stEntryParam.EP_i32_pco_pmh_pmb.m_pMsgHandleProc = Proc;
	}

	MsgEntry(CObject *pObj, i32_pco_pu8_i32 Proc)
	{
		m_nProcCodeFlag = enmProcCodeFlag_i32_pco_pu8_i32;
		m_stEntryParam.EP_i32_pco_pu8_i32.m_pObject = pObj;
		m_stEntryParam.EP_i32_pco_pu8_i32.m_pMsgHandleProc = Proc;
	}

	ProcCodeFlag	m_nProcCodeFlag;

	struct EntryParam_i32_pco_pmh_pmb
	{
		CObject			*m_pObject;
		IMsgHead		*m_pMsgHead;
		IMsgBody		*m_pMsgBody;
		i32_pco_pmh_pmb	m_pMsgHandleProc;
	};

	struct EntryParam_i32_pco_pu8_i32
	{
		CObject			*m_pObject;
		i32_pco_pu8_i32	m_pMsgHandleProc;
	};

	union EntryParam
	{
		EntryParam_i32_pco_pmh_pmb 		EP_i32_pco_pmh_pmb;
		EntryParam_i32_pco_pu8_i32		EP_i32_pco_pu8_i32;
	} m_stEntryParam;
};

FRAME_NAMESPACE_END

#endif /* FRAME_MSGENTRY_H_ */
