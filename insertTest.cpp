#include "RuleEngine.h"
#include "Capture.h"
#include <queue>
#include <libconfig.h++>
#include <boost/algorithm/string.hpp>
typedef struct protopaser
{
    int order;
    int sig_id;
    std::string header;
    std::string option;
} sPROTOCOL;

sPROTOCOL Protocol_split(std::string proto);

void Pinsert(std::vector<CRule> *rules, sPROTOCOL sp);
void Pdelete(std::vector<CRule> *rules, sPROTOCOL sp);

int main()
{
    //TODO rules에 몇개 넣어넣고 테스트해보자(insert와 delete)
    std::vector<CRule> rules;

    std::string proto = "INSERT sig_id=777, header=\"alert tcp any any -> any any\", option=\"content:\"HTTP\";";
    sPROTOCOL s = Protocol_split(proto);

    Pinsert(&rules, s);
    for (int i = 0; i < rules.size(); i++)
    {
        std::cout << rules.at(i).GetSig_id() << std::endl;
        std::cout << rules.at(i).GetDirOperator() << std::endl;
        std::cout << rules.at(i).GetRuleOptions().at(0).option << std::endl;
    }

    /*
    std::cout<<"라인구분선"<<std::endl;

    Pdelete(&rules,s);
    for(int i=0; i<rules.size();i++)
        std::cout<<rules.at(i).GetSig_id()<<std::endl;
        */
}
sPROTOCOL Protocol_split(char *proto)
{
  //INSERT sig_id=777, header=alert tcp any any -> any any, option=content:\"HTTP\";
    //DELETE sig_id=1\n
    sPROTOCOL ret;
    char *ret_ptr;
    char *next_ptr;
    char *value;
    char *value1;
    char *value2;
    char *str = strtok_r(proto, " ", &next_ptr);

    if (str[0] == 'I') //INSERT
    {
        ret.order = INSERT;
        ret_ptr = strtok_r(NULL, ", h", &next_ptr);
        strtok_r(ret_ptr, "=", &value);
        ret.sig_id = atoi(value);

        ret_ptr = strtok_r(NULL, ", o", &next_ptr);
        strtok_r(ret_ptr, "=", &value1);
        ret.header = value1;

        strtok_r(NULL, "n=", &next_ptr);
        ret.option = next_ptr;

        std::cout << ret.order << std::endl;
        std::cout << ret.sig_id << std::endl;
        std::cout << ret.header << std::endl;
        std::cout << ret.option << std::endl;
    }
    else if (str[0] == 'U') //UPDATE
    {
        ret.order = UPDATE;
    }
    else if (str[0] == 'D') //DELETE
    {
        ret.order = DELETE;
    }

    return ret;
}

void Pinsert(std::vector<CRule> *rules, sPROTOCOL sp)
{
    CRule tmp(sp.sig_id, 1, sp.header, sp.option);
    rules->push_back(tmp);
}

void Pdelete(std::vector<CRule> *rules, sPROTOCOL sp)
{
    for (int i = 0; i < rules->size(); i++)
    {
        if (rules->at(i).GetSig_id() == sp.sig_id)
        {
            rules->erase(rules->begin() + i);
        }
    }
}