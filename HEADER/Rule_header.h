#pragma once
#include <iostream>
#include <string>

class RuleHeader
{
private:
    std::string action;
    std::string protocols;
    std::string src_addr;
    std::string src_port;
    std::string dir_operator;
    std::string des_addr;
    std::string des_port;
    std::string rule_options;
public:
    RuleHeader();
    ~RuleHeader();
    RuleHeader(
        std::string action,
        std::string protocols,
        std::string src_addr,
        std::string src_port,
        std::string dir_operator,
        std::string des_addr,
        std::string des_port,
        std::string rule_options);
    RuleHeader(const RuleHeader &ref);
    RuleHeader &operator=(const RuleHeader &ref);
    inline std::string  GetAction(void){return action;}
    inline std::string  GetProtocols(void){return protocols;}
    inline std::string  GetSrcAddr(void){return src_addr;}
    inline std::string  GetSrcPort(void){return src_port;}
    inline std::string  GetDirOperator(void){return dir_operator;}
    inline std::string  GetDesAddr(void){return des_addr;}
    inline std::string  GetDesPort(void){return des_port;}
    inline std::string  GetRuleOptions(void){return rule_options;}
    std::string     SetAction(std::string action){}
    std::string     SetProtocols(std::string protocols){}
    std::string     SetSrcAddr(std::string src_addr){}
    std::string     SetSrcPort(std::string src_port){}
    std::string     SetDirOperator(std::string dir_operator){}
    std::string     SetDesAddr(std::string des_addr){}
    std::string     SetDesPort(std::string des_port){}
    std::string     SetRuleOptions(std::string rule_options){}
};
