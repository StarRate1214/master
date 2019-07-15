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

    return ruleNum;
  }
  return -1;
}