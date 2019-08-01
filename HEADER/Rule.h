#pragma once
#include "ids.h"
#include <boost/algorithm/string.hpp>
typedef struct
{
    int rule;
    std::string option;
} SRule_option;

typedef struct
{
    time_t timeout = 0;
    int limit = 0;
    int track = 0;
} D_filter;

typedef struct
{
    std::string sig_action = "";
    std::string sig_protocol = "";
    std::string sig_srcIP = "";
    std::string sig_srcPort = "";
    std::string sig_direction = "";
    std::string sig_dstIP = "";
    std::string sig_dstPort = "";
} SRule_header;

class CRule
{
private:
    bool sig_run;
    int action;
    int protocols;
    int src_ipOpt;                   //Source IP option
    u_int32_t src_ip;                //Source IP
    u_int32_t src_netmask;           //Source IP netmask
    std::string src_IPvariable;      //Source IP variable
    int src_portOpt;                 //Source port option
    std::vector<u_int16_t> src_port; //Source port
    std::string src_portvariable;    //Source port variable
    std::string dir_operator;        //Direction operator
    int des_ipOpt;                   //Destination IP option
    u_int32_t des_ip;                //Destination IP
    u_int32_t des_netmask;           //Destination IP netmask
    std::string des_IPvariable;      //Source IP variable
    int des_portOpt;                 //Destination port option
    std::vector<u_int16_t> des_port; //Destination port
    std::string des_portvariable;    //Source port variable
    std::vector<SRule_option> rule_options;
    //std::string h_rule(std::string &line); //룰 헤더 파싱
    //std::string                 h_ruleOption(std::string &line); //룰 옵션 덩어리
    u_int32_t sig_id;
    u_int8_t rev;
    D_filter d_filter;
    std::unordered_map<std::string, IP_value> *IP_map;
    std::unordered_map<std::string, Port_value> *Port_map;

    void option_parsing(std::string options);
    void SetIPFromMap(int direction);
    void SetPortFromMap(int direction);
    /*
    void SetAction(std::string action);
    void SetProtocols(std::string protocols);
    void SetSrcIP(std::string src_ip); //Set source ip->soruce IP option, source IP, source IP netmask
    void SetSrcPort(std::string port); //Set source port->soruce port option, source port
    void SetDirOperator(std::string dir_operator);
    void SetDesIP(std::string des_ip); //Set destination ip->destination IP option, destination IP, destination IP netmask
    void SetDesPort(std::string des_port); //Set destination port->destination port option, destination port
    void SetRuleOptions(std::string rule_options);
    void SetGid(u_int32_t gid);
    void SetSid(u_int32_t sid);
    void SetRev(u_int8_t rev);
    */
public:
    //CRule();
    ~CRule();
    //CRule( std::string rule); //from file
    CRule( u_int32_t sig_id, u_int8_t rev, SRule_header rule_header, std::string rule_opt, std::unordered_map<std::string, IP_value> *IP_map, std::unordered_map<std::string, Port_value> *Port_map,bool sig_run=1);
    CRule(const CRule &ref);
    CRule &operator=(const CRule &ref);
    inline bool GetSigrun(void) { return sig_run; }
    inline int GetAction(void) { return action; }
    inline int GetProtocols(void) { return protocols; }
    inline int GetSrcIPOpt(void) { return src_ipOpt; }
    inline u_int32_t GetSrcIP(void) { return src_ip; }
    inline u_int32_t GetSrcNetmask(void) { return src_netmask; }
    inline std::string GetSrcIPvariable(void) { return src_IPvariable; }
    inline int GetSrcPOpt(void) { return src_portOpt; }
    inline std::string GetSrcPortvariable(void) { return src_portvariable; }
    inline std::vector<u_int16_t> GetSrcPort(void) { return src_port; }
    inline std::string GetDirOperator(void) { return dir_operator; }
    inline int GetDesIPOpt(void) { return des_ipOpt; }
    inline u_int32_t GetDesIP(void) { return des_ip; }
    inline u_int32_t GetDesNetmask(void) { return des_netmask; }
    inline std::string GetDesIPvariable(void) { return des_IPvariable; }
    inline int GetDesPOpt(void) { return des_portOpt; }
    inline std::vector<u_int16_t> GetDesPort(void) { return des_port; }
    inline std::string GetDesPortvariable(void) { return des_portvariable; }
    inline std::vector<SRule_option> GetRuleOptions(void) { return rule_options; }
    inline u_int32_t GetSig_id(void) { return sig_id; }
    inline u_int8_t GetRev(void) { return rev; }
    inline D_filter GetD_filter(void) { return d_filter; }
    void SetHeader(SRule_header rule_header);
    void SetOptions(std::string rule_opt);
    void SetSig_run(bool sig_run);

    static void ip_parsing(std::string ip, int &ipOpt, u_int32_t &_ip, u_int32_t &netmask);  //ip->ip option, ip, ip netmask
    static void port_parsing(std::string port, int &portOpt, std::vector<u_int16_t> &_port); //port->port option, port

    void UpdateRule();
};
