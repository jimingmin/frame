/*
 * frame_bankmgt.h
 *
 *  Created on: Mar 7, 2015
 *      Author: jimm
 */

#ifndef FRAME_BANKMGT_H_
#define FRAME_BANKMGT_H_

#include "common/common_object.h"
#include "frame_impl.h"
#include "frame_namespace.h"
#include <map>
#include <string>
using namespace std;

FRAME_NAMESPACE_BEGIN

class CFrameBankMgt : public CBaseObject
{
	typedef map<string, IBank *>	BankMgt;
public:
	CFrameBankMgt()
	{
	}

	virtual int32_t Init();
	virtual int32_t Uninit();
	virtual int32_t GetSize();

	void RegistBank(const char *szBankName, IBank *pBank);

	IBank *GetBank(const char *szBankName);
protected:
	BankMgt			m_stBankMgt;
};

FRAME_NAMESPACE_END

#endif /* FRAME_BANKMGT_H_ */
