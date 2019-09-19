#pragma once
#include "rawpacket.h"
class CCapture
{
private:
    pcap_t *adhandle; //open_live
    char errbuf[PCAP_ERRBUF_SIZE]; //오류스트링
    struct pcap_pkthdr *header; //패킷헤더(사이즈, 타임스탬프)
    u_int8_t *buff; //패킷저장
public:
    CCapture(std::string interface, std::string pcapPath);
    void packetCapture(std::queue<CRawpacket*> *packetQueue, std::mutex *mtx);
};