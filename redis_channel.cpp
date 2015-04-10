/*
 * redis_channel.cpp
 *
 *  Created on: Mar 11, 2015
 *      Author: jimm
 */

#include "redis_channel.h"
#include "frame.h"
#include "redissession_bank.h"
#include "sessionindex_bank.h"
#include "../common/common_memmgt.h"
#include "../common/common_api.h"
#include "../common/common_datetime.h"
#include "../include/cachekey_define.h"

CRedisChannel::CRedisChannel(int32_t nServerID, char *pAddress, uint16_t nPort, char *pChannelKey)
:CRedisRaw(nServerID, pAddress, nPort)
{
	m_strChannelKey = string(pChannelKey);
}

int32_t CRedisChannel::OnConnected()
{
	return 0;
}

void CRedisChannel::OnClosed()
{

}

//---------------------------transaction--------------------------------
void CRedisChannel::Multi()
{
	char *pCmd = "*1\r\n$5\r\nMULTI\r\n";
	SendFormatedCommand(NULL, pCmd, strlen(pCmd));
}

void CRedisChannel::Exec(RedisSession *pSession)
{
	char *pCmd = "*1\r\n$4\r\nEXEC\r\n";
	SendFormatedCommand(pSession, pCmd, strlen(pCmd));
}

//---------------------------key-----------------------------------------
int32_t CRedisChannel::Del(RedisSession *pSession, char *szTarget)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	return SendCommand("DEL", (char *)strKey.c_str(), NULL);
}

