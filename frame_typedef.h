/*
 * frame_typedef.h
 *
 *  Created on: 2014年1月23日
 *      Author: jimm
 */

#ifndef FRAME_TYPEDEF_H_
#define FRAME_TYPEDEF_H_

#include "../common/common_object.h"
#include "frame_namespace.h"

#include "../rapidjson/document.h"
using namespace rapidjson;

FRAME_NAMESPACE_BEGIN

#define MODULE_NAME		"frame"

class IMsg : public CObject
{
public:
	virtual ~IMsg(){};

	virtual int32_t Encode(Value &value, Document::AllocatorType& allocator) = 0;

	virtual int32_t Decode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset) = 0;
};

typedef IMsg	IMsgHead;
typedef IMsg	IMsgBody;

FRAME_NAMESPACE_END

#endif /* FRAME_TYPEDEF_H_ */
