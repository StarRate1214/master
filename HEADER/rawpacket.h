#pragma once
#include "ids.h"

class CRawpacket
{
private:
    u_int8_t *packet;
    pcap_pkthdr pkthdr;

public:
    ~CRawpacket();
    CRawpacket(u_int8_t *packet,  pcap_pkthdr pkthdr);
    CRawpacket(const CRawpacket &ref);
    CRawpacket &operator=(const CRawpacket &ref);
    inline u_int8_t *getPacket() { return packet; }
    inline pcap_pkthdr getPkthdr(){return pkthdr;}
    void setPacket(u_int8_t *packet, int size);
    void setPkthdr(pcap_pkthdr pkthdr);
};