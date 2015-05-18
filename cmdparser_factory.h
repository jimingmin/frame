/*
 * cmdparser_factory.h
 *
 *  Created on: Apr 29, 2015
 *      Author: jimm
 */

#ifndef CMDPARSER_FACTORY_H_
#define CMDPARSER_FACTORY_H_

#include "netevent/net_namespace.h"
#include "netevent/net_impl.h"

using namespace NETEVENT;

class CCmdParserFactory : public IPacketParserFactory
{
public:
	CCmdParserFactory();

	virtual IPacketParser *Create();

	virtual void Destory(IPacketParser *pParser);

};

#endif /* CMDPARSER_FACTORY_H_ */
