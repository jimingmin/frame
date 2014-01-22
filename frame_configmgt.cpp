/*
 * frame_configmgt.cpp
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#include "../logger/logger_mgt.h"
#include "frame_configmgt.h"

using namespace LOGGER;

FRAME_NAMESPACE_BEGIN

int32_t CFrameConfigMgt::Init()
{
	CLogger *pLogger = g_LoggerMgt.GetLogger("gateway");

	for(int32_t i = 0; i < m_nConfigCount; ++i)
	{
		int32_t nRet = m_arrConfigInfo[i].m_pConfig->Init();
		if(nRet != 0)
		{
			pLogger->Warn("init %s error!\n", m_arrConfigInfo[i].m_strConfigName.c_str());
			return 1;
		}
	}

	return 0;
}

int32_t CFrameConfigMgt::Uninit()
{
	return 0;
}

void CFrameConfigMgt::RegistConfig(const char *szConfigName, IConfig *pConfig)
{
	m_arrConfigInfo[m_nConfigCount].m_pConfig = pConfig;
	m_arrConfigInfo[m_nConfigCount].m_strConfigName = szConfigName;
	++m_nConfigCount;
}

FRAME_NAMESPACE_END

