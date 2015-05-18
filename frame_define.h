/*
 * frame_define.h
 *
 *  Created on: Mar 6, 2015
 *      Author: jimm
 */

#ifndef FRAME_DEFINE_H_
#define FRAME_DEFINE_H_

#include "common/common_codeengine.h"
#include "frame_namespace.h"
#include "frame_impl.h"
#include <stdio.h>

FRAME_NAMESPACE_BEGIN

#define MODULE_NAME			"frame"

class MsgHead : public IMsgHead
{
public:
	uint16_t		m_nTotalSize;
	uint16_t		m_nMsgID;
	uint32_t		m_nSeq;
	uint32_t		m_nSrcUin;
	uint32_t		m_nDstUin;

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
		return sizeof(m_nTotalSize) + sizeof(m_nMsgID) + sizeof(m_nSeq) + sizeof(m_nSrcUin) + sizeof(m_nDstUin);
	}

	//virtual int32_t Encode(Value &value, Document::AllocatorType& allocator) = 0;
	virtual int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nTotalSize);
		CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nMsgID);
		CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nSeq);
		CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nSrcUin);
		CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nDstUin);
		return 0;
	}

	virtual int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = 0;
		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nTotalSize);
		if(nRet < 0)
		{
			return 1;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nMsgID);
		if(nRet < 0)
		{
			return 1;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nSeq);
		if(nRet < 0)
		{
			return 1;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nSrcUin);
		if(nRet < 0)
		{
			return 1;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nDstUin);
		if(nRet < 0)
		{
			return 1;
		}

		return nRet;
	}

	virtual void Dump(char* buf, const uint32_t size, uint32_t& nOffset)
	{
		uint32_t nLen = sprintf(buf, "msghead={m_nTotalSize=%d, m_nMsgID=%d, m_nSeq=%u, m_nSrcUin=%u, m_nDstUin=%u}",
				m_nTotalSize, m_nMsgID, m_nSeq, m_nSrcUin, m_nDstUin);
		nOffset += nLen;
	}
};

FRAME_NAMESPACE_END

#endif /* FRAME_DEFINE_H_ */
