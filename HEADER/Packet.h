#pragma once
#include "ids.h"

#define CHECK_DF 0x4000
#define CHECK_MF 0x2000

/*
class CEthernet
{
private:
    u_int8_t src_mac[6];
    u_int8_t dst_mac[6];
    u_int16_t ether_type;

public:
    CEthernet();
    ~CEthernet();
    CEthernet(u_int8_t src_mac[], u_int8_t dst_mac[], u_int16_t ether_type);
    CEthernet(const CEthernet &ref);
    CEthernet &operator=(const CEthernet &ref);
    inline u_int8_t *getSrcMac() { return src_mac; }
    inline u_int8_t *getDstMac() { return dst_mac; }
    inline u_int16_t getType() { return ether_type; }
    void setSrcMac(u_int8_t src_mac[]);
    void setDstMac(u_int8_t dst_mac[]);
    void setEtherType(u_int16_t ether_type);
};
*/
class CIPv4 // : public CEthernet
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
        u_int8_t ttl);
    CIPv4(const CIPv4 &ref);
    CIPv4 &operator=(const CIPv4 &ref);
    inline u_int32_t getSrcIP() { return src_ip; }
    inline u_int32_t getDstIP() { return dst_ip; }
    inline u_int8_t getTos() { return tos; }
    inline bool getMoreFrag() { return more_frag; }
    inline bool getDontFrag() { return dont_frag; }
    inline u_int8_t getTTL() { return ttl; }
    void setSrcIP(u_int32_t src_ip);
    void setDstIP(u_int32_t dst_ip);
    void setTos(u_int8_t tos);
    void setMoreFrag(bool more_frag);
    void setDontFrag(bool dont_frag);
    void setTTL(u_int8_t ttl);
};

class CICMP // : public CIPv4
{
private:
    u_int8_t icmp_type;
    u_int8_t icmp_code;

public:
    CICMP();
    ~CICMP();
    CICMP(u_int8_t icmp_type, u_int8_t icmp_code);
    CICMP(const CICMP &ref);
    CICMP &operator=(const CICMP &ref);
    inline u_int8_t getICMPtype() { return icmp_type; }
    inline u_int8_t getICMPcode() { return icmp_code; }
    void setICMPtype(u_int8_t icmp_type);
    void setICMPcode(u_int8_t icmp_code);
};

class CTCP // : public CIPv4
{
private:
    u_int16_t src_port;
    u_int16_t dst_port;
    u_int32_t seq_num;
    u_int32_t ack_num;
    bool urg;
    bool ack;
    bool psh;
    bool rst;
    bool syn;
    bool fin;
    u_int16_t win_size;

public:
    CTCP();
    ~CTCP();
    CTCP(u_int16_t src_port,
         u_int16_t dst_port,
         u_int32_t seq_num,
         u_int32_t ack_num,
         bool urg,
         bool ack,
         bool psh,
         bool rst,
         bool syn,
         bool fin,
         u_int16_t win_size);
    CTCP(const CTCP &ref);
    CTCP &operator=(const CTCP &ref);
    inline u_int16_t getSrcPort(void) { return src_port; }
    inline u_int16_t getDstPort(void) { return dst_port; }
    inline u_int32_t getSeqNum(void) { return seq_num; }
    inline u_int32_t getAckNum(void) { return ack_num; }
    inline bool getUrg(void) { return urg; }
    inline bool getAck(void) { return ack; }
    inline bool getPsh(void) { return psh; }
    inline bool getRst(void) { return rst; }
    inline bool getSyn(void) { return syn; }
    inline bool getFin(void) { return fin; }
    inline u_int16_t getWinSize(void) { return win_size; }
    void setSrcPort(u_int16_t src_port);
    void setDstPort(u_int16_t dst_port);
    void setSeqNum(u_int32_t seq_num);
    void setAckNum(u_int32_t ack_num);
    void setUrg(bool urg);
    void setAck(bool ack);
    void setPsh(bool psh);
    void setRst(bool rst);
    void setSyn(bool syn);
    void setFin(bool fin);
    void setWinSize(u_int16_t win_size);
};

class CUDP // : public CIPv4
{
private:
    u_int16_t src_port;
    u_int16_t dst_port;

public:
    CUDP();
    ~CUDP();
    CUDP(u_int16_t src_port, u_int16_t dst_port);
    CUDP(const CUDP &ref);
    CUDP &operator=(const CUDP &ref);
    inline u_int16_t getSrcPort(void) { return src_port; }
    inline u_int16_t getDstPort(void) { return dst_port; }
    void setSrcPort(u_int16_t src_port);
    void setDstPort(u_int16_t dst_port);
};

class CPacket
{
public:
    int protocol_type;
    CTCP tcp;
    CUDP udp;
    CICMP icmp;
    CIPv4 ip;
    time_t time;
    u_int32_t data_payload_size;
    u_int8_t *data_payload;

    CPacket();
    ~CPacket();
    CPacket(const CPacket &ref);
    CPacket &operator=(const CPacket &ref);
};
