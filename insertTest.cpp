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
sPROTOCOL Protocol_split(std::string proto)
{
    std::vector<std::string> tmp;
    sPROTOCOL ret;
    int lpt = proto.find(' ');
    
    std::string str = proto.substr(0, lpt-1);
    proto.erase(0, lpt + 1);
    std::cout << "debug"<< std::endl;
    if (str == "INSERT")
    {
        ret.order = INSERT;
        boost::split(tmp, proto, boost::is_any_of(", "));
        ret.sig_id = std::atoi((tmp.at(0).substr(tmp.at(0).find('='))).c_str());
        ret.header = tmp.at(1).substr(tmp.at(1).find("=\""), tmp.at(1).size() - 1);
        ret.option = tmp.at(2).substr(tmp.at(2).find("=\""), tmp.at(2).size() - 1);
    }
    else if (str == "UPDATE")
    {
        ret.order = UPDATE;
        boost::split(tmp, proto, boost::is_any_of(", "));
        ret.sig_id = std::atoi((tmp.at(0).substr(tmp.at(0).find('='))).c_str());
        ret.header = tmp.at(1).substr(tmp.at(1).find("=\""), tmp.at(1).size() - 1);
        ret.option = tmp.at(2).substr(tmp.at(2).find("=\""), tmp.at(2).size() - 1);
    }
    else if (str == "DELETE")
    {
        ret.order = DELETE;
        boost::split(tmp, proto, boost::is_any_of(", "));
        ret.sig_id = std::atoi((tmp.at(0).substr(tmp.at(0).find('='))).c_str());
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