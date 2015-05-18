/*
 * cachechar_define.h
 *
 *  Created on: Mar 9, 2015
 *      Author: jimm
 */

#ifndef CACHEKEY_DEFINE_H_
#define CACHEKEY_DEFINE_H_

#include <string.h>
#include <map>
#include <string>
#include "frame_impl.h"
using namespace FRAME;

//======================================
//list
class ClientResp
{
public:
	static const char *servername;
	static const char *keyname;
};

//list
class AccountPool
{
public:
	static const char *servername;
	static const char *keyname;
};

//string
class AccountID
{
public:
	static const char *servername;
	static const char *keyname;
	static const char *accountname;
};

//list
class PushApns
{
public:
	static const char *servername;
	static const char *keyname;
};

//list
class PushSms
{
public:
	static const char *servername;
	static const char *keyname;
};

//sortedset
class UserLookMe
{
public:
	static const char *servername;
	static const char *keyname;
};

//hash
class RegistPhoneInfo
{
public:
	static const char *servername;
	static const char *keyname;
	static const char *regist_count;
	static const char *last_regist_date;
	static const char *auth_code;
	static const char *auth_code_expire_time;
};

//hash
class RegistAddrInfo
{
public:
	static const char *servername;
	static const char *keyname;
	static const char *regist_count;
	static const char *last_regist_date;
};

//hash
class ResetPassword
{
public:
	static const char *servername;
	static const char *keyname;
	static const char *reset_count;
	static const char *last_reset_date;
	static const char *auth_code;
	static const char *auth_code_expire_time;
};

//hash
class UserBaseInfo
{
	static std::map<std::string, uint8_t>		m_stFieldPer;
public:
	static bool CanWrite(std::string strField)
	{
		std::map<std::string, uint8_t>::iterator it = m_stFieldPer.find(strField);
		if(it != m_stFieldPer.end())
		{
			if(it->second != 0)
			{
				return true;
			}
		}
		return false;
	}

	//初始化配置
	UserBaseInfo()
	{
		m_stFieldPer[version] = 0;
		m_stFieldPer[accountname] = 0;
		m_stFieldPer[uin] = 0;
		m_stFieldPer[accountid] = 0;
		m_stFieldPer[oneselfwords] = 1;
		m_stFieldPer[gender] = 0;
		m_stFieldPer[school] = 1;
		m_stFieldPer[hometown] = 1;
		m_stFieldPer[birthplace] = 1;
		m_stFieldPer[liveplace] = 1;
		m_stFieldPer[job] = 1;
		m_stFieldPer[height] = 1;
		m_stFieldPer[weight] = 1;
		m_stFieldPer[nickname] = 1;
		m_stFieldPer[headimage] = 1;
		m_stFieldPer[photowall] = 1;
		m_stFieldPer[createtime] = 0;
		m_stFieldPer[birthday] = 1;
		m_stFieldPer[version] = 1;
		m_stFieldPer[createtopics_count] = 1;
		m_stFieldPer[jointopics_count] = 1;
		m_stFieldPer[phonetype] = 1;
		m_stFieldPer[osversion] = 1;
		m_stFieldPer[phonestyle] = 1;
		m_stFieldPer[lastlogintime] = 1;
		m_stFieldPer[devicetoken] = 1;
	}

	static const char *servername;
	static const char *keyname;
	static const char *version;
	static const char *accountname;
	static const char *uin;
	static const char *accountid;
	static const char *nickname;
	static const char *oneselfwords;
	static const char *gender;
	static const char *school;
	static const char *hometown;
	static const char *birthplace;
	static const char *liveplace;
	static const char *job;
	static const char *height;
	static const char *weight;
	static const char *headimage;
	static const char *photowall;
	static const char *createtime;
	static const char *birthday;
	static const char *age;
	static const char *createtopics_count;
	static const char *jointopics_count;
	static const char *phonetype;
	static const char *osversion;
	static const char *phonestyle;
	static const char *lastlogintime;
	static const char *devicetoken;
};

//hash
class AccountInfo
{
public:
	static const char *servername;
	static const char *keyname;
	static const char *accountname;
	static const char *uin;
	static const char *accountid;
	static const char *password;
	static const char *status;
};

//sortedset
class UserBlackList
{
public:
	static const char *servername;
	static const char *keyname;
	static const char *uin;
};

//hash
class UserSessionInfo
{
public:
	static const char *servername;
	static const char *keyname;
	static const char *sessionid;
	static const char *clientaddress;
	static const char *clientport;
	static const char *gateid;
	static const char *phonetype;
	static const char *gateredisaddress;
	static const char *gateredisport;
};

//list
class UserUnreadMsgList
{
public:
	static const char *servername;
	static const char *keyname;
	static const char *timestamp;
	static const char *unreadmsg;
};

//sortedset
class UserFans
{
public:
	static const char *servername;
	static const char *keyname;
	static const char *uin;
	static const char *addtime;
};

//sortedset
class UserFollowers
{
public:
	static const char *servername;
	static const char *keyname;
	static const char *uin;
	static const char *addtime;
};

//sortedset
class UserFriends
{
public:
	static const char *servername;
	static const char *keyname;
	static const char *uin;
	static const char *addtime;
};

#endif /* CACHEKEY_DEFINE_H_ */
