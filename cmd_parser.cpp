/*
 * cmd_parser.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: jimm
 */

#include "cmd_parser.h"
#include "common/common_codeengine.h"
#include "common/common_crypt.h"
#include <string.h>

int32_t CCmdParser::Init()
{
	return 0;
}

int32_t CCmdParser::Uninit()
{
	return 0;
}

int32_t CCmdParser::GetSize()
{
	return sizeof(*this);
}

int32_t CCmdParser::Parser(const uint8_t arrInputBuf[], const uint32_t nInputBufSize, uint8_t arrOutputBuf[], int32_t &nOutputBufSize)
{
	int32_t nTotalSize = 0;
	uint32_t nOffset = 0;
	int32_t nRet = CCodeEngine::Decode(arrInputBuf, nInputBufSize, nOffset, nTotalSize);
	if((nRet != 0) || (nTotalSize <= 0))
	{
		return 0;
	}

	memcpy(arrOutputBuf, arrInputBuf + sizeof(nTotalSize), nTotalSize);
	nOutputBufSize = nTotalSize;

	return nTotalSize + 4;
}




