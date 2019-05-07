#include "RuleEngine.h"

void CRuleEngine::RuleLoad(std::string rule_fileName)
{
    std::ifstream rule;
    rule.open(rule_fileName.c_str());//파일오픈

    if(!rule)//파일유효성 검사
        std::cout<<"Wrong file " << rule_fileName << std::endl;

    std::string line;

    while(getline(rule,line))
    {
        CRule rulebook(line);
        rules.push_back(rulebook);
    }
}

