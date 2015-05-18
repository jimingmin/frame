/*
 * frame_impl.h
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#ifndef FRAME_IMPL_H_
#define FRAME_IMPL_H_

#include "common/common_object.h"
#include "common/common_typedef.h"
#include "common/common_export.h"
#include "netevent/net_handler.h"
#include "frame_namespace.h"

using namespace NETEVENT;

FRAME_NAMESPACE_BEGIN

class EXPORT IInitFrame
{
public:
	IInitFrame(){};
	virtual ~IInitFrame(){};

	virtual int32_t InitFrame(CNetHandler *pNetHandler) = 0;
};

class EXPORT IConfig : public CBaseObject
{
public:
	IConfig(){};
	virtual ~IConfig(){};

	//初始化配置
	virtual int32_t Init() = 0;
	//卸载配置
	virtual int32_t Uninit() = 0;

	virtual int32_t GetSize()
	{
		return 0;
	}

	virtual bool IsSafe()
	{
		return false;
	}

	virtual int32_t Parser(char *pXMLString)
	{
		return 0;
	}
};

class EXPORT IBank : public CBaseObject
{
public:
	IBank(){};
	virtual ~IBank(){};

	//初始化
	virtual int32_t Init() = 0;
	//卸载
	virtual int32_t Uninit() = 0;

	virtual int32_t GetSize()
	{
		return 0;
	}
};

class IMsg : public CBaseObject
{
public:
	virtual ~IMsg(){};

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
		return 0;
	}

	//virtual int32_t Encode(Value &value, Document::AllocatorType& allocator) = 0;
	virtual int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset) = 0;

	virtual int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset) = 0;

	virtual void Dump(char* buf, const uint32_t size, uint32_t& offset) = 0;
};

typedef IMsg	IMsgHead;
typedef IMsg	IMsgBody;
typedef IMsg	ICtlHead;

class IRedisReplyHandler : public CBaseObject
{
public:
	IRedisReplyHandler()
	{
		Init();
	}

	virtual ~IRedisReplyHandler()
	{
		Uninit();
	}

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
		return 0;
	}

	virtual int32_t OnRedisReply(int32_t nResult, void *pReply, CBaseObject *pParam) = 0;
};

FRAME_NAMESPACE_END

#endif /* FRAME_IMPL_H_ */
