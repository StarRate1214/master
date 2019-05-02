#pragma once
#include "Packet.h"
#include "Rule.h"
#include <vector>
#include<string>
class CRuleEngine //load, compare
{
private:
    CPacket packet;
    std::vector<CRule> rules;
    bool content(bool nocase);
    bool pcre();
    bool non_payload();
public:
    void RuleLoad();
    void PacketLoad();
    CRule Compare();
};