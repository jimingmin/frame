/*
 * cmdparser_factory.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: jimm
 */

#include "cmdparser_factory.h"
#include "cmd_parser.h"
#include "common/common_memmgt.h"

CCmdParserFactory::CCmdParserFactory()
{
}

IPacketParser *CCmdParserFactory::Create()
{
	return NEW(CCmdParser);
}

void CCmdParserFactory::Destory(IPacketParser *pParser)
{
	DELETE(pParser);
}


