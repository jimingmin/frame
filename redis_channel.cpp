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
#include "common/common_memmgt.h"
#include "common/common_api.h"
#include "common/common_datetime.h"
#include "cachekey_define.h"

CRedisChannel::CRedisChannel(int32_t nServerID, char *pAddress, uint16_t nPort)
:CRedisRaw(nServerID, pAddress, nPort)
{
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
int32_t CRedisChannel::Del(RedisSession *pSession, const char *szKey)
{
	return SendCommand("DEL", szKey, NULL);
}

//int32_t CRedisChannel::Del(RedisSession *pSession, const char *szFormat, ...)
//{
//	va_list ap;
//	va_start(ap, szFormat);
//	int32_t nStatus = SendCommand("DEL", NULL, pSession, szFormat, ap);
//	va_end(ap);
//
//	return nStatus;
//}

int32_t CRedisChannel::Expire(RedisSession *pSession, const char *szKey, int32_t nTimeout)
{
	return SendCommand("EXPIRE", szKey, pSession, "%d", nTimeout);
}

//---------------------------string--------------------------------------
int32_t CRedisChannel::Get(RedisSession *pSession, const char *szKey)
{
	return SendCommand("GET", szKey, pSession);
}

int32_t CRedisChannel::IncrBy(RedisSession *pSession, const char *szKey, int64_t nIncrement)
{
	return SendCommand("INCRBY", szKey, pSession, "%ld", nIncrement);
}

int32_t CRedisChannel::Incr(RedisSession *pSession, const char *szKey)
{
	return IncrBy(pSession, szKey, 1);
}

int32_t CRedisChannel::Set(RedisSession *pSession, const char *szKey, const char *szValue)
{
	return SendCommand("SET", szKey, pSession, "%s", szValue);
}

int32_t CRedisChannel::Set(RedisSession *pSession, const char *szKey, int64_t nValue)
{
	return SendCommand("SET", szKey, pSession, "%ld", nValue);
}

int32_t CRedisChannel::Set(RedisSession *pSession, const char *szKey, const char *szValue, enum TimeUnit nUnit, int64_t nTime)
{
	char szTimeUnit[8];
	if(nUnit == enmTimeUnit_Second)
	{
		strcpy(szTimeUnit, "EX");
		szTimeUnit[2] = '\0';
	}
	else
	{
		strcpy(szTimeUnit, "PX");
		szTimeUnit[2] = '\0';
	}
	return SendCommand("SET", szKey, pSession, "%s %s %ld", szValue, szTimeUnit, nTime);
}

int32_t CRedisChannel::Set(RedisSession *pSession, const char *szKey, const char *szValue, enum TimeUnit nUnit, int64_t nTime, enum SetType nType)
{
	char szTimeUnit[4];
	if(nUnit == enmTimeUnit_Second)
	{
		strcpy(szTimeUnit, "EX");
		szTimeUnit[2] = '\0';
	}
	else
	{
		strcpy(szTimeUnit, "PX");
		szTimeUnit[2] = '\0';
	}

	char szSetType[4];
	if(nType == enmSetType_XX)
	{
		strcpy(szSetType, "XX");
		szSetType[2] = '\0';
	}
	else
	{
		strcpy(szSetType, "NX");
		szSetType[2] = '\0';
	}
	return SendCommand("SET", szKey, pSession, "%s %s %ld %s", szValue, szTimeUnit, nTime, szSetType);
}

int32_t CRedisChannel::Set(RedisSession *pSession, const char *szKey, const char *szValue, enum SetType nType)
{
	char szSetType[4];
	if(nType == enmSetType_XX)
	{
		strcpy(szSetType, "XX");
		szSetType[2] = '\0';
	}
	else
	{
		strcpy(szSetType, "NX");
		szSetType[2] = '\0';
	}
	return SendCommand("SET", szKey, pSession, "%s %s", szValue, szSetType);
}

int32_t CRedisChannel::SetNX(RedisSession *pSession, const char *szKey, const char *szValue)
{
	return SendCommand("SETNX", szKey, pSession, "%s", szValue);
}

int32_t CRedisChannel::SetNX(RedisSession *pSession, const char *szKey, int64_t nValue)
{
	return SendCommand("SETNX", szKey, pSession, "%ld", nValue);
}

int32_t CRedisChannel::GetSet(RedisSession *pSession, const char *szKey, const char *szValue)
{
	return SendCommand("GETSET", szKey, pSession, "%s", szValue);
}

int32_t CRedisChannel::GetSet(RedisSession *pSession, const char *szKey, int64_t nValue)
{
	return SendCommand("GETSET", szKey, pSession, "%ld", nValue);
}

//---------------------------hash----------------------------------------
int32_t CRedisChannel::HDel(RedisSession *pSession, const char *szKey, const char *szFormat, ...)
{
	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("HDEL", szKey, pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

int32_t CRedisChannel::HMSet(RedisSession *pSession, const char *szKey, const char *szFormat, ...)
{
	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("HMSET", szKey, pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

int32_t CRedisChannel::HMSet(RedisSession *pSession, const char *szKey, int32_t nArgc, const char **Argv, const size_t *ArgvLen)
{
	return SendCommand("HMSET", szKey, pSession, nArgc, Argv, ArgvLen);
}

int32_t CRedisChannel::HMGet(RedisSession *pSession, const char *szKey, const char *szFormat, ...)
{
	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("HMGET", szKey, pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

int32_t CRedisChannel::HIncrBy(RedisSession *pSession, const char *szKey, const char *szField, int64_t nIncr)
{
	return SendCommand("HINCRBY", szKey, pSession, "%s %ld", szField, nIncr);
}

int32_t CRedisChannel::HExists(RedisSession *pSession, const char *szKey, const char *szField)
{
	return SendCommand("HEXISTS", szKey, pSession, "%s", szField);
}

//---------------------------list-----------------------------------------
int32_t CRedisChannel::BLPop(RedisSession *pSession, const char *szKey, int32_t nTimeout)
{
	return SendCommand("BLPOP", szKey, pSession, "%d", nTimeout);
}

int32_t CRedisChannel::RPush(RedisSession *pSession, const char *szKey, const char *pValue, uint16_t nValueLen)
{
	return SendCommand("RPUSH", szKey, pSession, "%b", pValue, (size_t)nValueLen);
}

int32_t CRedisChannel::LPop(RedisSession *pSession, const char *szKey)
{
	return SendCommand("LPOP", szKey, pSession);
}

int32_t CRedisChannel::LPush(RedisSession *pSession, const char *szKey, const char *pValue, uint16_t nValueLen)
{
	return SendCommand("LPUSH", szKey, pSession, "%b", pValue, (size_t)nValueLen);
}

//---------------------------set-----------------------------------------
int32_t CRedisChannel::SAdd(RedisSession *pSession, const char *szKey, const char *szFormat, ...)
{
	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("SADD", szKey, pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

int32_t CRedisChannel::SIsMember(RedisSession *pSession, const char *szKey, int64_t nField)
{
	return SendCommand("SISMEMBER", szKey, pSession, "%ld", nField);
}

int32_t CRedisChannel::SIsMember(RedisSession *pSession, const char *szKey, const char *szField)
{
	return SendCommand("SISMEMBER", szKey, pSession, "%s", szField);
}

int32_t CRedisChannel::SRem(RedisSession *pSession, const char *szKey, const char *szFormat, ...)
{
	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("SREM", szKey, pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

//---------------------------sortedset-------------------------------------
int32_t CRedisChannel::ZAdd(RedisSession *pSession, const char *szKey, const char *szFormat, ...)
{
	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("ZADD", szKey, pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

int32_t CRedisChannel::ZCard(RedisSession *pSession, const char *szKey)
{
	return SendCommand("ZCARD", szKey, pSession);
}

int32_t CRedisChannel::ZCount(RedisSession *pSession, const char *szKey, int32_t nMinIndex, int32_t nMaxIndex)
{
	string strMinIndex = (nMinIndex == -1 ? "-inf" : itoa(nMinIndex));
	string strMaxIndex = (nMaxIndex == -1 ? "+inf" : itoa(nMaxIndex));

	return SendCommand("ZCOUNT", szKey, pSession, "%s %s", strMinIndex.c_str(), strMaxIndex.c_str());
}

int32_t CRedisChannel::ZRem(RedisSession *pSession, const char *szKey, const char *szFormat, ...)
{
	va_list ap;
	va_start(ap, szFormat);
	int32_t nStatus = SendCommand("ZREM", szKey, pSession, szFormat, ap);
	va_end(ap);

	return nStatus;
}

int32_t CRedisChannel::ZRangeByScore(RedisSession *pSession, const char *szKey, int32_t nMinIndex, int32_t nMaxIndex, bool bWithScores, int32_t nOffset, int32_t nCount)
{
	string strMinIndex = (nMinIndex == -1 ? "-inf" : itoa(nMinIndex));
	string strMaxIndex = (nMaxIndex == -1 ? "+inf" : itoa(nMaxIndex));

	int32_t nStatus;
	if(bWithScores)
	{
		if(nCount < 0)
		{
			nStatus = SendCommand("ZRANGEBYSCORE", szKey, pSession, "%s %s %s", strMinIndex.c_str(),
					strMaxIndex.c_str(), "WITHSCORES");
		}
		else
		{
			nStatus = SendCommand("ZRANGEBYSCORE", szKey, pSession, "%s %s %s %s %d %d", strMinIndex.c_str(),
					strMaxIndex.c_str(), "WITHSCORES", "LIMIT", (size_t)nOffset, (size_t)nCount);
		}
	}
	else
	{
		if(nCount < 0)
		{
			nStatus = SendCommand("ZRANGEBYSCORE", szKey, pSession, "%s %s", strMinIndex.c_str(),
					strMaxIndex.c_str());
		}
		else
		{
			nStatus = SendCommand("ZRANGEBYSCORE", szKey, pSession, "%s %s %s %d %d", strMinIndex.c_str(),
					strMaxIndex.c_str(), "LIMIT", (size_t)nOffset, (size_t)nCount);
		}
	}

	return nStatus;
}

int32_t CRedisChannel::ZRank(RedisSession *pSession, const char *szKey, int64_t nField)
{
	return SendCommand("ZRANK", szKey, pSession, "%ld", nField);
}

int32_t CRedisChannel::ZRank(RedisSession *pSession, const char *szKey, const char *szField)
{
	return SendCommand("ZRANK", szKey, pSession, "%s", szField);
}

int32_t CRedisChannel::ZRemRangeByRank(RedisSession *pSession, const char *szKey, int32_t nStartIndex, int32_t nStopIndex)
{
	return SendCommand("ZREMRANGEBYRANK", szKey, pSession, "%d %d", nStartIndex, nStopIndex);
}

int32_t CRedisChannel::ZScore(RedisSession *pSession, const char *szKey, int64_t nField)
{
	return SendCommand("ZSCORE", szKey, pSession, "%ld", nField);
}

int32_t CRedisChannel::ZScore(RedisSession *pSession, const char *szKey, const char *szField)
{
	return SendCommand("ZSCORE", szKey, pSession, "%s", szField);
}

//----------------------------sub/pub--------------------------------------
int32_t CRedisChannel::Subscribe(RedisSession *pSession, const char *szKey)
{
	return SendCommand("SUBSCRIBE", szKey, pSession);
}

int32_t CRedisChannel::Unsubscribe(RedisSession *pSession, const char *szKey)
{
	return SendCommand("UNSUBSCRIBE", szKey, pSession);
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

int32_t CRedisChannel::Publish(RedisSession *pSession, const char *szKey, const char *pValue, int32_t nValueLen)
{
	return SendCommand("PUBLISH", szKey, pSession, "%b", pValue, (size_t)nValueLen);
}

