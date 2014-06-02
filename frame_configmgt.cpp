/*
 * frame_configmgt.cpp
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#include "../logger/logger.h"
#include "frame_typedef.h"
#include "frame_configmgt.h"

using namespace LOGGER;

FRAME_NAMESPACE_BEGIN

int32_t CFrameConfigMgt::Init()
{
	for(int32_t i = 0; i < m_nConfigCount; ++i)
	{
		int32_t nRet = m_arrConfigInfo[i].m_pConfig->Init();
		if(nRet != 0)
		{
			WRITE_WARN_LOG(MODULE_NAME, "init %s error!\n", m_arrConfigInfo[i].m_strConfigName.c_str());
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

	m_arrConfigInfo[m_nConfigCount].m_pConfig = pConfig;
	m_arrConfigInfo[m_nConfigCount].m_strConfigName = szConfigName;
	++m_nConfigCount;
}

IConfig *CFrameConfigMgt::GetConfig(const char *szConfigName)
{
	for(int32_t i = 0; i < m_nConfigCount; ++i)
	{
		if(strncmp(szConfigName, m_arrConfigInfo[i].m_strConfigName.c_str(), m_arrConfigInfo[i].m_strConfigName.length()) == 0)
		{
			return m_arrConfigInfo[i].m_pConfig;
		}
	}

	return NULL;
}

FRAME_NAMESPACE_END

