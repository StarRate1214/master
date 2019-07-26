#include "RuleEngine.h"

int CRuleEngine::Compare(std::vector<CRule> *rules, CNation *country, int ruleNum)
{
  for (ruleNum; ruleNum < rules->size(); ruleNum++)
  {
    if (!CompareHeader(rules->at(ruleNum), this->packet))
    {
      continue;
    }

    if (!CompareOption(rules->at(ruleNum).GetRuleOptions(), country))
    {
      continue;
    }
    ////////////////여기부터 d_filter씀
    if (rules->at(ruleNum).GetD_filter().timeout)
    {
      time_t timeout = rules->at(ruleNum).GetD_filter().timeout;
      int limit = rules->at(ruleNum).GetD_filter().limit;
      u_int32_t ip_addr;
      if (rules->at(ruleNum).GetD_filter().track == SRC)
        ip_addr = packet.ip.getSrcIP();
      else
        ip_addr = packet.ip.getDstIP();

      u_int32_t sig_id = rules->at(ruleNum).GetSig_id();
      u_int8_t rev = rules->at(ruleNum).GetRev();
      int i;
      for (i = 0; i < d_filter.size(); i++)
      {
        if (d_filter[i].getsig_id() == sig_id && d_filter[i].getip_addr() == ip_addr)
        {
          if (d_filter[i].getrev() != rev)
          {
            d_filter.erase(d_filter.begin() + i);
            i = d_filter.size();
          }
          break;
        }
      }
      if (i == d_filter.size())
      {
        CDetection_filter c(sig_id, rev, limit, timeout, ip_addr);
        d_filter.push_back(c);
      }
      d_filter[i].insertPacket(packet); // 패킷넣는 함수
      d_filter[i].deleteTimeOutPacket();                //<-이라인에 지우는함수

      if (!d_filter[i].isMatched())
        continue;
    }
    //////////////////////////여기까지
    return ruleNum;
  }
  return -1;
}