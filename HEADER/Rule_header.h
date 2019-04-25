#pragma once
#include <string>
#include <iostream>

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
    inline std::string GetAction(void) { return action; }
    inline std::string GetProtocols(void) { return protocols; }
    inline std::string GetSrcAddr(void) { return src_addr; }
    inline std::string GetSrcPort(void) { return src_port; }
    inline std::string GetDirOperator(void) { return dir_operator; }
    inline std::string GetDesAddr(void) { return des_addr; }
    inline std::string GetDesPort(void) { return des_port; }
    inline std::string GetRuleOptions(void) { return rule_options; }
    void SetAction(std::string action);
    void SetProtocols(std::string protocols);
    void SetSrcAddr(std::string src_addr);
    void SetSrcPort(std::string src_port);
    void SetDirOperator(std::string dir_operator);
    void SetDesAddr(std::string des_addr);
    void SetDesPort(std::string des_port);
    void SetRuleOptions(std::string rule_options);
};
