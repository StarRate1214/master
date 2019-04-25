#include <iostream>
#include <string>

bool FindContent(std::string rule_content, std::string payload) //들어온 content를 룰의 content와 비교함 rule_content는 찾는 문장,  payload는 내용
{
    if (-1 != payload.find(rule_content)) //찾으면 ture 못찾으면 false
    {
        return true;
    }
    else
    {
        return false;
    }
}
