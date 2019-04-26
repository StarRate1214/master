//#include "Rule_header.h"
#include<iostream>
#include<string>
#include <regex>

bool pcre(std::string str, std::string regstr)
{

    std::regex reg(regstr);
    if(std::regex_match(str,reg))
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

int main()
{
    if(pcre("hello=3","([a-z])=([0-9])"))
        std::cout<<"find"<<std::endl;
    else
        std::cout<<"Not Find!"<<std::endl;
        
    return 0;
}
