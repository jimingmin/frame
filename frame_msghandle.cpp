/*
 * frame_msghandle.cpp
 *
 *  Created on: 2014年1月23日
 *      Author: jimm
 */

#include "frame_msgmap.h"
#include "frame_msghandle.h"

#include <stddef.h>

FRAME_NAMESPACE_BEGIN

int32_t FrameMsgCallBack(int32_t nMsgID, va_list ap)
{
	MsgEntry *pEntry = g_MsgMapDecl.GetMsgEntry(nMsgID);
	if(pEntry == NULL)
	{
		return -1;
	}

	int32_t nRet = 0;

	ProcCodeFlag flag = pEntry->m_nProcCodeFlag;

//	va_list ap;
//	va_start(ap, nMsgID);

	switch(flag)
	{
	case enmProcCodeFlag_i32_pco_pmh_pmb:
	{
		CObject *pObj = va_arg(ap, CObject *);
		uint8_t *pBuf = va_arg(ap, uint8_t *);
		int32_t nBufSize = va_arg(ap, int32_t);

		i32_pco_pmh_pmb Proc = pEntry->m_stEntryParam.EP_i32_pco_pmh_pmb.m_pMsgHandleProc;
		CObject *pInstance = pEntry->m_stEntryParam.EP_i32_pco_pmh_pmb.m_pObject;
		IMsgHead *pMsgHead = pEntry->m_stEntryParam.EP_i32_pco_pmh_pmb.m_pMsgHead;
		IMsgBody *pMsgBody = pEntry->m_stEntryParam.EP_i32_pco_pmh_pmb.m_pMsgBody;

		uint32_t nOffset = 0;
		if(pMsgHead->Decode(pBuf, nBufSize, nOffset) != 0)
		{
			return 2;
		}

		if(pMsgBody->Decode(pBuf, nBufSize, nOffset) != 0)
		{
			return 2;
		}

		nRet = (pInstance->*Proc)(pObj, pMsgHead, pMsgBody);
	}
	break;
	case enmProcCodeFlag_i32_pco_pu8_i32:
	{
		CObject *pObj = va_arg(ap, CObject *);
		uint8_t *pBuf = va_arg(ap, uint8_t *);
		int32_t nBufSize = va_arg(ap, int32_t);

		i32_pco_pu8_i32 Proc = pEntry->m_stEntryParam.EP_i32_pco_pu8_i32.m_pMsgHandleProc;
		CObject *pInstance = pEntry->m_stEntryParam.EP_i32_pco_pu8_i32.m_pObject;

		nRet = (pInstance->*Proc)(pObj, pBuf, nBufSize);
	}
	break;
	case enmProcCodeFlag_i32_pco_pmh_pmb_pu8_i32:
	{
		CObject *pObj = va_arg(ap, CObject *);
		uint8_t *pBuf = va_arg(ap, uint8_t *);
		int32_t nBufSize = va_arg(ap, int32_t);

		i32_pco_pmh_pmb_pu8_i32 Proc = pEntry->m_stEntryParam.EP_i32_pco_pmh_pmb_pu8_i32.m_pMsgHandleProc;
		CObject *pInstance = pEntry->m_stEntryParam.EP_i32_pco_pmh_pmb_pu8_i32.m_pObject;
		IMsgHead *pMsgHead = pEntry->m_stEntryParam.EP_i32_pco_pmh_pmb_pu8_i32.m_pMsgHead;
		IMsgBody *pMsgBody = pEntry->m_stEntryParam.EP_i32_pco_pmh_pmb_pu8_i32.m_pMsgBody;

		uint32_t nOffset = 0;
		if(pMsgHead->Decode(pBuf, nBufSize, nOffset) != 0)
		{
			return 2;
		}

		if(pMsgBody->Decode(pBuf, nBufSize, nOffset) != 0)
		{
			return 2;
		}

		nRet = (pInstance->*Proc)(pObj, pMsgHead, pMsgBody, pBuf, nBufSize);
	}
	break;
	default:
		nRet = 1;
	break;
	}

//	va_end(ap);

	return nRet;
}

FRAME_NAMESPACE_END
