#include"Capture.h"
#include <net/if.h>
#include <errno.h>
#include <netpacket/packet.h>
CCapture::CCapture(std::string interface)
{
    struct sockaddr_ll sockAddr;
    memset(&sockAddr,0,sizeof(struct sockaddr_ll));


    if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
    {
        // socket error
        perror("packet capture socket error: ");
        exit(1);
    }
    
}

void CCapture::packetCapture(std::queue<CRawpacket> *packetQueue, std::mutex *mtx)
{
    while (1)
    {
        // get packet
        if ((n = recv(sockfd, buff, ETHER_MAX_LEN, 0)) < 0)
        {
            // recv error
            perror("packet capture recv error: ");
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
            if (buff[23] == IPPROTO_TCP)
            {
                // input packet data in queue
                pcap_pkthdr pkthdr;//수정요
                CRawpacket rawpacket(buff, pkthdr);

                mtx->lock();
                packetQueue->push(rawpacket);
                mtx->unlock();
            }
        }
    }
}