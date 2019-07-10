#pragma once
#include "ids.h"
#include "Packet.h"
#include "DB.h"
class CDetection_filter
{
private:
    //무결성
    u_int8_t rev;
    u_int32_t sig_id;
    //고정 조건
    time_t timeout;
    int limit;
    u_int32_t ip_addr;
public:
    std::vector<time_t> timeInfo;
    std::vector<CPacket> packet;
    CDetection_filter(u_int32_t sig_id, u_int8_t rev, int limit, time_t timeout, u_int32_t ip_addr);
    ~CDetection_filter();
    CDetection_filter &operator=(const CDetection_filter &ref);
    CDetection_filter(const CDetection_filter &ref);
    inline u_int8_t getrev(){return rev;}
    inline u_int32_t getsig_id(){return sig_id;}
    inline u_int32_t getip_addr() {return ip_addr;}

    bool isMatched();
    void deleteTimeOutPacket(void);
    void insertPacket(CPacket &packet);
    void logging(CDB *db);
};
