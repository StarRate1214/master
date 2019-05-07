#pragma once
#include "ids.h"
#include "Packet.h"
#include "Rule.h"

class CRuleEngine //load, compare
{
private:
    CPacket packet;
    std::vector<CRule> rules;
    bool content(bool nocase);
    bool pcre();
    bool non_payload();
public:
    void RuleLoad(std::string rule_fileName);
    void PacketLoad(u_int8_t *buff);
    int Compare();
};