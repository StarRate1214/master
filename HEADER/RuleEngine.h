#pragma once
#include "ids.h"
#include "Packet.h"
#include "Rule.h"
#include "NonPayload.h"
#include "rawpacket.h"
#include "CInherit_CompareHeader.h"

class CRuleEngine : public CNonPayload, public CompareHeader //load, compare
{
private:
    CPacket packet;
    //bool content(std::string content, int semicolon, bool nocase, int depth, int offset, int distance, int within, u_int8_t http_option);
    bool CompareOption(std::vector<SRule_option> options);
    bool pcre(std::string option);

public:
    //void RuleLoad(std::string rule_fileName,std::vector<CRule> rules);
    void PacketLoad(CRawpacket *rwpack);

    int Compare(std::vector<CRule> *rules, int ruleNum = 0);
    inline CPacket &getPacket() { return packet; }
};