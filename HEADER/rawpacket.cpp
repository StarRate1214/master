#include "rawpacket.h"

CRawpacket :: CRawpacket() { }
CRawpacket :: ~CRawpacket() 
{
    delete[] packet;
}

CRawpacket :: CRawpacket(u_int8_t * packet, time_t time)
{
    this->packet = packet;
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
void CRawpacket :: setPacket(u_int8_t * packet) { this->packet = packet; }
void CRawpacket :: setTime(time_t time)         { this->time = time; }