/*
 * frame.h
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#ifndef FRAME_H_
#define FRAME_H_

#include "../common/common_singleton.h"
#include "../common/common_typedef.h"
#include "../common/common_export.h"
#include "../netevent/net_impl.h"
#include "frame_impl.h"
#include "frame_namespace.h"
#include "frame_timermgt.h"
#include "frame_msghandle.h"
#include "frame_msgmap.h"
#include "frame_configmgt.h"
#include "frame_timertask.h"
#include "frame_bankmgt.h"
#include "frame_macrodefine.h"

#include <signal.h>
#include <list>
using namespace std;
using namespace NETEVENT;

FRAME_NAMESPACE_BEGIN

class CFrame
{
public:
	EXPORT CFrame();
	EXPORT virtual ~CFrame();

	EXPORT int32_t Init(const char *szServerName, bool bDaemon = false);

	EXPORT int32_t Uninit();

	EXPORT int32_t Run();

	EXPORT void AddRunner(IRunnable *pRunner);

	EXPORT int32_t CreateTimer(TimerProc Proc, CBaseObject *pTimer, CBaseObject *pTimerData, int64_t nCycleTime, bool bLoop, TimerIndex& timerIndex);

	EXPORT CTimer *GetTimer(TimerIndex nTimerIndex);

	EXPORT int32_t RemoveTimer(TimerIndex nTimerIndex);

	EXPORT void RegistConfig(const char *szConfigName, IConfig *pConfig);

	EXPORT IConfig *GetConfig(const char *szConfigName);

	EXPORT void RegistBank(const char *szBankName, IBank *pBank);

	EXPORT IBank *GetBank(const char *szBankName);
	/*
	 * 返回值:	1:not found msg handler
	 * 			2:decode msg failed
	 */
	EXPORT int32_t FrameCallBack(int32_t nMsgID, ...);

	EXPORT CMsgMapDecl &GetMsgMap();

	EXPORT void Dump(ICtlHead *pCtlHead, IMsgHead *pMsgHead, IMsgBody *pMsgBody, const char *szPrefix = "");

	EXPORT int32_t InitSig();

	EXPORT int32_t SendMsg(IIOSession *pIoSession, IMsgHead *pMsgHead, IMsgBody *pMsgBody);

protected:
	EXPORT int32_t SetDaemon();

protected:
	string					m_strServerName;
	list<IRunnable *>		m_stRunnerList;
	CMsgMapDecl				m_stMsgMap;
	CFrameTimerTask			*m_pTimerTask;
	CFrameConfigMgt			*m_pConfigMgt;
	CTimerMgt				*m_pTimerMgt;
	CFrameBankMgt			*m_pBankMgt;
};

#define g_Frame		CSingleton<CFrame>::GetInstance()

REGIST_CLASS(Config, const char *, IConfig *)
#define REGIST_CONFIG(config_name, config_class)	static regist_Config reg_##config_class(config_name, new config_class(config_name));

REGIST_CLASS(Bank, const char *, IBank *)
#define REGIST_BANK(bank_name, bank_class)		static regist_Bank reg_##bank_class(bank_name, new bank_class());

FRAME_NAMESPACE_END

#endif /* FRAME_H_ */

