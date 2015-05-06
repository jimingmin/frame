/*
 * cmd_define.h
 *
 *  Created on: May 5, 2015
 *      Author: jimm
 */

#ifndef CMD_DEFINE_H_
#define CMD_DEFINE_H_

#include "../rapidjson/document.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/writer.h"
#include <string>
using namespace std;
using namespace rapidjson;

static string CmdWrapper(Value &stCmd, Value &stParams, Document::AllocatorType& allocator)
{
	Value stRequest(kObjectType);
	stRequest.AddMember("cmd", stCmd, allocator);
	stRequest.AddMember("params", stParams, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	stRequest.Accept(writer);

	return string(buffer.GetString());
}

class ICmdReq
{
public:
	virtual ~ICmdReq(){}

	virtual string Encode() = 0;
};

class CmdRegistReq : public ICmdReq
{
#define CMD			"regist"
public:

	CmdRegistReq(char *szServerName, int32_t nServerType, int32_t nServerID)
	{
		memset(m_szServerName, 0, sizeof(m_szServerName));
		strcpy(m_szServerName, szServerName);
		m_nServerType = nServerType;
		m_nServerID = nServerID;
	}

	virtual ~CmdRegistReq(){}

	virtual string Encode()
	{
		Document document;
		Document::AllocatorType& allocator = document.GetAllocator();

		Value stCmd(kStringType);
		stCmd.SetString(CMD, strlen(CMD), allocator);

		Value stServerName(kStringType);
		stServerName.SetString(m_szServerName, strlen(m_szServerName), allocator);

		Value stServerType(kNumberType);
		stServerType.SetInt(m_nServerType);

		Value stServerID(kNumberType);
		stServerID.SetInt(m_nServerID);

		Value stParams(kObjectType);
		stParams.AddMember("server_name", stServerName, allocator);
		stParams.AddMember("server_type", stServerType, allocator);
		stParams.AddMember("server_id", stServerID, allocator);

		return CmdWrapper(stCmd, stParams, allocator);
	}

private:
	char		m_szServerName[1024];
	int32_t		m_nServerType;
	int32_t		m_nServerID;
};



#endif /* CMD_DEFINE_H_ */
