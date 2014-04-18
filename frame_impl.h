/*
 * frame_impl.h
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#ifndef FRAME_IMPL_H_
#define FRAME_IMPL_H_

#include "../common/common_typedef.h"
#include "../common/common_export.h"
#include "frame_namespace.h"

FRAME_NAMESPACE_BEGIN

class EXPORT IConfig
{
public:
	IConfig(){};
	virtual ~IConfig(){};

	//初始化配置
	virtual int32_t Init() = 0;
	//卸载配置
	virtual int32_t Uninit() = 0;
};

FRAME_NAMESPACE_END

#endif /* FRAME_IMPL_H_ */
