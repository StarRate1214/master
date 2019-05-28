#include "RuleEngine.h"

int CRuleEngine::Compare(std::vector<CRule> *rules, int ruleNum)
{
  for (ruleNum; ruleNum < rules->size(); ruleNum++)
  {
    if (!CompareHeader(rules->at(ruleNum), this->packet))
    {
      continue;
    }

    if (!CompareOption(rules->at(ruleNum).GetRuleOptions()))
    {
      continue;
    }

    return ruleNum;
  }
  return -1;
}