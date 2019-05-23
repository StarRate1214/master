#include "RuleEngine.h"

int CRuleEngine::Compare(std::vector<CRule> *rules, int ruleNum)
{
  for (ruleNum; ruleNum < rules->size(); ruleNum++)
  {
<<<<<<< HEAD
      if (!CompareHeader(rules->at(ruleNum)))
=======
      if (!compareHeader(rules[i]))
>>>>>>> origin/StarRate1214_Master
        continue;
      
      if(!CompareOption(rules->at(ruleNum).GetRuleOptions()))
        continue;
    
<<<<<<< HEAD
    return ruleNum;
=======
    return i;
>>>>>>> origin/StarRate1214_Master
  }
  return -1;
}