#include "../HEADER/RuleEngine.h"

int CRuleEngine::Compare()
{
  for (unsigned int i = 0; i < rules.size(); ++i)
  {
      if (compareHeader(rules[i]))
      {
           return i;
      }
  }


  return -1;
}