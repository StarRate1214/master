#include "rawpacket.h"

CRawpacket :: CRawpacket() { }
CRawpacket :: ~CRawpacket() 
{
    delete[] packet;
}

CRawpacket :: CRawpacket(u_int8_t * packet, int size, time_t time)
{
    this->packet = new u_int8_t[size];
    for(int i=0; i<size; i++)
        this->packet[i] = packet[i];
    this->time   = time;
}
CRawpacket :: CRawpacket(const CRawpacket &ref)
{
    packet  = ref.packet;
    time    = ref.time;
}
CRawpacket &CRawpacket :: operator=(const CRawpacket &ref)
{
    packet  = ref.packet;
    time    = ref.time;
    return *this;
}
void CRawpacket :: setPacket(u_int8_t * packet, int size)
{ 
    for(int i=0; i<size; i++)
        this->packet[i] = packet[i];
}
void CRawpacket :: setTime(time_t time) { this->time = time; }