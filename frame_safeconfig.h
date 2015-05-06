/*
 * frame_safeconfig.h
 *
 *  Created on: May 5, 2015
 *      Author: jimm
 */

#ifndef FRAME_SAFECONFIG_H_
#define FRAME_SAFECONFIG_H_

#include "frame_impl.h"

FRAME_NAMESPACE_BEGIN

template<typename CONFIG>
class CFrameSafeConfig : public IConfig
{
public:
	CFrameSafeConfig(const char *szName)
	{
		m_nCurIndex = 0;
		m_nLoadIndex = 1;

		m_arrConfig[0] = new CONFIG(szName);
		m_arrConfig[1] = new CONFIG(szName);
	}

	virtual ~CFrameSafeConfig()
	{
		m_arrConfig[0]->Uninit();
		delete m_arrConfig[0];

		m_arrConfig[1]->Uninit();
		delete m_arrConfig[1];
	}

	//初始化配置
	virtual int32_t Init()
	{
		return 0;
	}
	//卸载配置
	virtual int32_t Uninit()
	{
		return 0;
	}

	virtual bool IsSafe()
	{
		return true;
	}

	virtual int32_t Parser(char *pXMLString)
	{
		int32_t nRet = m_arrConfig[m_nLoadIndex]->Parser(pXMLString);
		if(nRet == 0)
		{
			int32_t nIndex = m_nCurIndex;
			m_nCurIndex = m_nLoadIndex;
			m_nLoadIndex = nIndex;
		}

		return nRet;
	}

	IConfig *GetConfig()
	{
		return m_arrConfig[m_nCurIndex];
	}

protected:
	IConfig			*m_arrConfig[2];
	int32_t			m_nCurIndex;
	int32_t			m_nLoadIndex;
};

FRAME_NAMESPACE_END

#endif /* FRAME_SAFECONFIG_H_ */
