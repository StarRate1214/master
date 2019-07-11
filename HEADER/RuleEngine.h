#pragma once
#include "ids.h"
#include "Packet.h"
#include "Rule.h"
#include "NonPayload.h"
#include "rawpacket.h"
#include "CompareHeader.h"
#include "detection_filter.h"

class CRuleEngine : public CNonPayload, public CCompareHeader //load, compare
{
private:
    char * hexcontent;
    CPacket packet;
    int content(std::string cnt, bool nocase, int depth, int offset, int distance, int within, u_int8_t http_option, int prev);
    bool CompareOption(std::vector<SRule_option> options);
    bool pcre(std::string option);
    std::vector<CDetection_filter> d_filter;
public:
    CRuleEngine();
    ~CRuleEngine();
    //void RuleLoad(std::string rule_fileName,std::vector<CRule> rules);
    void PacketLoad(CRawpacket *rwpack);
    int Compare(std::vector<CRule> *rules, int ruleNum = 0);
    inline CPacket &getPacket() { return packet; }
};