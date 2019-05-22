#pragma once
#include "ids.h"
#include "Packet.h"
#include "Rule.h"


class CInherit_CompareHeader //load, compare
{
    CPacket packet;
    bool PortCompare(std::vector<u_int16_t> rulePort, u_int16_t packetPort);
protected:
    bool CompareHeader(CRule rule);
    bool CompareDirection(CRule rule, u_int32_t packetNetmaskA, u_int16_t packetPortA, u_int32_t packetNetmaskB, u_int16_t packetPortB);
};