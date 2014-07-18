/*
 * frame.h
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#ifndef FRAME_H_
#define FRAME_H_

//#include "../common/common_singleton.h"
#include "../common/common_typedef.h"
#include "../common/common_export.h"
#include "frame_impl.h"
#include "frame_namespace.h"
#include "frame_timermgt.h"
#include "frame_msghandle.h"
#include "frame_msgmap.h"
#include "frame_configmgt.h"
#include "frame_timertask.h"

#include <signal.h>
#include <list>
using namespace std;

FRAME_NAMESPACE_BEGIN

class CFrame
{
public:
	EXPORT CFrame();
	EXPORT virtual ~CFrame();

	EXPORT int32_t Init(const char *szServerName);

	EXPORT int32_t Uninit();

	EXPORT int32_t Run();

	EXPORT void AddRunner(IRunnable *pRunner);

	EXPORT int32_t CreateTimer(TimerProc Proc, CBaseObject *pTimer, CBaseObject *pTimerData, int64_t nCycleTime, bool bLoop, TimerIndex& timerIndex);

	EXPORT int32_t RemoveTimer(TimerIndex timerIndex);

	EXPORT void RegistConfig(const char *szConfigName, IConfig *pConfig);

	EXPORT IConfig *GetConfig(const char *szConfigName);
	/*
	 * 返回值:	1:not found msg handler
	 * 			2:decode msg failed
	 */
	EXPORT int32_t FrameCallBack(int32_t nMsgID, ...);

	EXPORT CMsgMapDecl &GetMsgMap();

	EXPORT void Dump(IMsgHead *pMsgHead, IMsgBody *pMsgBody, const char *szPrefix = "");

	EXPORT int32_t InitSig();

protected:
	string					m_strServerName;
	list<IRunnable *>		m_stRunnerList;
	CMsgMapDecl				m_stMsgMap;
	CFrameTimerTask			*m_pTimerTask;
	CFrameConfigMgt			*m_pConfigMgt;
	CTimerMgt				*m_pTimerMgt;
};

//#define g_Frame		CSingleton<CFrame>::GetInstance()
//
//class EXPORT regist
//{
//public:
//	regist(const char *szConfigName, IConfig *pConfig)
//	{
//		g_Frame.RegistConfig(szConfigName, pConfig);
//	}
//};
//
/*#define REGIST_CONFIG(config_name, config_class)	\
	static regist reg_##config_class(config_name, new config_class(config_name))
*/
#define MSGMAP_BEGIN(entity)	\
class CMsgMapDecl_##entity	\
{	\
public:	\
	CMsgMapDecl_##entity()	\
{	\
	DeclMsgMap();	\
}	\
	~CMsgMapDecl_##entity()	\
{	\
}	\
	void DeclMsgMap()	\
{

#define ON_PROC_PMH_PMB(id, msghead, msgbody, obj, msgproc)	\
	g_Frame.GetMsgMap().RegistMsgEntry(id, new msghead(), new msgbody(), new obj(), static_cast<i32_pco_pmh_pmb>(&msgproc));

#define ON_PROC_PMH_PMB_PU8_I32(id, msghead, msgbody, obj, msgproc)	\
	g_Frame.GetMsgMap().RegistMsgEntry(id, new msghead(), new msgbody(), new obj(), static_cast<i32_pco_pmh_pmb_pu8_i32>(&msgproc));

#define ON_PROC_PMH_PU8_I32(id, msghead, obj, msgproc)	\
	g_Frame.GetMsgMap().RegistMsgEntry(id, new msghead(), new obj(), static_cast<i32_pco_pmh_pu8_i32>(&msgproc));

#define ON_STREAMEVENT(id, obj, msgproc)	\
	g_Frame.GetMsgMap().RegistMsgEntry(id, new obj(), static_cast<i32_pco_pu8_i32>(&msgproc));

#define ON_MSG_EVENT(id, msgproc)	\
	g_Frame.GetMsgMap().RegistMsgEntry(id, msgproc);

#define MSGMAP_END(entity)	\
}	\
};	\
	static CMsgMapDecl_##entity l_##entity;


#define DECLARE_MSG_MAP() \
protected:\
	static CMsgMapDecl* GetMyMsgMap(); \
	virtual CMsgMapDecl* GetMsgMap();

#define BEGIN_MSG_MAP(cls)	\
		CMsgMapDecl* cls::GetMsgMap()	\
		{	\
			return GetMyMsgMap();	\
		}	\
		CMsgMapDecl* cls::GetMyMsgMap()	\
		{	\
			static CMsgMapDecl stMsgMap;

//#define ON_MSG_EVENT(id, proc)

#define END_MSG_MAP()	\
			return &stMsgMap;	\
		};

FRAME_NAMESPACE_END

#endif /* FRAME_H_ */

