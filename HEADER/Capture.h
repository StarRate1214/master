#pragma once
#include "rawpacket.h"
class CCapture
{
private:
    int sockfd, n;
    u_int8_t buff[ETH_FRAME_LEN];
public:
    CCapture(std::string interface);
    void packetCapture(std::queue<CRawpacket> *p, std::mutex *mtx);
};