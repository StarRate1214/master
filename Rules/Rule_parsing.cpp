#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../HEADER/Rule_header.h"

RuleHeader RuleParsing(std::ifstream rules)
{
    std::string line;
    if (!rules)
    {
        std::cout << "fail to open [" << rules << "] file " << std::endl;
        std::cout << "Re Input Plz" << std::endl;
        return;
    }
    while (getline(rules, line))
    {
        
    }
}
