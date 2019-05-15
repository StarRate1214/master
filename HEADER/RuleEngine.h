#pragma once
#include "ids.h"
#include "Packet.h"
#include "Rule.h"
#include "NonPayload.h"
#include "CInherit_CompareHeader.h"

class CRuleEngine : public CNonPayload, public CInherit_CompareHeader //load, compare
{
private:
    CPacket packet;
    std::vector<CRule> rules;
    bool content(bool nocase);
    bool pcre();
    // bool CompareHeader(CRule rule);
public:
    void RuleLoad(std::string rule_fileName);
    void PacketLoad(u_int8_t *buff);
    int Compare();
};