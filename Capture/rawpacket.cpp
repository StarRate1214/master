#include "rawpacket.h"

CRawpacket ::~CRawpacket() { delete[] packet; }

CRawpacket ::CRawpacket(u_int8_t *packet, pcap_pkthdr pkthdr)
{
    this->pkthdr = pkthdr;
    this->packet = new u_int8_t[pkthdr.len];
    for (int i = 0; i < pkthdr.len; i++)
        this->packet[i] = packet[i];
}

CRawpacket ::CRawpacket(const CRawpacket &ref)
{
    pkthdr = ref.pkthdr;
    this->packet = new u_int8_t[pkthdr.len];
    for (int i = 0; i < pkthdr.len; i++)
        packet[i] = ref.packet[i];
}

CRawpacket &CRawpacket ::operator=(const CRawpacket &ref)
{
    delete[] packet;
    pkthdr = ref.pkthdr;
    this->packet = new u_int8_t[pkthdr.len];
    for (int i = 0; i < pkthdr.len; i++)
        packet[i] = ref.packet[i];
    return *this;
}

void CRawpacket ::setPacket(u_int8_t *packet, int size)
{
    for (int i = 0; i < size; i++)
        this->packet[i] = packet[i];
}

void CRawpacket ::setPkthdr(pcap_pkthdr pkthdr)
{
    this->pkthdr=pkthdr;
}