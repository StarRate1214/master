#include "Capture.h"

CCapture::CCapture(std::string interface)
{
    //pcap_open_live(인터페이스, 패킷저장길이, 캡쳐모드, 패킷저장길이가 채워지지않아도 지정한시간이 지나면 리턴, buff)
    if (!(adhandle = pcap_open_live(interface.c_str(), UINT16_MAX, 1, 1000, errbuf)))
        pcap_perror(adhandle, errbuf);
}

void CCapture::packetCapture(std::queue<CRawpacket *> *packetQueue, std::mutex *mtx)
{
    int res; //next_ex 오류값리턴저장
    while ((res = pcap_next_ex(adhandle, &header, (const u_char **)&buff)) >= 0)
    {
        if (res == 0) //타임아웃
            continue;
        if (res == -1) //에러
        {
            printf("Error reading the packets: %s\n", pcap_geterr(adhandle));
            exit(1);
        }

        // ethernet type
        // u_int16_t ethertype = ((u_int16_t)buff[12] << 8) | buff[13];
        u_int16_t ethertype = (u_int16_t)buff[12];
        ethertype <<= 8;
        ethertype += buff[13];
        if (ethertype == ETHERTYPE_IP)
        {
            // ethernet + ip header length
            // int pkhl = (buff[14]&0x0F)*4;
            if (buff[23] == IPPROTO_TCP || buff[23] == IPPROTO_UDP || buff[23] == IPPROTO_ICMP)
            {
                // input packet data in queue
                CRawpacket *rawpacket = new CRawpacket(buff, *header);
                mtx->lock();
                packetQueue->push(rawpacket);
                mtx->unlock();
            }
        }
        /*
        local_tv_sec = header->ts.tv_sec;
        localtime_r(&local_tv_sec, &ltime);
        strftime(timestr,sizeof(timestr),"%H시%M분%S",&ltime);
        printf("time : %s\n", timestr);
        */
    }

    pcap_close(adhandle); //close하는 함수
}