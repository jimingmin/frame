/*
 * frame_macrodefine.h
 *
 *  Created on: Mar 7, 2015
 *      Author: jimm
 */

#ifndef FRAME_MACRODEFINE_H_
#define FRAME_MACRODEFINE_H_

#include "common/common_export.h"
#include "frame_namespace.h"

FRAME_NAMESPACE_BEGIN

extern CFrame &GetFrameInstance();

#define REGIST_CLASS(KIND, KEY_TYPE, VALUE_TYPE)	\
	class EXPORT regist_##KIND	\
	{	\
		public:	\
		regist_##KIND(KEY_TYPE key, VALUE_TYPE value)	\
		{	\
			GetFrameInstance().Regist##KIND(key, value);	\
		}	\
	};

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

#define ON_PROC_PCH_PMH_PMB_PU8_I32(id, ctlhead, msghead, msgbody, obj, msgproc)	\
	g_Frame.GetMsgMap().RegistMsgEntry(id, new ctlhead(), new msghead(), new msgbody(), new obj(), static_cast<i32_pch_pmh_pmb_pu8_i32>(&msgproc));

#define ON_PROC_PCH_PMH_PU8_I32(id, ctlhead, msghead, obj, msgproc)	\
	g_Frame.GetMsgMap().RegistMsgEntry(id, new ctlhead(), new msghead(), new obj(), static_cast<i32_pch_pmh_pu8_i32>(&msgproc));


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

#endif /* FRAME_MACRODEFINE_H_ */
