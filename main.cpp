#include "DB.h"
#include "RuleEngine.h"
#include <thread>
#include <queue>

void compareRules(int no, queue<CRawpacket> * p);
void packetCapture(int no, queue<CRawpacket> * p);

std::mutex mtx;

int main()
{
    std::queue<CRawpacket> *packetQueue = new std::queue<CRawpacket>;
    std::vector<CRule> *rules=new std::vector<CRule>;
    CDB db=new CDB("localhost", "test", "Qwer!234", "test");
    thread thread1(packetCapture, 1, packetQueue);
    thread thread2(compareRules, 2, packetQueue);

    thread1.join();
    thread2.join();

    delete p;
 
    return 0;
}

void packetCapture(int no, queue<CRawpacket> * p)
{
    int sockfd, n;
    u_int8_t buff[ETH_FRAME_LEN];

    if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
    {
        // socket error
    }
    while(1)
    {
        //mutex lock
        mtx.lock();

        // get packet
        if ((n = recv(sockfd, buff, ETHER_MAX_LEN, 0)) < 0)
        {
            // recv error
        }
        
        // ethernet type
        // u_int16_t ethertype = ((u_int16_t)buff[12] << 8) | buff[13];
        u_int16_t ethertype = (u_int16_t)buff[12];
        ethertype <<= 8;
        ethertype += buff[13];

        if(ethertype == ETHERTYPE_IP)
        {
            // ethernet + ip header length 
            // int pkhl = (buff[14]&0x0F)*4;
            if(buff[23] == IPPROTO_TCP)
            {
                // input packet data in queue
                CRawpacket rawpacket(buff, n, time(NULL));
                p->push(rawpacket);
            }
        
        }
        mtx.unlock();
    }
}

// pop data from queue ( Temporarily )
void compareRules(int no, queue<CRawpacket> * p)
{
    
}