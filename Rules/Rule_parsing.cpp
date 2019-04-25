#include "Rule_parsing.h"

void SetRule(RuleHeader &ruleBook, std::string rule_fileName)
{
    std::ifstream rule;
    rule.open(rule_fileName.c_str());

    if(!rule)
        std::cout<<"Wrong file " << rule_fileName << std::endl;

    std::string line;
    while(getline(rule,line))
    {
        ruleBook.SetAction(h_rule(line));
        ruleBook.SetProtocols(h_rule(line));
        ruleBook.SetSrcAddr(h_rule(line));
        ruleBook.SetSrcPort(h_rule(line));
        ruleBook.SetDirOperator(h_rule(line));
        ruleBook.SetDesAddr(h_rule(line));
        ruleBook.SetDesPort(h_rule(line));
        ruleBook.SetRuleOptions(h_ruleOption(line));
    }
}

std::string h_rule(std::string &line) //룰 헤더 찾는부분
{
        std::string ret;

        //0번째부터 공백까지의 글자
        int fpt = 0;
        int lpt = line.find(' ');

        //리턴값 뽑아오기
        ret = line.substr(fpt, lpt);

        //뽑아온 값 지우기
        line.erase(fpt, lpt+1);

        return ret;
}
std::string h_ruleOption(std::string &line) //룰 옵션 찾는 부분
{
        std::string ret;

        //0번째부터 공백까지의 글자
        int fpt = 0;
        int lpt = line.find(')');

        //리턴값 뽑아오기
        ret = line.substr(fpt, lpt+1);

        //뽑아온 값 지우기
        line.erase(fpt, lpt);

        return ret;
}

int main()
{
    RuleHeader cTest;
    SetRule(cTest,"myrules.rules");

    std::cout<<cTest.GetAction()<<std::endl;
    std::cout<<cTest.GetProtocols()<<std::endl;
    std::cout<<cTest.GetSrcAddr()<<std::endl;
    std::cout<<cTest.GetSrcPort()<<std::endl;
    std::cout<<cTest.GetDirOperator()<<std::endl;
    std::cout<<cTest.GetDesAddr()<<std::endl;
    std::cout<<cTest.GetDesPort()<<std::endl;
    std::cout<<cTest.GetRuleOptions()<<std::endl;
}