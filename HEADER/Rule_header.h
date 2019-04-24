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
    RuleHeader(const RuleHeader &ref);
    RuleHeader &operator=(const RuleHeader &ref);
    inline std::string  GetAction(void){return action;}
    inline std::string  GetProtocols(void){return protocols;}
    inline std::string  GetSrc_addr(void){return src_addr;}
    inline std::string  GetSrc_port(void){return src_port;}
    inline std::string  GetDir_operator(void){return dir_operator;}
    inline std::string  GetDes_addr(void){return des_addr;}
    inline std::string  GetDes_port(void){return des_port;}
    inline std::string  GetRule_options(void){return rule_options;}
    std::string     SetAction(std::string action){}
    std::string     SetProtocols(std::string action){}
    std::string     SetSrc_addr(std::string action){}
    std::string     SetSrc_port(std::string action){}
    std::string     SetDir_operator(std::string action){}
    std::string     SetDes_addr(std::string action){}
    std::string     SetDes_port(std::string action){}
    std::string     SetRule_options(std::string action){}
};
