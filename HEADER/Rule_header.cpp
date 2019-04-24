#pragma once
#include "Rule_header.h"
#include <iostream>
#include <string>

RuleHeader::RuleHeader(const RuleHeader &ref){
    action = ref.action;
    protocols = ref.protocols;
    src_addr = ref.src_addr;
    src_port = ref.src_port;
    dir_operator = ref.dir_operator;
    des_addr = ref.des_addr;
    des_port = ref.des_port;
    rule_options = ref.rule_options;
}
RuleHeader& RuleHeader::operator=(const RuleHeader &ref){
    action = ref.action;
    protocols = ref.protocols;
    src_addr = ref.src_addr;
    src_port = ref.src_port;
    dir_operator = ref.dir_operator;
    des_addr = ref.des_addr;
    des_port = ref.des_port;
    rule_options = ref.rule_options;
}
std::string RuleHeader::SetAction(std::string action){
    this->action = action;
}
std::string RuleHeader::SetProtocols(std::string protocols){
    this->protocols = protocols;
}
std::string RuleHeader::SetSrc_addr(std::string src_addr){
    this->src_addr = src_addr;
}
std::string RuleHeader::SetSrc_port(std::string src_port){
    this->src_port = src_port;
}
std::string RuleHeader::SetDir_operator(std::string dir_operator){
    this->dir_operator = dir_operator;
}
std::string RuleHeader::SetDes_addr(std::string des_addr){
    this->des_addr = des_addr;
}
std::string RuleHeader::SetDes_port(std::string des_port){
    this->des_port = des_port;
}
std::string RuleHeader::SetRule_options(std::string rule_options){
    this->rule_options = rule_options;
}
