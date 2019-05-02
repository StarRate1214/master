#include "Rule.h"

CRule::CRule()
{
    action="\0";
    protocols="\0";
    src_ipOpt=0;
    src_ip=0;
    src_netmask=0;
    src_port=0;
    dir_operator="->";
    des_ipOpt=0;
    des_ip=0;
    des_netmask=0;
    des_portOpt=0;
    des_port=0;
    rule_options="\0";
}

CRule::~CRule()
{
}

CRule::CRule(std::string rule)
{
    action = h_rule(rule);
    protocols = h_rule(rule);
    std::string sIP = h_rule(rule);
    ip_parsing(sIp, src_ipOpt, src_ip, src_netmask);
    std::string sPort = h_rule(rule);
    port_parsing(sPort, src_portOpt, src_port);
    dir_operator = h_rule(rule);
    std::string dIP = h_rule(rule);
    ip_parsing(dIp, des_ipOpt, des_ip, des_netmask);
    std::string dPort = h_rule(rule);
    port_parsing(dPort, des_portOpt, des_port);
    rule_options = h_ruleOption(rule);
}

CRule::CRule(const CRule &ref)
{
    action = ref.action;
    protocols = ref.protocols;
    src_ipOpt = ref.src_ipOpt;
    src_ip = ref.src_ip;
    src_netmask = ref.src_netmask;
    src_portOpt = ref.src_portOpt;
    src_port = ref.src_port;
    dir_operator = ref.dir_operator;
    des_ipOpt = ref.des_ipOpt;
    des_ip = ref.des_ip;des
    des_netmask = ref.des_netmask;
    des_portOpt = ref.des_portOpt;
    des_port = ref.des_port;
    rule_options = ref.rule_options;
}

CRule &CRule::operator=(const CRule &ref)
{
    action = ref.action;
    protocols = ref.protocols;
    src_ipOpt = ref.src_ipOpt;
    src_ip = ref.src_ip;
    src_netmask = ref.src_netmask;
    src_portOpt = ref.src_portOpt;
    src_port = ref.src_port;
    dir_operator = ref.dir_operator;
    des_ipOpt = ref.des_ipOpt;
    des_ip = ref.des_ip;des
    des_netmask = ref.des_netmask;
    des_portOpt = ref.des_portOpt;
    des_port = ref.des_port;
    rule_options = ref.rule_options;

    return *this;
}

void CRule::SetAction(std::string action)
{
    this->action = action;
}
void CRule::SetProtocols(std::string protocols)
{
    this->protocols = protocols;
}
void CRule::SetSrcIP(std::string src_ip)
{
    ip_parsing(src_ip, src_ipOpt, src_ip, src_netmask);
}
void CRule::SetSrcPort(std::string src_port)
{
    port_parsing(src_port, src_portOpt, src_port);
}
void CRule::SetDirOperator(std::string dir_operator)
{
    this->dir_operator = dir_operator;
}
void CRule::SetDesIP(std::string des_ip)
{
    ip_parsing(des_ip, des_ipOpt, des_ip, des_netmask);
}
void CRule::SetDesPort(std::string des_port)
{
    port_parsing(des_port, des_portOpt, des_port);
}
void CRule::SetRuleOptions(std::string rule_options)
{
    this->rule_options = rule_options;
}

std::string CRule::h_rule(std::string &line) //룰 헤더 찾는부분
{
        std::string ret;

        //0번째부터 공백까지의 글자
        int fpt = 0;
        int lpt = line.find(' ');

        //리턴값 뽑아오기
        ret = line.substr(fpt, lpt);

        //뽑아온 값 지우기
        line.erase(fpt, lpt+1);

        return ret;
}

std::string CRule::h_ruleOption(std::string &line) //룰 옵션 찾는 부분
{
        std::string ret;

        //0번째부터 공백까지의 글자
        int fpt = 0;
        int lpt = line.find(')');

        //괄호 제외된 리턴값 뽑아오기
        ret = line.substr(fpt+1, lpt-1);

        //뽑아온 값 지우기
        line.erase(fpt, lpt);

        return ret;
}

void CRule::ip_parsing(std::string ip, int &ipOpt, u_int32_t &_ip, u_int32_t &netmask)
{
    int mask=ip.find('/');
    ip,netmask = 0;
    if(ip=="any")
        ipOpt=ANY;
    else if(ip[0]=='!')
    {
        ipOpt=NOT;
        if(mask==-1)
        {
            _ip=htonl(stoi(ip.substr(1)));
            netmask=~netmask;
        }
        else
        {
            _ip=htonl(stoi(ip.substr(1,mask-1)));
            mask=stoi(ip.substr(mask+1));
        }        
    }
    else
    {
        ipOpt=COMM;
        ip.substr(0,mask);
        if(mask==-1)
        {
            _ip=htonl(stoi(ip.substr(0)));
            netmask=~netmask;
        }
        else
        {
            _ip=htonl(stoi(ip.substr(0,mask)));
            mask=stoi(ip.substr(mask+1));
        }
    }
}

void CRlue::port_parsing(std::string port, int &portOpt, std::vector<u_int16_t> &_port)
{

}
