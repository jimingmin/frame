/*
 * cachekey_define.cpp
 *
 *  Created on: May 15, 2015
 *      Author: jimm
 */

#include "cachekey_define.h"
#include "frame_impl.h"
#include "frame.h"

//
const char *ClientResp::servername = "queue";
const char *ClientResp::keyname = "client:resp:";

//
const char *AccountPool::servername = "accountpool";
const char *AccountPool::keyname = "account:pool";

//
const char *AccountID::servername = "user:info";
const char *AccountID::keyname = "accountid:";
const char *AccountID::accountname = "accountname";

//
const char *PushApns::servername = "queue";
const char *PushApns::keyname = "push:apns";

//
const char *PushSms::servername = "queue";
const char *PushSms::keyname = "push:sms";

//
const char *PushNoti::servername = "queue";
const char *PushNoti::keyname = "push:noti";

//
const char *UserLookMe::servername = "user:info";
const char *UserLookMe::keyname = "user:lookme:";

//
const char *RegistPhoneInfo::servername = "temp";
const char *RegistPhoneInfo::keyname = "regist:phoneinfo:";
const char *RegistPhoneInfo::regist_count = "regist_count";
const char *RegistPhoneInfo::last_regist_date = "last_regist_date";
const char *RegistPhoneInfo::auth_code = "auth_code";
const char *RegistPhoneInfo::auth_code_expire_time = "auth_code_expire_time";

//
const char *RegistAddrInfo::servername = "temp";
const char *RegistAddrInfo::keyname = "regist:addrinfo:";
const char *RegistAddrInfo::regist_count = "regist_count";
const char *RegistAddrInfo::last_regist_date = "last_regist_date";

//
const char *ResetPassword::servername = "temp";
const char *ResetPassword::keyname = "resetpassword:";
const char *ResetPassword::reset_count = "reset_count";
const char *ResetPassword::last_reset_date = "last_reset_date";
const char *ResetPassword::auth_code = "auth_code";
const char *ResetPassword::auth_code_expire_time = "auth_code_expire_time";

//
std::map<std::string, uint8_t> UserBaseInfo::m_stFieldPer;
const char *UserBaseInfo::servername = "user:info";
const char *UserBaseInfo::keyname = "user:baseinfo:";
const char *UserBaseInfo::version = "version";
const char *UserBaseInfo::accountname = "accountname";
const char *UserBaseInfo::uin = "uin";
const char *UserBaseInfo::accountid = "accountid";
const char *UserBaseInfo::nickname = "nickname";
const char *UserBaseInfo::oneselfwords = "oneselfwords";
const char *UserBaseInfo::gender = "gender";
const char *UserBaseInfo::school = "school";
const char *UserBaseInfo::hometown = "hometown";
const char *UserBaseInfo::birthplace = "birthplace";
const char *UserBaseInfo::liveplace = "liveplace";
const char *UserBaseInfo::job = "job";
const char *UserBaseInfo::height = "height";
const char *UserBaseInfo::weight = "weight";
const char *UserBaseInfo::headimage = "headimage";
const char *UserBaseInfo::photowall = "photowall";
const char *UserBaseInfo::createtime = "createtime";
const char *UserBaseInfo::birthday = "birthday";
const char *UserBaseInfo::age = "age";
const char *UserBaseInfo::createtopics_count = "createtopics_count";
const char *UserBaseInfo::jointopics_count = "jointopics_count";
const char *UserBaseInfo::phonetype = "phonetype";
const char *UserBaseInfo::osversion = "osversion";
const char *UserBaseInfo::phonestyle = "phonestyle";
const char *UserBaseInfo::lastlogintime = "lastlogintime";
const char *UserBaseInfo::devicetoken = "devicetoken";
const char *UserBaseInfo::followbusline_count = "followbusline_count";
const char *UserBaseInfo::radar = "radar";
const char *UserBaseInfo::status = "status";
const char *UserBaseInfo::accounttype = "accounttype";

//
const char *AccountInfo::servername = "user:info";
const char *AccountInfo::keyname = "accountinfo:";
const char *AccountInfo::accountname = "accountname";
const char *AccountInfo::uin = "uin";
const char *AccountInfo::accountid = "accountid";
const char *AccountInfo::password = "password";

//
const char *UserBlackList::servername = "user:social";
const char *UserBlackList::keyname = "user:blacklist:";
const char *UserBlackList::uin = "uin";

//
const char *UserSessionInfo::servername = "user:session";
const char *UserSessionInfo::keyname = "user:sessioninfo:";
const char *UserSessionInfo::sessionid = "sessionid";
const char *UserSessionInfo::clientaddress = "clientaddress";
const char *UserSessionInfo::clientport = "clientport";
const char *UserSessionInfo::gateid = "gateid";
const char *UserSessionInfo::phonetype = "phonetype";
const char *UserSessionInfo::gateredisaddress = "gateredisaddress";
const char *UserSessionInfo::gateredisport = "gateredisport";

//
const char *UserSessionKey::servername = "user:session";
const char *UserSessionKey::keyname = "user:sessionkey:";
const char *UserSessionKey::tokenkey = "tokenkey";
const char *UserSessionKey::datakey = "datakey";
const char *UserSessionKey::rc4key = "rc4key";

//
const char *UserUnreadMsgList::servername = "user:unreadmsg";
const char *UserUnreadMsgList::keyname = "user:unreadmsglist:";
const char *UserUnreadMsgList::timestamp = "timestamp";
const char *UserUnreadMsgList::unreadmsg = "unreadmsg";

//
const char *UserFans::servername = "user:social";
const char *UserFans::keyname = "user:fans:";
const char *UserFans::uin = "uin";
const char *UserFans::addtime = "addtime";

//
const char *UserFollowers::servername = "user:social";
const char *UserFollowers::keyname = "user:followers:";
const char *UserFollowers::uin = "uin";
const char *UserFollowers::addtime = "addtime";

//
const char *UserFriends::servername = "user:social";
const char *UserFriends::keyname = "user:friends:";
const char *UserFriends::uin = "uin";
const char *UserFriends::addtime = "addtime";

class InitCacheKey : public IConfig
{
public:
	InitCacheKey(const char *szConfigName)
	{

	}
	//初始化配置
	virtual int32_t Init()
	{
		static UserBaseInfo stUserBaseInfo;
		return 0;
	}
	//卸载配置
	virtual int32_t Uninit()
	{
		return 0;
	}
};

REGIST_CONFIG("initcachekey", InitCacheKey)



