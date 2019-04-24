#include "Packet.h"

CEthernet::CEthernet(){}
CEthernet::~CEthernet(){}
CEthernet::CEthernet(u_int8_t src_mac[], u_int8_t dst_mac[],u_int16_t ether_type)
{
    for(int i=0; i<6;i++)
    {
        this->src_mac[i]=src_mac[i];
        this->dst_mac[i]=dst_mac[i];
    }
    this->ether_type=ether_type;
}
CEthernet::CEthernet(const CEthernet &ref)
    :ether_type(ref.ether_type)
{
    for(int i=0; i<6;i++)
    {
        src_mac[i]=ref.src_mac[i];
        dst_mac[i]=ref.dst_mac[i];
    }
}
CEthernet &CEthernet::operator=(const CEthernet &ref)
{
    for(int i=0; i<6;i++)
    {
        src_mac[i]=ref.src_mac[i];
        dst_mac[i]=ref.dst_mac[i];
    }
    ether_type=ref.ether_type;
}
void CEthernet::setSrcMac(u_int8_t src_mac[])
{
    for(int i=0; i<6;i++)
        this->src_mac[i]=src_mac[i];
}
void CEthernet::setDstMac(u_int8_t dst_mac[6])
{
    for(int i=0; i<6;i++)
        this->dst_mac[i]=dst_mac[i];
}
void CEthernet::setEtherType(u_int16_t ether_type)
{
    this->ether_type=ether_type;
}

    
CIPv4::CIPv4()
    :src_ip(0), dst_ip(0), tos(0), more_frag(false), dont_frag(true), ttl(0)
{}
CIPv4::~CIPv4(){}
CIPv4::CIPv4(
    u_int32_t src_ip,
    u_int32_t dst_ip,
    u_int8_t tos,
    bool more_frag,
    bool dont_frag,
    u_int8_t ttl
)
{
    this->src_ip=src_ip;
    this->dst_ip=dst_ip;
    this->tos=tos;
    this->more_frag=more_frag;
    this->dont_frag=dont_frag;
    this->ttl=ttl;
}
CIPv4::CIPv4(const CIPv4 &ref)
    :src_ip(ref.src_ip), dst_ip(ref.dst_ip), tos(ref.tos), more_frag(ref.more_frag), dont_frag(ref.dont_frag), ttl(ref.ttl)
{}
CIPv4 &CIPv4::operator=(const CIPv4 &ref)
{
    src_ip=ref.src_ip;
    dst_ip=ref.dst_ip;
    tos=ref.tos;
    more_frag=ref.more_frag;
    dont_frag=ref.dont_frag;
    ttl=ref.ttl;
}
void CIPv4::setSrcIP(u_int32_t src_ip)
{
    this->src_ip=src_ip;
}
void CIPv4::setDstIP(u_int32_t dst_ip)
{
    this->dst_ip=dst_ip;
}
void CIPv4::setTos(u_int8_t tos)
{
    this->tos=tos;
}
void CIPv4::setMoreFrag(bool more_frag)
{
    this->more_frag=more_frag;
}
void CIPv4::setDontFrag(bool dont_frag)
{
    this->dont_frag=dont_frag;
}
void CIPv4::setTTL(u_int8_t ttl)
{
    this->ttl=ttl;
}

    
CICMP::CICMP()
    :icmp_type(0), icmp_code(0)
{}
CICMP::~CICMP(){}
CICMP::CICMP(u_int8_t icmp_type, u_int8_t icmp_code)
{
    this->icmp_type=icmp_type;
    this->icmp_code=icmp_code;
}
CICMP::CICMP (const CICMP &ref)
    :icmp_type(ref.icmp_type), icmp_code(ref.icmp_code)
{}
CICMP &CICMP::operator=(const CICMP &ref)
{
    icmp_type=ref.icmp_type;
    icmp_code=ref.icmp_code;
}
void CICMP::setICMPtype(u_int8_t icmp_type)
{
    this->icmp_type=icmp_type;
}
void CICMP::setICMPcode(u_int8_t icmp_code)
{
    this->icmp_code=icmp_code;
}