#include "Rule.h"
/*
CRule::CRule()
{
    action="\0";
    protocols="\0";
    src_ipOpt=0;
    src_ip=0;
    src_netmask=0;
    dir_operator="->";
    des_ipOpt=0;
    des_ip=0;
    des_netmask=0;
    des_portOpt=0;
    rule_options="\0";
}*/

CRule::~CRule()
{
}
/*
CRule::CRule(std::string rule)
{
    sig_id=0;
    action = h_rule(rule);
    std::string tmp = h_rule(rule);
    if((tmp =="TCP") || (tmp=="tcp") )
        protocols = TCP;
    else if((tmp == "UDP") || (tmp=="udp"))
        protocols = UDP;
    else if((tmp == "ICMP")||(tmp=="icmp"))
        protocols = ICMP;
    std::string sIP = h_rule(rule);
    ip_parsing(sIP, src_ipOpt, src_ip, src_netmask);
    std::string sPort = h_rule(rule);
    port_parsing(sPort, src_portOpt, src_port);
    dir_operator = h_rule(rule);
    std::string dIP = h_rule(rule);
    ip_parsing(dIP, des_ipOpt, des_ip, des_netmask);
    std::string dPort = h_rule(rule);
    port_parsing(dPort, des_portOpt, des_port);
    option_parsing(h_ruleOption(rule));    
}*/
CRule::CRule(u_int32_t sig_id, u_int8_t rev, SRule_header rule_header, std::string rule_opt, std::unordered_map<std::string, IP_value> *IP_map, std::unordered_map<std::string, Port_value> *Port_map,bool sig_run)
{
    this->sig_run = sig_run;
    this->sig_id = sig_id;
    this->rev = rev;

    if (rule_header.sig_action == "alert")
        this->action = ALERT;
    else if (rule_header.sig_action == "log")
        this->action = LOG;
    else
        this->action = PASS;

    if ((rule_header.sig_protocol == "TCP") || (rule_header.sig_protocol == "tcp"))
        this->protocols = TCP;
    else if ((rule_header.sig_protocol == "UDP") || (rule_header.sig_protocol == "udp"))
        this->protocols = UDP;
    else if ((rule_header.sig_protocol == "ICMP") || (rule_header.sig_protocol == "icmp"))
        this->protocols = ICMP;
    
    if (rule_header.sig_srcIP[0] == '$')
        this->src_IPvariable = rule_header.sig_srcIP;
    else
        ip_parsing(rule_header.sig_srcIP, src_ipOpt, src_ip, src_netmask);

    if (rule_header.sig_srcPort[0] == '$')
        this->src_portvariable = rule_header.sig_srcPort;
    else
        port_parsing(rule_header.sig_srcPort, src_portOpt, src_port);

    this->dir_operator = rule_header.sig_direction;

    if (rule_header.sig_dstIP[0] == '$')
        this->des_IPvariable = rule_header.sig_dstIP;
    else
        ip_parsing(rule_header.sig_dstIP, des_ipOpt, des_ip, des_netmask);

    if (rule_header.sig_dstPort[0] == '$')
        this->des_portvariable = rule_header.sig_dstPort;
    else
        port_parsing(rule_header.sig_dstPort, des_portOpt, des_port);

    option_parsing(rule_opt);
    this->IP_map=IP_map;
    this->Port_map=Port_map;
}
CRule::CRule(const CRule &ref)
{
    sig_run = ref.sig_run;
    action = ref.action;
    protocols = ref.protocols;
    src_ipOpt = ref.src_ipOpt;
    src_ip = ref.src_ip;
    src_netmask = ref.src_netmask;
    src_portOpt = ref.src_portOpt;
    src_port = ref.src_port;
    dir_operator = ref.dir_operator;
    des_ipOpt = ref.des_ipOpt;
    des_ip = ref.des_ip;
    des_netmask = ref.des_netmask;
    des_portOpt = ref.des_portOpt;
    des_port = ref.des_port;
    rule_options = ref.rule_options;
    sig_id = ref.sig_id;
    d_filter = ref.d_filter;
    src_IPvariable=ref.src_IPvariable;
    src_portvariable=ref.src_portvariable;
    des_IPvariable=ref.des_IPvariable;
    des_portvariable=ref.des_portvariable;
    IP_map=ref.IP_map;
    Port_map=ref.Port_map;
}

