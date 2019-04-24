#pragma once
#include<sys/types.h>
class CEthernet
{
private:
    u_int8_t src_mac[6];
    u_int8_t dst_mac[6];
    u_int16_t ether_type;
public:
    CEthernet();
    ~CEthernet();
    CEthernet(u_int8_t src_mac[], u_int8_t dst_mac[],u_int16_t ether_type);
    CEthernet(const CEthernet &ref);
    CEthernet &operator=(const CEthernet &ref);
    inline u_int8_t* getSrcMac(){return src_mac;}
    inline u_int8_t* getDstMac(){return dst_mac;}
    inline u_int16_t getType(){return ether_type;}
    void setSrcMac(u_int8_t src_mac[]);
    void setDstMac(u_int8_t dst_mac[]);
    void setEtherType(u_int16_t ether_type);
};

class CIPv4:public CEthernet
{
private:
    u_int32_t src_ip;
    u_int32_t dst_ip;
    u_int8_t tos;
    bool more_frag;
    bool dont_frag;
    u_int8_t ttl;
public:
    CIPv4();
    ~CIPv4();
    CIPv4(
        u_int32_t src_ip,
        u_int32_t dst_ip,
        u_int8_t tos,
        bool more_frag,
        bool dont_frag,
        u_int8_t ttl
    );
    CIPv4(const CIPv4 &ref);
    CIPv4 &operator=(const CIPv4 &ref);
    inline u_int32_t getSrcIP(){return src_ip;}
    inline u_int32_t getDstIP(){return dst_ip;}
    inline u_int8_t getTos(){return tos;}
    inline bool getMoreFrag(){return more_frag;}
    inline bool getDontFrag(){return dont_frag;}
    inline u_int8_t getTTL(){return ttl;}
    void setSrcIP(u_int32_t src_ip);
    void setDstIP(u_int32_t dst_ip);
    void setTos(u_int8_t tos);
    void setMoreFrag(bool more_frag);
    void setDontFrag(bool dont_frag);
    void setTTL(u_int8_t ttl);
};

class CICMP:CIPv4
{
private:
    u_int8_t icmp_type;
    u_int8_t icmp_code;
public:
    CICMP();
    ~CICMP();
    CICMP(u_int8_t icmp_type, u_int8_t icmp_code);
    CICMP (const CICMP &ref);
    CICMP &operator=(const CICMP &ref);
    inline u_int8_t getICMPtype(){return icmp_type;}
    inline u_int8_t getICMPcode(){return icmp_code;}
    void setICMPtype(u_int8_t icmp_type);
    void setICMPcode(u_int8_t icmp_code);
};