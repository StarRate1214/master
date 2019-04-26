#include "Packet.h"

CEthernet::CEthernet() {}
CEthernet::~CEthernet() {}
CEthernet::CEthernet(u_int8_t src_mac[], u_int8_t dst_mac[], u_int16_t ether_type)
{
    for (int i = 0; i < 6; i++)
    {
        this->src_mac[i] = src_mac[i];
        this->dst_mac[i] = dst_mac[i];
    }
    this->ether_type = ether_type;
}
CEthernet::CEthernet(const CEthernet &ref)
    : ether_type(ref.ether_type)
{
    for (int i = 0; i < 6; i++)
    {
        src_mac[i] = ref.src_mac[i];
        dst_mac[i] = ref.dst_mac[i];
    }
}
CEthernet &CEthernet::operator=(const CEthernet &ref)
{
    for (int i = 0; i < 6; i++)
    {
        src_mac[i] = ref.src_mac[i];
        dst_mac[i] = ref.dst_mac[i];
    }
    ether_type = ref.ether_type;
    return *this;
}
void CEthernet::setSrcMac(u_int8_t src_mac[])
{
    for (int i = 0; i < 6; i++)
        this->src_mac[i] = src_mac[i];
}
void CEthernet::setDstMac(u_int8_t dst_mac[6])
{
    for (int i = 0; i < 6; i++)
        this->dst_mac[i] = dst_mac[i];
}
void CEthernet::setEtherType(u_int16_t ether_type)
{
    this->ether_type = ether_type;
}

CIPv4::CIPv4()
    : src_ip(0), dst_ip(0), tos(0), more_frag(false), dont_frag(true), ttl(0)
{
}
CIPv4::~CIPv4() {}
CIPv4::CIPv4(
    u_int32_t src_ip,
    u_int32_t dst_ip,
    u_int8_t tos,
    bool more_frag,
    bool dont_frag,
    u_int8_t ttl)
{
    this->src_ip = src_ip;
    this->dst_ip = dst_ip;
    this->tos = tos;
    this->more_frag = more_frag;
    this->dont_frag = dont_frag;
    this->ttl = ttl;
}
CIPv4::CIPv4(const CIPv4 &ref)
    : src_ip(ref.src_ip), dst_ip(ref.dst_ip), tos(ref.tos), more_frag(ref.more_frag), dont_frag(ref.dont_frag), ttl(ref.ttl)
{
}
CIPv4 &CIPv4::operator=(const CIPv4 &ref)
{
    src_ip = ref.src_ip;
    dst_ip = ref.dst_ip;
    tos = ref.tos;
    more_frag = ref.more_frag;
    dont_frag = ref.dont_frag;
    ttl = ref.ttl;
    return *this;
}
void CIPv4::setSrcIP(u_int32_t src_ip)
{
    this->src_ip = src_ip;
}
void CIPv4::setDstIP(u_int32_t dst_ip)
{
    this->dst_ip = dst_ip;
}
void CIPv4::setTos(u_int8_t tos)
{
    this->tos = tos;
}
void CIPv4::setMoreFrag(bool more_frag)
{
    this->more_frag = more_frag;
}
void CIPv4::setDontFrag(bool dont_frag)
{
    this->dont_frag = dont_frag;
}
void CIPv4::setTTL(u_int8_t ttl)
{
    this->ttl = ttl;
}

CICMP::CICMP()
    : icmp_type(0), icmp_code(0)
{
}
CICMP::~CICMP() {}
CICMP::CICMP(u_int8_t icmp_type, u_int8_t icmp_code)
{
    this->icmp_type = icmp_type;
    this->icmp_code = icmp_code;
}
CICMP::CICMP(const CICMP &ref)
    : icmp_type(ref.icmp_type), icmp_code(ref.icmp_code)
{
}
CICMP &CICMP::operator=(const CICMP &ref)
{
    icmp_type = ref.icmp_type;
    icmp_code = ref.icmp_code;
    return *this;
}
void CICMP::setICMPtype(u_int8_t icmp_type)
{
    this->icmp_type = icmp_type;
}
void CICMP::setICMPcode(u_int8_t icmp_code)
{
    this->icmp_code = icmp_code;
}

CTCP ::CTCP()
{
}
CTCP ::~CTCP()
{
}
CTCP ::CTCP(u_int16_t src_port,
            u_int16_t dst_port,
            u_int32_t seq_num,
            u_int32_t ack_num,
            bool urg,
            bool ack,
            bool psh,
            bool rst,
            bool syn,
            bool fin,
            u_int16_t win_size)
{
    this->src_port = src_port;
    this->dst_port = dst_port;
    this->seq_num = seq_num;
    this->ack_num = ack_num;
    this->urg = urg;
    this->ack = ack;
    this->psh = psh;
    this->rst = rst;
    this->syn = syn;
    this->fin = fin;
    this->win_size = win_size;
}
CTCP ::CTCP(const CTCP &ref)
{
    src_port = ref.src_port;
    dst_port = ref.dst_port;
    seq_num = ref.seq_num;
    ack_num = ref.ack_num;
    urg = ref.urg;
    ack = ref.ack;
    psh = ref.psh;
    rst = ref.rst;
    syn = ref.syn;
    fin = ref.fin;
    win_size = ref.win_size;
}
CTCP &CTCP ::operator=(const CTCP &ref)
{
    src_port = ref.src_port;
    dst_port = ref.dst_port;
    seq_num = ref.seq_num;
    ack_num = ref.ack_num;
    urg = ref.urg;
    ack = ref.ack;
    psh = ref.psh;
    rst = ref.rst;
    syn = ref.syn;
    fin = ref.fin;
    win_size = ref.win_size;

    return *this;
}
void CTCP ::setSrcPort(u_int16_t src_port) { this->src_port = src_port; }
void CTCP ::setDstPort(u_int16_t dst_port) { this->dst_port = dst_port; }
void CTCP ::setSeqNum(u_int32_t seq_num) { this->seq_num = seq_num; }
void CTCP ::setAckNum(u_int32_t ack_num) { this->ack_num = ack_num; }
void CTCP ::setUrg(bool urg) { this->urg = urg; }
void CTCP ::setAck(bool ack) { this->ack = ack; }
void CTCP ::setPsh(bool psh) { this->psh = psh; }
void CTCP ::setRst(bool rst) { this->rst = rst; }
void CTCP ::setSyn(bool syn) { this->syn = syn; }
void CTCP ::setFin(bool fin) { this->fin = fin; }
void CTCP ::setWinSize(u_int16_t win_size) { this->win_size = win_size; }

CUDP ::CUDP()
{
}
CUDP ::~CUDP()
{
}
CUDP ::CUDP(u_int16_t src_port, u_int16_t dst_port)
{
    this->src_port = src_port;
    this->dst_port = dst_port;
}
CUDP ::CUDP(const CUDP &ref)
{
    src_port = ref.src_port;
    dst_port = ref.dst_port;
}
CUDP &CUDP ::operator=(const CUDP &ref)
{
    src_port = ref.src_port;
    dst_port = ref.dst_port;
    return *this;
}
void CUDP ::setSrcPort(u_int16_t src_port) { this->src_port = src_port; }
void CUDP ::setDstPort(u_int16_t dst_port) { this->dst_port = dst_port; }