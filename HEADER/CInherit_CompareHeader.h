#pragma once
#include "ids.h"
#include "Packet.h"
#include "Rule.h"


class CInherit_CompareHeader //load, compare
{
    CPacket packet;
protected:
    bool CompareHeader(CRule rule);
};