#include "RuleEngine.h"

int CRuleEngine::Compare(std::vector<CRule> rules)
{
  for (unsigned int i = 0; i < rules.size(); ++i)
  {
      if (!CompareHeader(rules[i]))
        continue;
      
      if(!CompareOption(rules[i].GetRuleOptions()))
        continue;
    
    return i;
  }


  return -1;
}