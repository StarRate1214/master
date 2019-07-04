#pragma once
#include "ids.h"
#include "Packet.h"
#include "DB.h"
class CCount
{
private:
    //무결성
    u_int8_t rev;
    u_int32_t sig_id;
    //고정 조건
    time_t timeout;
    int limit;
    //flag
    bool logged;//초기화 필요
public:
    std::vector<time_t> timeInfo;
    std::vector<CPacket> packet;

    CCount(u_int32_t sig_id, u_int8_t rev, int limit, time_t timeout);
    ~CCount();
    CCount &operator=(CCount &ref);
    CCount(CCount &ref);
    inline u_int8_t getrev(){return rev;}
    inline u_int32_t getsig_id(){return sig_id;}

    int getPacketCount(void);
    void deleteTimeOutPacket(void);
    void insertPacket(CPacket &packet);
    void logging(CDB *db);
};