CRule &CRule::operator=(const CRule &ref)
{
    sig_run = ref.sig_run;
    action = ref.action;
    protocols = ref.protocols;
    src_ipOpt = ref.src_ipOpt;
    src_ip = ref.src_ip;
    src_netmask = ref.src_netmask;
    src_portOpt = ref.src_portOpt;
    src_port = ref.src_port;
    dir_operator = ref.dir_operator;
    des_ipOpt = ref.des_ipOpt;
    des_ip = ref.des_ip;
    des_netmask = ref.des_netmask;
    des_portOpt = ref.des_portOpt;
    des_port = ref.des_port;
    rule_options = ref.rule_options;
    sig_id = ref.sig_id;
    d_filter = ref.d_filter;
    src_IPvariable=ref.src_IPvariable;
    src_portvariable=ref.src_portvariable;
    des_IPvariable=ref.des_IPvariable;
    des_portvariable=ref.des_portvariable;
    IP_map=ref.IP_map;
    Port_map=ref.Port_map;
    return *this;
}
void CRule::SetSig_run(bool sig_run){
    this->sig_run = sig_run;
}
/*
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
    ip_parsing(src_ip, src_ipOpt, this->src_ip, src_netmask);
}
void CRule::SetSrcPort(std::string src_port)
{
    port_parsing(src_port, src_portOpt, this->src_port);
}
void CRule::SetDirOperator(std::string dir_operator)
{
    this->dir_operator = dir_operator;
}
void CRule::SetDesIP(std::string des_ip)
{
    ip_parsing(des_ip, des_ipOpt, this->des_ip, des_netmask);
}
void CRule::SetDesPort(std::string des_port)
{
    port_parsing(des_port, des_portOpt, this->des_port);
}
void CRule::SetRuleOptions(std::string rule_options)
{
    this->rule_options = rule_options;
}
*/
/*
std::string CRule::h_rule(std::string &line) //룰 헤더 찾는부분
{
    std::string ret;

    //0번째부터 공백까지의 글자
    int fpt = 0;
    int lpt = line.find(' ');

    //리턴값 뽑아오기
    ret = line.substr(fpt, lpt);

    //뽑아온 값 지우기
    line.erase(fpt, lpt + 1);

    return ret;
}
 */
/*
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
*/
void CRule::ip_parsing(std::string ip, int &ipOpt, u_int32_t &_ip, u_int32_t &netmask)
{
    int mask = ip.find('/');
    int nmask = UINT32_MAX;
    std::string tmp;
    if (ip == "any")
    {
        ipOpt = ANY;
        _ip = 0;
        netmask = 0;
    }
    else if (ip[0] == '!')
    {
        ipOpt = NOT;
        if (mask == -1) //!ip
        {
            tmp = ip.substr(1);
            _ip = inet_addr(tmp.c_str());
        }
        else //!ip/prefix
        {
            tmp = ip.substr(1, mask - 1);
            _ip = inet_addr(tmp.c_str());
            mask = std::stoi(ip.substr(mask + 1));
            mask = 32 - mask;
            netmask = htonl(nmask << mask);
        }
    }
    else
    {
        ipOpt = COMM;
        ip.substr(0, mask);
        if (mask == -1) //ip
        {
            tmp = ip.substr(0);
            _ip = inet_addr(tmp.c_str());
            netmask = nmask;
        }
        else //ip/prefix
        {
            tmp = ip.substr(0, mask);
            _ip = inet_addr(tmp.c_str());
            mask = std::stoi(ip.substr(mask + 1));
            mask = 32 - mask;
            netmask = htonl(nmask << mask);
        }
    }
}

