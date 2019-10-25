#include "RuleEngine.h"
/*
void CRuleEngine::RuleLoad(std::string rule_fileName, std::vector<CRule> rules)
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
*/
CRuleEngine::CRuleEngine()
{
    hexcontent = new char[1];
    c_content = new char[1];
	c_payload = new char[1];
	c_httpmatch = new char[1];
    Py_Initialize();
}

CRuleEngine::~CRuleEngine()
{
    delete[] hexcontent;
    delete[] c_content;
	delete[] c_payload;
	delete[] c_httpmatch;
    Py_FinalizeEx();
}