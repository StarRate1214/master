#pragma once
#include "ids.h"
class CRule
{
private:
    std::string             action;
    std::string             protocols;
    int                     src_ipOpt; //Source IP option
    u_int32_t               src_ip; //Source IP
    u_int32_t               src_netmask; //Source IP netmask
    int                     src_portOpt; //Source port option
    std::vector<u_int16_t>  src_port; //Source port
    std::string             dir_operator; //Direction operator
    int                     des_ipOpt; //Destination IP option
    u_int32_t               des_ip; //Destination IP
    u_int32_t               des_netmask; //Destination IP netmask
    int                     des_portOpt; //Destination port option
    std::vector<u_int16_t>  des_port; //Destination port
    std::string             rule_options;
    std::string             h_rule(std::string &line); //룰 헤더 파싱
    std::string             h_ruleOption(std::string &line); //룰 옵션 덩어리
    void                    ip_parsing(std::string ip); //ip->ip option, ip, ip netmask
    void                    port_parsing(std::string port); //port->port option, port
public:
    enum{NOT, COMM, ANY}; //IP,port option
    RuleHeader();
    ~RuleHeader();
    RuleHeader( std::string rule); //from file
    RuleHeader( std::string rule_header, std::string rule_opt);
    RuleHeader(const RuleHeader &ref);
    RuleHeader &operator=(const RuleHeader &ref);
    inline std::string              GetAction(void) { return action; }
    inline std::string              GetProtocols(void) { return protocols; }
    inline int                      GetSrcIPOpt(void) { return src_ipOpt; }
    inline u_int32_t                GetSrcIP(void) { return src_ip; }
    inline u_int32_t                GetSrcNetmask(void) { return src_netmask; }
    inline int                      GetSrcPOpt(void) { return src_portOpt; }
    inline std::vector<u_int16_t>   GetSrcPort(void) { return src_port; }
    inline std::string              GetDirOperator(void) { return dir_operator; }
    inline int                      GetDesIPOpt(void) { return des_ipOpt; }
    inline u_int32_t                GetDesIP(void) { return des_ip; }
    inline u_int32_t                GetDesNetmask(void) { return des_netmask; }
    inline int                      GetDesPOpt(void) { return des_portOpt; }
    inline std::vector<u_int16_t>   GetDesPort(void) { return des_port; }
    inline std::string              GetRuleOptions(void) { return rule_options; }
    void SetAction(std::string action);
    void SetProtocols(std::string protocols);
    void SetSrcIP(std::string src_ip); //Set source ip->soruce IP option, source IP, source IP netmask
    void SetSrcPort(std::string port); //Set source port->soruce port option, source port
    void SetDirOperator(std::string dir_operator);
    void SetDesIP(std::string des_ip); //Set destination ip->destination IP option, destination IP, destination IP netmask
    void SetDesPort(std::string des_port); //Set destination port->destination port option, destination port
    void SetRuleOptions(std::string rule_options);
};
