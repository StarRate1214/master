#include "RuleEngine.h"

int CRuleEngine::Compare()
{
  for (unsigned i = 0; i < rules.size(); ++i)
  {
      if (!compareHeader(rules[i]))
        continue;
      
      if(!CompareOption(rules[i].GetRuleOptions))
        continue;
    
    return i;
  }
  return -1;
}