#pragma once
#include "Rule_header.h"
#include <fstream>
#include <algorithm>
void SetRule(RuleHeader &ruleBook, std::string rule_fileName);//룰설정하는부분
std::string h_rule(std::string &line);//찾는 부분
std::string h_ruleOption(std::string &line);