int32_t CRedisChannel::Del(RedisSession *pSession, const char *szFormat, ...)
{
	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("DEL", (char *)m_strChannelKey.c_str(), pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

//---------------------------string--------------------------------------
int32_t CRedisChannel::Get(RedisSession *pSession, char *szKey)
{
	return SendCommand("GET", szKey, pSession);
}

int32_t CRedisChannel::IncrBy(RedisSession *pSession, char *szTarget, int64_t nIncrement)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	return SendCommand("INCRBY", (char *)strKey.c_str(), pSession, "%ld", nIncrement);
}

int32_t CRedisChannel::Incr(RedisSession *pSession, char *szTarget)
{
	return IncrBy(pSession, szTarget, 1);
}

int32_t CRedisChannel::SetNX(RedisSession *pSession, char *szKey, char *szValue)
{
	return SendCommand("SETNX", szKey, pSession, "%s", szValue);
}

int32_t CRedisChannel::SetNX(RedisSession *pSession, char *szKey, int64_t nValue)
{
	return SendCommand("SETNX", szKey, pSession, "%ld", nValue);
}

int32_t CRedisChannel::GetSet(RedisSession *pSession, char *szKey, char *szValue)
{
	return SendCommand("GETSET", szKey, pSession, "%s", szValue);
}

int32_t CRedisChannel::GetSet(RedisSession *pSession, char *szKey, int64_t nValue)
{
	return SendCommand("GETSET", szKey, pSession, "%ld", nValue);
}

//---------------------------hash----------------------------------------
int32_t CRedisChannel::HDel(RedisSession *pSession, char *szTarget, const char *szFormat, ...)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("HDEL", (char *)strKey.c_str(), pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

int32_t CRedisChannel::HMSet(RedisSession *pSession, char *szTarget, const char *szFormat, ...)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("HMSET", (char *)strKey.c_str(), pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

int32_t CRedisChannel::HMSet(RedisSession *pSession, char *szTarget, int32_t nArgc, const char **Argv, const size_t *ArgvLen)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	return SendCommand("HMSET", (char *)strKey.c_str(), pSession, nArgc, Argv, ArgvLen);
}

int32_t CRedisChannel::HMGet(RedisSession *pSession, char *szTarget, const char *szFormat, ...)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("HMGET", (char *)strKey.c_str(), pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

int32_t CRedisChannel::HIncrBy(RedisSession *pSession, char *szTarget, const char *szFormat, ...)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("HINCRBY", (char *)strKey.c_str(), pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

int32_t CRedisChannel::HExists(RedisSession *pSession, char *szTarget, const char *szFormat, ...)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("HEXISTS", (char *)strKey.c_str(), pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

//---------------------------list-----------------------------------------
int32_t CRedisChannel::BLPop(RedisSession *pSession, int32_t nTimeout)
{
	return SendCommand("BLPOP", (char *)m_strChannelKey.c_str(), pSession, "%d", nTimeout);
}

int32_t CRedisChannel::RPush(RedisSession *pSession, char *pValue, uint16_t nValueLen)
{
	return SendCommand("RPUSH", (char *)m_strChannelKey.c_str(), pSession, "%b", pValue, (size_t)nValueLen);
}

int32_t CRedisChannel::LPop(RedisSession *pSession)
{
	return SendCommand("LPOP", (char *)m_strChannelKey.c_str(), pSession);
}

//---------------------------set-----------------------------------------
int32_t CRedisChannel::SAdd(RedisSession *pSession, char *szTarget, const char *szFormat, ...)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("SADD", (char *)strKey.c_str(), pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

int32_t CRedisChannel::SRem(RedisSession *pSession, char *szTarget, const char *szFormat, ...)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("SREM", (char *)strKey.c_str(), pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

//---------------------------sortedset-------------------------------------
int32_t CRedisChannel::ZAdd(RedisSession *pSession, char *szTarget, const char *szFormat, ...)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("ZADD", (char *)strKey.c_str(), pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

int32_t CRedisChannel::ZCount(RedisSession *pSession, char *szTarget, int32_t nMinIndex, int32_t nMaxIndex)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	string strMinIndex = (nMinIndex == -1 ? "-inf" : itoa(nMinIndex));
	string strMaxIndex = (nMaxIndex == -1 ? "+inf" : itoa(nMaxIndex));

	return SendCommand("ZCOUNT", (char *)strKey.c_str(), pSession, "%s %s", strMinIndex.c_str(), strMaxIndex.c_str());
}

int32_t CRedisChannel::ZRem(RedisSession *pSession, char *szTarget, const char *szFormat, ...)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("ZREM", (char *)strKey.c_str(), pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

int32_t CRedisChannel::ZRangeByScore(RedisSession *pSession, char *szTarget, int32_t nMinIndex, int32_t nMaxIndex, bool bWithScores, int32_t nOffset, int32_t nCount)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	string strMinIndex = (nMinIndex == -1 ? "-inf" : itoa(nMinIndex));
	string strMaxIndex = (nMaxIndex == -1 ? "+inf" : itoa(nMaxIndex));

	int32_t nStatus;
	if(bWithScores)
	{
		if(nCount < 0)
		{
			nStatus = SendCommand("ZRANGEBYSCORE", (char *)strKey.c_str(), pSession, "%s %s %s", strMinIndex.c_str(),
					strMaxIndex.c_str(), "WITHSCORES");
		}
		else
		{
			nStatus = SendCommand("ZRANGEBYSCORE", (char *)strKey.c_str(), pSession, "%s %s %s %s %d %d", strMinIndex.c_str(),
					strMaxIndex.c_str(), "WITHSCORES", "LIMIT", (size_t)nOffset, (size_t)nCount);
		}
	}
	else
	{
		if(nCount < 0)
		{
			nStatus = SendCommand("ZRANGEBYSCORE", (char *)strKey.c_str(), pSession, "%s %s", strMinIndex.c_str(),
					strMaxIndex.c_str());
		}
		else
		{
			nStatus = SendCommand("ZRANGEBYSCORE", (char *)strKey.c_str(), pSession, "%s %s %s %d %d", strMinIndex.c_str(),
					strMaxIndex.c_str(), "LIMIT", (size_t)nOffset, (size_t)nCount);
		}
	}

	return nStatus;
}


int32_t CRedisChannel::ZRemRangeByRank(RedisSession *pSession, char *szTarget, int32_t nStartIndex, int32_t nStopIndex)
{
	string strKey = m_strChannelKey;
	if(szTarget != NULL)
	{
		strKey += szTarget;
	}

	return SendCommand("ZREMRANGEBYRANK", (char *)strKey.c_str(), pSession, "%d %d", nStartIndex, nStopIndex);
}

//----------------------------sub/pub--------------------------------------
int32_t CRedisChannel::Subscribe(RedisSession *pSession)
{
	return SendCommand("SUBSCRIBE", (char *)m_strChannelKey.c_str(), pSession);
}

int32_t CRedisChannel::Unsubscribe(RedisSession *pSession)
{
	return SendCommand("UNSUBSCRIBE", (char *)m_strChannelKey.c_str(), pSession);
}

void CRedisChannel::OnUnsubscribeReply(const redisAsyncContext *pContext, void *pReply, void *pSessionIndex)
{
	if(pSessionIndex == NULL)
	{
		return;
	}

	CSessionIndexBank *pSessionIndexBank = (CSessionIndexBank *)g_Frame.GetBank(BANK_SESSION_INDEX);
	RedisSession *pRedisSession = ((CRedisSessionBank *)g_Frame.GetBank(BANK_REDIS_SESSION))->GetSession(*((uint32_t *)pSessionIndex));
	if(pRedisSession == NULL)
	{
		pSessionIndexBank->DestroySessionIndex((uint32_t *)pSessionIndex);
		return;
	}

	delete pRedisSession->GetHandlerObj();
	delete pRedisSession;
}

int32_t CRedisChannel::Publish(RedisSession *pSession, char *pValue, int32_t nValueLen)
{
	return SendCommand("PUBLISH", (char *)m_strChannelKey.c_str(), pSession, "%b", pValue, (size_t)nValueLen);
}

