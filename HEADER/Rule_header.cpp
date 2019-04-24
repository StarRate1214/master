#pragma once
#include "Rule_header.h"
#include <iostream>
#include <string>
RuleHeader::RuleHeader()
{

}

RuleHeader::~RuleHeader()
{

}

RuleHeader::RuleHeader(
        std::string action,
        std::string protocols,
        std::string src_addr,
        std::string src_port,
        std::string dir_operator,
        std::string des_addr,
        std::string des_port,
        std::string rule_options)
{
    this->action = action;
    this->protocols = protocols;
    this->src_addr = src_addr;
    this->src_port = src_port;
    this->dir_operator = dir_operator;
    this->des_addr = des_addr;
    this->des_port = des_port;
    this->rule_options = rule_options;
}

RuleHeader::RuleHeader(const RuleHeader &ref)
{
    action = ref.action;
    protocols = ref.protocols;
    src_addr = ref.src_addr;
    src_port = ref.src_port;
    dir_operator = ref.dir_operator;
    des_addr = ref.des_addr;
    des_port = ref.des_port;
    rule_options = ref.rule_options;
}

RuleHeader& RuleHeader::operator=(const RuleHeader &ref)
{
    action = ref.action;
    protocols = ref.protocols;
    src_addr = ref.src_addr;
    src_port = ref.src_port;
    dir_operator = ref.dir_operator;
    des_addr = ref.des_addr;
    des_port = ref.des_port;
    rule_options = ref.rule_options;
}

std::string RuleHeader::SetAction(std::string action)
{
    this->action = action;
}
std::string RuleHeader::SetProtocols(std::string protocols)
{
    this->protocols = protocols;
}
std::string RuleHeader::SetSrcAddr(std::string src_addr)
{
    this->src_addr = src_addr;
}
std::string RuleHeader::SetSrcPort(std::string src_port)
{
    this->src_port = src_port;
}
std::string RuleHeader::SetDirOperator(std::string dir_operator)
{
    this->dir_operator = dir_operator;
}
std::string RuleHeader::SetDesAddr(std::string des_addr)
{
    this->des_addr = des_addr;
}
std::string RuleHeader::SetDesPort(std::string des_port)
{
    this->des_port = des_port;
}
std::string RuleHeader::SetRuleOptions(std::string rule_options)
{
    this->rule_options = rule_options;
}
