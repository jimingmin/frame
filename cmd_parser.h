/*
 * cmd_parser.h
 *
 *  Created on: Apr 29, 2015
 *      Author: jimm
 */

#ifndef CMD_PARSER_H_
#define CMD_PARSER_H_

#include "netevent/net_namespace.h"
#include "netevent/net_impl.h"
#include "netevent/net_packet.h"

#include <string.h>

using namespace NETEVENT;

class CCmdParser : public IPacketParser
{
public:

	virtual ~CCmdParser(){};

	virtual int32_t Init();

	virtual int32_t Uninit();

	virtual int32_t GetSize();

	virtual int32_t Parser(const uint8_t arrInputBuf[], const uint32_t nInputBufSize, uint8_t arrOutputBuf[], int32_t &nOutputBufSize);

};

#endif /* CMD_PARSER_H_ */
