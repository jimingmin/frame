/*
 * frame_configmgt.h
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#ifndef FRAME_CONFIGMGT_H_
#define FRAME_CONFIGMGT_H_

#include "../common/common_object.h"
#include "../common/common_singleton.h"
#include "frame_impl.h"
#include "frame_namespace.h"

#include <string>
using namespace std;

FRAME_NAMESPACE_BEGIN

class ConfigInfo
{
public:
	ConfigInfo()
	{
		m_pConfig = NULL;
	}
	string		m_strConfigName;
	IConfig		*m_pConfig;
};

class CFrameConfigMgt : public CObject
{
	enum
	{
		enmMaxConfigInfoSize	= 32,	//最大的配置文件数量
	};
public:
	CFrameConfigMgt()
	{
		m_nConfigCount = 0;
	}

	virtual int32_t Init();
	virtual int32_t Uninit();
	virtual int32_t GetSize();

	void RegistConfig(const char *szConfigName, IConfig *pConfig);

	IConfig *GetConfig(const char *szConfigName);

protected:
	int32_t			m_nConfigCount;
	ConfigInfo		m_arrConfigInfo[enmMaxConfigInfoSize];
};

FRAME_NAMESPACE_END

#endif /* FRAME_CONFIGMGT_H_ */
