#pragma once
#include "ids.h"
#include "Packet.h"
#include "Rule.h"
#include "NonPayload.h"

class CRuleEngine : public CNonPayload //load, compare
{
private:
    CPacket packet;
    std::vector<CRule> rules;
    bool content(std::string content_options);
    bool pcre();
    bool compareHeader(CRule);
    bool CompareOption(std::vector<SRule_option> options);
public:
    void RuleLoad(std::string rule_fileName);
    void PacketLoad(u_int8_t *buff);
    int Compare();
};