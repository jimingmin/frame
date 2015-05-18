/*
 * frame_bankmgt.cpp
 *
 *  Created on: Mar 7, 2015
 *      Author: jimm
 */

#include "logger/logger.h"
#include "frame_define.h"
#include "frame_bankmgt.h"

using namespace LOGGER;

FRAME_NAMESPACE_BEGIN

int32_t CFrameBankMgt::Init()
{
	BankMgt::iterator it = m_stBankMgt.begin();
	for(; it != m_stBankMgt.end(); ++it)
	{
		int32_t nRet = it->second->Init();
		if(nRet != 0)
		{
			WRITE_WARN_LOG(MODULE_NAME, "init bank %s error!\n", it->first.c_str());
			return 1;
		}
	}

	return 0;
}

int32_t CFrameBankMgt::Uninit()
{
	return 0;
}

int32_t CFrameBankMgt::GetSize()
{
	return sizeof(*this);
}

void CFrameBankMgt::RegistBank(const char *szBankName, IBank *pBank)
{
	if(GetBank(szBankName) != NULL)
	{
		return;
	}

	m_stBankMgt[szBankName] = pBank;
}

IBank *CFrameBankMgt::GetBank(const char *szBankName)
{

	BankMgt::iterator it = m_stBankMgt.find(szBankName);
	if(it != m_stBankMgt.end())
	{
		return it->second;
	}

	return NULL;
}

FRAME_NAMESPACE_END