void CRule::port_parsing(std::string port, int &portOpt, std::vector<u_int16_t> &_port)
{
    int range = port.find(':');
    if (port == "any")
    {
        portOpt = ANY;
        _port.push_back(0);
    }
    else if (port[0] == '!')
    {
        portOpt = NOT;
        if (range == -1) //!range
        {
            _port.push_back(htons((u_int16_t)std::stoi(port.substr(1))));
        }
        else if (range == 1) //!:range
        {
            _port.push_back(0);
            _port.push_back(htons((u_int16_t)std::stoi(port.substr(2))));
        }
        else if ((range + 1) == port.length()) //!range:
        {
            _port.push_back(htons((u_int16_t)std::stoi(port.substr(1, range - 1))));
            _port.push_back(UINT16_MAX);
        }
        else //!range1:range2
        {
            _port.push_back(htons((u_int16_t)std::stoi(port.substr(1, range - 1))));
            _port.push_back(htons((u_int16_t)std::stoi(port.substr(range + 1))));
        }
    }
    else
    {
        portOpt = COMM;
        if (range == -1) //range
        {
            _port.push_back(htons((u_int16_t)std::stoi(port)));
        }
        else if (range == 0) //:range
        {
            _port.push_back(0);
            _port.push_back(htons((u_int16_t)std::stoi(port.substr(1))));
        }
        else if ((range + 1) == port.length()) //range:
        {
            _port.push_back(htons((u_int16_t)std::stoi(port.substr(0, range))));
            _port.push_back(UINT16_MAX);
        }
        else //range1:range2
        {
            _port.push_back(htons((u_int16_t)std::stoi(port.substr(0, range))));
            _port.push_back(htons((u_int16_t)std::stoi(port.substr(range + 1))));
        }
    }
}
void CRule::option_parsing(std::string options)
{
    int start = 0, stop = 0, colon;
    std::string opt;
    SRule_option tmp;
    bool contflag = false; //false=only content

    while ((stop = (int)options.find(';', start)) != -1)
    {
        colon = options.find(':', start);
        if ((colon == -1) || (colon > stop))
        {
            opt = options.substr(start, stop - start);
            if (("sameip") == opt)
            {
                if (contflag)
                {
                    rule_options.push_back(tmp);
                    contflag = false;
                }
                tmp.rule = NPSAMEIP;
                tmp.option = "";
                rule_options.push_back(tmp);
                contflag = false;
            }
            else if (contflag)
            {
                tmp.option += ' ';
                tmp.option += options.substr(start, stop - start + 1);
            }
            start = stop + 2;
            if (options[start - 1] != ' ')
                break;
            continue;
        }
        opt = options.substr(start, colon - start);
        if (opt == "pcre")
        {
            if (contflag)
            {
                rule_options.push_back(tmp);
                contflag = false;
            }
            tmp.rule = PCRE;
            tmp.option = options.substr(colon + 1, stop - 1 - colon);
            rule_options.push_back(tmp);
        }
        else if (opt == "ttl")
        {
            if (contflag)
            {
                rule_options.push_back(tmp);
                contflag = false;
            }
            tmp.rule = NPTTL;
            tmp.option = options.substr(colon + 1, stop - 1 - colon);
            rule_options.push_back(tmp);
        }
        else if (opt == "tos")
        {
            if (contflag)
            {
                rule_options.push_back(tmp);
                contflag = false;
            }
            tmp.rule = NPTOS;
            tmp.option = options.substr(colon + 1, stop - 1 - colon);
            rule_options.push_back(tmp);
        }
        else if (opt == "fragbits")
        {
            if (contflag)
            {
                rule_options.push_back(tmp);
                contflag = false;
            }
            tmp.rule = NPFBITS;
            tmp.option = options.substr(colon + 1, stop - 1 - colon);
            rule_options.push_back(tmp);
        }
        else if (opt == "dsize")
        {
            if (contflag)
            {
                rule_options.push_back(tmp);
                contflag = false;
            }
            tmp.rule = NPDSIZE;
            tmp.option = options.substr(colon + 1, stop - 1 - colon);
            rule_options.push_back(tmp);
        }
        else if (opt == "flags")
        {
            if (contflag)
            {
                rule_options.push_back(tmp);
                contflag = false;
            }
            tmp.rule = NPFLAGS;
            tmp.option = options.substr(colon + 1, stop - 1 - colon);
            rule_options.push_back(tmp);
        }
        else if (opt == "seq")
        {
            if (contflag)
            {
                rule_options.push_back(tmp);
                contflag = false;
            }
            tmp.rule = NPSEQ;
            tmp.option = options.substr(colon + 1, stop - 1 - colon);
            rule_options.push_back(tmp);
        }
        else if (opt == "ack")
        {
            if (contflag)
            {
                rule_options.push_back(tmp);
                contflag = false;
            }
            tmp.rule = NPACK;
            tmp.option = options.substr(colon + 1, stop - 1 - colon);
            rule_options.push_back(tmp);
        }
        else if (opt == "window")
        {
            if (contflag)
            {
                rule_options.push_back(tmp);
                contflag = false;
            }
            tmp.rule = NPWINDOW;
            tmp.option = options.substr(colon + 1, stop - 1 - colon);
            rule_options.push_back(tmp);
        }
        else if (opt == "itype")
        {
            if (contflag)
            {
                rule_options.push_back(tmp);
                contflag = false;
            }
            tmp.rule = NPITYPE;
            tmp.option = options.substr(colon + 1, stop - 1 - colon);
            rule_options.push_back(tmp);
        }
        else if (opt == "icode")
        {
            if (contflag)
            {
                rule_options.push_back(tmp);
                contflag = false;
            }
            tmp.rule = NPICODE;
            tmp.option = options.substr(colon + 1, stop - 1 - colon);
            rule_options.push_back(tmp);
        }
        else if (opt == "detection_filter") //detection_filter:track by_src|by_dst, count 10, seconds 30;
        {
            if (contflag)
            {
                rule_options.push_back(tmp);
                contflag = false;
            }
            std::string str = options.substr(colon + 1, stop - 1 - colon);

            std::vector<std::string> tmp;
            boost::split(tmp, str, boost::is_any_of(","));

            for (int i = 0; i < 3; i++)
                boost::algorithm::trim(tmp.at(i));

            if (tmp.at(0).substr(tmp.at(0).find(' ') + 1) == "by_src")
                d_filter.track = SRC;
            else
                d_filter.track = DST;

            d_filter.limit = std::atoi((tmp.at(1).substr(tmp.at(1).find(' ') + 1)).c_str());
            d_filter.timeout = (time_t)std::atoi((tmp.at(2).substr(tmp.at(2).find(' ') + 1)).c_str());
        }
        else if (opt == "nation") // nation:KR;
        {
            if (contflag)
            {
                rule_options.push_back(tmp);
                contflag = false;
            }
            tmp.rule = NATION;
            tmp.option = options.substr(colon + 1, stop - 1 - colon);
            rule_options.push_back(tmp);
        }
        else if (opt == "content")
        {
            if (contflag)
            {
                rule_options.push_back(tmp);
            }
            tmp.rule = CONTENTS;
            tmp.option = options.substr(start, stop - start + 1);
            contflag = true;
        }
        else if (contflag)
        {
            tmp.option += ' ';
            tmp.option += options.substr(start, stop - start + 1);
        }
        start = stop + 2;
        if (options[start - 1] != ' ')
            break;
    }
    if (contflag)
    {
        rule_options.push_back(tmp);
        contflag = false;
    }
}
void CRule::SetHeader(SRule_header rule_header)
{
    if (rule_header.sig_action == "alert")
        action = ALERT;
    else if (rule_header.sig_action == "log")
        action = LOG;
    else
        action = PASS;

    if ((rule_header.sig_protocol == "TCP") || (rule_header.sig_protocol == "tcp"))
        protocols = TCP;
    else if ((rule_header.sig_protocol == "UDP") || (rule_header.sig_protocol == "udp"))
        protocols = UDP;
    else if ((rule_header.sig_protocol == "ICMP") || (rule_header.sig_protocol == "icmp"))
        protocols = ICMP;

    ip_parsing(rule_header.sig_srcIP, src_ipOpt, src_ip, src_netmask);
    port_parsing(rule_header.sig_srcPort, src_portOpt, src_port);
    dir_operator = rule_header.sig_direction;
    ip_parsing(rule_header.sig_dstIP, des_ipOpt, des_ip, des_netmask);
    port_parsing(rule_header.sig_dstPort, des_portOpt, des_port);
}
void CRule::SetOptions(std::string rule_opt)
{
    option_parsing(rule_opt);
}

