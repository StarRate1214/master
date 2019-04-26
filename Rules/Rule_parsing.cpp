#include "Rule_parsing.h"

//사용법 SetRule(벡터변수, "룰 path");
void SetRule(std::vector<RuleHeader> &v, std::string rule_fileName)
{
    std::ifstream rule;
    rule.open(rule_fileName.c_str());//파일오픈

    if(!rule)//파일유효성 검사
        std::cout<<"Wrong file " << rule_fileName << std::endl;

    std::string line;
    RuleHeader ruleBook;

    while(getline(rule,line))//rule파일의 한줄마다 line에 저장하고, 라인의 끝이면 빠져나옴
    {//룰북 클래스에 하나하나 넣어줌
        ruleBook.SetAction(h_rule(line));
        ruleBook.SetProtocols(h_rule(line));
        ruleBook.SetSrcAddr(h_rule(line));
        ruleBook.SetSrcPort(h_rule(line));
        ruleBook.SetDirOperator(h_rule(line));
        ruleBook.SetDesAddr(h_rule(line));
        ruleBook.SetDesPort(h_rule(line));
        ruleBook.SetRuleOptions(h_ruleOption(line));
        //v백터에 룰북클래스를 push해줌
        v.push_back(ruleBook);
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