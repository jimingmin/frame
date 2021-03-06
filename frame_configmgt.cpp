﻿/*
 * frame_configmgt.cpp
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#include "logger/logger.h"
#include "frame_define.h"
#include "frame_configmgt.h"
#include "frame_safeconfig.h"

using namespace LOGGER;

FRAME_NAMESPACE_BEGIN

int32_t CFrameConfigMgt::Init()
{
	ConfigMgt::iterator it = m_stConfigMgt.begin();
	for(; it != m_stConfigMgt.end(); ++it)
	{
		int32_t nRet = it->second->Init();
		if(nRet != 0)
		{
			WRITE_WARN_LOG(MODULE_NAME, "init config %s error!\n", it->first.c_str());
			return 1;
		}
	}

	return 0;
}

int32_t CFrameConfigMgt::Uninit()
{
	return 0;
}

int32_t CFrameConfigMgt::GetSize()
{
	return sizeof(*this);
}

void CFrameConfigMgt::RegistConfig(const char *szConfigName, IConfig *pConfig)
{
	if(GetConfig(szConfigName) != NULL)
	{
		return;
	}

	if(pConfig->IsSafe())
	{
		m_stSafeConfigMgt[szConfigName] = pConfig;
	}
	else
	{
		m_stConfigMgt[szConfigName] = pConfig;
	}
}

IConfig *CFrameConfigMgt::GetConfig(const char *szConfigName)
{
	ConfigMgt::iterator it = m_stConfigMgt.find(szConfigName);
	if(it != m_stConfigMgt.end())
	{
		return it->second;
	}
	else
	{
		it = m_stSafeConfigMgt.find(szConfigName);
		if(it != m_stSafeConfigMgt.end())
		{
			CFrameSafeConfig<IConfig> *pConfig = (CFrameSafeConfig<IConfig> *)(it->second);
			if(pConfig != NULL)
			{
				return pConfig->GetConfig();
			}
		}
	}

	return NULL;
}

IConfig *CFrameConfigMgt::GetSafeConfig(const char *szConfigName)
{
	ConfigMgt::iterator it = m_stSafeConfigMgt.find(szConfigName);
	if(it != m_stSafeConfigMgt.end())
	{
		return it->second;
	}

	return NULL;
}

FRAME_NAMESPACE_END

