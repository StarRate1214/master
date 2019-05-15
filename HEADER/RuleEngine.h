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
    bool content(std::string content, int semicolon, bool nocase, int depth, int offset, int distance, int within, u_int8_t http_option);
    bool pcre(std::string option);
    bool compareHeader(CRule);
    bool CompareOption(std::vector<SRule_option> options);
public:
    void RuleLoad(std::string rule_fileName);
    void PacketLoad(u_int8_t *buff);
    int Compare();
};