void CRule::SetIPFromMap(int direction)
{
    IP_value ip_value;
    if (direction == ObserverMap::SET_SOURCE)
    {
        ip_value = IP_map->at(src_IPvariable);
        src_ip = ip_value.ip;
        src_ipOpt = ip_value.ipOpt;
        src_netmask = ip_value.netmask;
    }
    else if (direction == ObserverMap::SET_DEST)
    {
        ip_value = IP_map->at(des_IPvariable);
        des_ip = ip_value.ip;
        des_ipOpt = ip_value.ipOpt;
        des_netmask = ip_value.netmask;
    }
}
void CRule::SetPortFromMap(int direction)
{
    Port_value port_value;
    if (direction == ObserverMap::SET_SOURCE)
    {
        port_value = Port_map->at(src_portvariable);
        src_port = port_value.port;
        src_portOpt = port_value.portOpt;
    }
    else if (direction == ObserverMap::SET_DEST)
    {
        port_value = Port_map->at(des_portvariable);
        des_port = port_value.port;
        des_portOpt = port_value.portOpt;
    }
}
void CRule::UpdateRule()
{
    if(!src_IPvariable.empty())
        SetIPFromMap(ObserverMap::SET_SOURCE);
    
    if(!src_portvariable.empty())
        SetIPFromMap(ObserverMap::SET_SOURCE);

    if(!des_IPvariable.empty())
        SetIPFromMap(ObserverMap::SET_DEST);
    
    if(!des_portvariable.empty())
        SetIPFromMap(ObserverMap::SET_DEST);
}