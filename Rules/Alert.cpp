#include "Rule_header.h"
#include <time.h>
#include <fstream>

void Alert(RuleHeader &c);
const std::string currentDateTime();

int main(void)
{
    std::ifstream rule;
    rule.open("myrules.rules");

    if(!rule)
        std::cout<<"not file!";
    std::string line;
    getline(rule,line);

    RuleHeader c("1","2","3","4","5","6","7","8"); //test
    c.SetRuleOptions(line);
    std::cout << line << std::endl;
    Alert(c);
    rule.close();
    return 0;
}

void Alert(RuleHeader &c)
{

    std::string msg = c.GetRuleOptions();
    int msg_pt;
    int msg_fpt;
    int msg_lpt;

    msg_pt = msg.find("msg");
    msg_fpt = msg.find('"',msg_pt);
    msg_lpt = msg.find('"',msg_fpt);

    msg = msg.substr(msg_fpt,msg_lpt);
    std::cout << currentDateTime();
    std::cout << msg << std::endl;


}

const std::string currentDateTime()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X",&tstruct);
    return buf;
}