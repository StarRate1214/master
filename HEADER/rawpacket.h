#pragma once

#include <sys/types.h>

class CRawpacket
{
private:
    u_int8_t * packet;
    int size;
    time_t time;

public:    
    CRawpacket();
    ~CRawpacket();
    CRawpacket(u_int8_t * packet, int size, time_t time);
    CRawpacket(const CRawpacket &ref);
    CRawpacket &operator=(const CRawpacket &ref);
    inline u_int8_t * getPacket();
    inline int getSize();
    inline time_t getTime();
    void setPacket(u_int8_t * packet, int size);
    void setSize(int size);
    void setTime(time_t time);
}; 