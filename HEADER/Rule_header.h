#pragma once
#include <iostream>
#include <sys/types.h>
#include <string>

class RulesHeader
{
private:
    std::string action;
    std::string protocols;
    std::string src_addr;
    u_int16_t src_port;
    std::string dir_operator;
    std::string des_addr;
    u_int16_t des_port;
    std::string rule_options;
public:
    RulesHeader(/* args */);
    ~RulesHeader();
    

};
