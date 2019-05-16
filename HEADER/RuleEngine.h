#pragma once
#include "ids.h"
#include "Packet.h"
#include "Rule.h"
#include "NonPayload.h"

class CRuleEngine : public CNonPayload //load, compare
{
private:
    CPacket packet;
    bool content(std::string content, int semicolon, bool nocase, int depth, int offset, int distance, int within, u_int8_t http_option);
    bool pcre(std::string option);
    bool compareHeader(CRule);
    bool CompareOption(std::vector<SRule_option> options);
public:
    void RuleLoad(std::string rule_fileName,std::vector<CRule> rules);
    void PacketLoad(u_int8_t *buff);
    int Compare();
};