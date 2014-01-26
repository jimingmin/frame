/*
 * frame_msghandle.h
 *
 *  Created on: 2014年1月23日
 *      Author: jimm
 */

#ifndef FRAME_MSGHANDLE_H_
#define FRAME_MSGHANDLE_H_

#include "../common/common_object.h"
#include "frame_typedef.h"
#include "frame_namespace.h"

#include <stdarg.h>

FRAME_NAMESPACE_BEGIN

typedef int32_t (CObject::*i32_pco_pmh_pmb)(CObject *, IMsgHead *, IMsgBody *);
typedef int32_t	(CObject::*i32_pco_pu8_i32)(CObject *, uint8_t *, int32_t);

enum ProcCodeFlag
{
	enmProcCodeFlag_Invalid				= 0,
	enmProcCodeFlag_i32_pco_pmh_pmb		= 1,
	enmProcCodeFlag_i32_pco_pu8_i32		= 2,
};


int32_t FrameMsgCallBack(int32_t nMsgID, ...);


FRAME_NAMESPACE_END

#endif /* FRAME_MSGHANDLE_H_ */
