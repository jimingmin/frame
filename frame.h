/*
 * frame.h
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#ifndef FRAME_H_
#define FRAME_H_

#include "../common/common_typedef.h"
#include "frame_impl.h"
#include "frame_namespace.h"

#include <list>
using namespace std;

FRAME_NAMESPACE_BEGIN

class CFrame
{
public:
	CFrame();
	virtual ~CFrame();

	int32_t Init();

	int32_t Uninit();

	int32_t Run();

	void AddRunner(IRunnable *pRunner);

protected:
	list<IRunnable *>		m_stRunnerList;
};

FRAME_NAMESPACE_END

#endif /* FRAME_H_ */

