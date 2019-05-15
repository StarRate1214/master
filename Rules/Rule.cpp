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

CRule::CRule(std::string rule)
{
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
}
CRule::CRule( u_int32_t sig_id, std::string rule_header, std::string rule_opt)
{
    this->sig_id;
    action = h_rule(rule_header);
    protocols = stoi(h_rule(rule_header));
    std::string sIP = h_rule(rule_header);
    ip_parsing(sIP, src_ipOpt, src_ip, src_netmask);
    std::string sPort = h_rule(rule_header);
    port_parsing(sPort, src_portOpt, src_port);
    dir_operator = h_rule(rule_header);
    std::string dIP = h_rule(rule_header);
    ip_parsing(dIP, des_ipOpt, des_ip, des_netmask);
    std::string dPort = h_rule(rule_header);
    port_parsing(dPort, des_portOpt, des_port);
    option_parsing(rule_opt);
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
    des_ip = ref.des_ip;
    des_netmask = ref.des_netmask;
    des_portOpt = ref.des_portOpt;
    des_port = ref.des_port;
    rule_options = ref.rule_options;
    sig_id = ref.sig_id;
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
    des_ip = ref.des_ip;
    des_netmask = ref.des_netmask;
    des_portOpt = ref.des_portOpt;
    des_port = ref.des_port;
    rule_options = ref.rule_options;
    sig_id = ref.sig_id;
    return *this;
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
    int nmask = UINT32_MAX;
    std::string tmp;
    if(ip=="any")
    {
        ipOpt=ANY;
        _ip=0;
        netmask=0;
    }
    else if(ip[0]=='!')
    {
        ipOpt=NOT;
        if(mask==-1) //!ip
        {
            tmp=ip.substr(1);
            _ip=htonl(inet_addr(tmp.c_str())); 
        }
        else //!ip/prefix
        {
            tmp=ip.substr(1,mask-1);
            _ip=htonl(inet_addr(tmp.c_str()));
            mask=stoi(ip.substr(mask+1));
            mask=32-mask;
            netmask= nmask<<mask;
        }        
    }
    else
    {
        ipOpt=COMM;
        ip.substr(0,mask);
        if(mask==-1) //ip
        {
            tmp=ip.substr(0);
            _ip=htonl(inet_addr(tmp.c_str()));
        }
        else //ip/prefix
        {
            tmp=ip.substr(0,mask);
            _ip=htonl(inet_addr(tmp.c_str()));
            mask=stoi(ip.substr(mask+1));
            mask=32-mask;
            netmask= nmask<<mask;
        }
    }
}

void CRule::port_parsing(std::string port, int &portOpt, std::vector<u_int16_t> &_port)
{
    int range = port.find(':');
    if(port=="any")
    {
        portOpt=ANY;
        _port.push_back(0);
    }
    else if(port[0]=='!')
    {
        portOpt=NOT;
        if(range==-1) //!range
        {
            _port.push_back(htons((u_int16_t)stoi(port.substr(1))));
        }
        else if(range==1) //!:range
        {
            _port.push_back(0);
            _port.push_back(htons((u_int16_t)stoi(port.substr(2))));
        }
        else if((range+1)==port.length()) //!range:
        {
            _port.push_back(htons((u_int16_t)stoi(port.substr(1,range-1))));
            _port.push_back(UINT16_MAX);
        }
        else //!range1:range2
        {
            _port.push_back(htons((u_int16_t)stoi(port.substr(1,range-1))));
            _port.push_back(htons((u_int16_t)stoi(port.substr(range+1))));
        }
    }
    else
    {
        portOpt=COMM;
        if(range==-1) //range
        {
            _port.push_back(htons((u_int16_t)stoi(port)));
        }
        else if(range==0) //:range 
        {
            _port.push_back(0);
            _port.push_back(htons((u_int16_t)stoi(port.substr(1))));
        }
        else if((range+1)==port.length()) //range:
        {
            _port.push_back(htons((u_int16_t)stoi(port.substr(0,range))));
            _port.push_back(UINT16_MAX);
        }
        else //range1:range2
        {
            _port.push_back(htons((u_int16_t)stoi(port.substr(0,range))));
            _port.push_back(htons((u_int16_t)stoi(port.substr(range+1))));
        }
    }    
}
void CRule::option_parsing(std::string options)
{
    int	start=0,stop=0,semicolon;
	std::string opt;
	SRule_option tmp;
	bool contflag = false; //false=only content
	while ((stop=(int)options.find(':',start)) != -1)
	{
		semicolon = options.find(';', stop);
		opt = options.substr(start, stop - start);
		if (opt == "pcre")
		{
			if (contflag)
			{
				rule_options.push_back(tmp);
				contflag = false;
			}
			tmp.rule = PCRE;
			tmp.option = options.substr(stop+1,semicolon-1-stop);
			rule_options.push_back(tmp);
		}
		else if(opt == "ttl")
		{
			if (contflag)
			{
				rule_options.push_back(tmp);
				contflag = false;
			}
			tmp.rule = NPTTL;
			tmp.option = options.substr(stop + 1, semicolon - 1 - stop);
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
			tmp.option = options.substr(stop + 1, semicolon - 1 - stop);
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
			tmp.option = options.substr(stop + 1, semicolon - 1 - stop);
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
			tmp.option = options.substr(stop + 1, semicolon - 1 - stop);
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
			tmp.option = options.substr(stop + 1, semicolon - 1 - stop);
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
			tmp.option = options.substr(stop + 1, semicolon - 1 - stop);
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
			tmp.option = options.substr(stop + 1, semicolon - 1 - stop);
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
			tmp.option = options.substr(stop + 1, semicolon - 1 - stop);
			rule_options.push_back(tmp);
		}
		else if (opt == "itype")
		{
			tmp.rule = NPITYPE;
			tmp.option = options.substr(stop + 1, semicolon - 1 - stop);
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
			tmp.option = options.substr(stop + 1, semicolon -1 - stop);
			rule_options.push_back(tmp);
		}
		else if (opt == "content")
		{
			tmp.rule = NPTTL;
			tmp.option = options.substr(start, semicolon - start +1);
			contflag = true;
		}
		else { //content others
			if ((options.substr(start, semicolon - start + 1))[0] != ' ')
				tmp.option += ' ';
			tmp.option += options.substr( start, semicolon - start+1);	
		}			
		start = semicolon + 2;
		if (options[start-1] != ' ')
			break;
	}
}
