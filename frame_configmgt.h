/*
 * frame_configmgt.h
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#ifndef FRAME_CONFIGMGT_H_
#define FRAME_CONFIGMGT_H_

#include "common/common_object.h"
#include "frame_impl.h"
#include "frame_namespace.h"

#include <map>
#include <string>
using namespace std;

FRAME_NAMESPACE_BEGIN

class CFrameConfigMgt : public CBaseObject
{
	typedef map<string, IConfig *>		ConfigMgt;
public:
	CFrameConfigMgt()
	{
	}

	virtual int32_t Init();
	virtual int32_t Uninit();
	virtual int32_t GetSize();

	void RegistConfig(const char *szConfigName, IConfig *pConfig);

	IConfig *GetConfig(const char *szConfigName);

	IConfig *GetSafeConfig(const char *szConfigName);

protected:
	ConfigMgt		m_stConfigMgt;
	ConfigMgt		m_stSafeConfigMgt;
};

FRAME_NAMESPACE_END

#endif /* FRAME_CONFIGMGT_H_ */
