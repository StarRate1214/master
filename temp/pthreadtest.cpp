#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/if_ether.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#include <thread>
#include <queue>
#include <time.h>
#include <unistd.h>
#include <mutex>

// g++ pthreadtest.cpp -o pthreadtest --std=c++11 -pthread
using namespace std;

class CRawpacket
{
private:
    u_int8_t * packet;
    int size;
    time_t time;

public:    
    CRawpacket() { }
    ~CRawpacket() { delete[] packet; }
    CRawpacket(u_int8_t * packet, int size, time_t time)
    {
        this->size = size;
        this->packet = new u_int8_t[this->size];
        for(int i = 0; i<size; i++)
            this->packet[i] = packet[i];
        this->time   = time;
    }
    CRawpacket(const CRawpacket &ref)
    {
        size = ref.size;
        this->packet = new u_int8_t[size];
        for(int i=0; i<size; i++)
            packet[i] = ref.packet[i];
        time = ref.time;
    }
    CRawpacket &operator=(const CRawpacket &ref)
    {
        size = ref.size;
        this->packet = new u_int8_t[size];
        for(int i=0; i<size; i++)
            packet[i] = ref.packet[i];
        time    = ref.time;
        return *this;
    }
    inline u_int8_t * getPacket() { return packet; }
    inline int getSize() { return size; }
    inline time_t getTime() { return time; }
    void setPacket(u_int8_t * packet, int size)
    {
        for(int i = 0; i<size; i++)
            this->packet[i] = packet[i];
    }
    void setSize(int size) { this->size = size; }
    void setTime(time_t time) { this->time = time; }
}; 

std::mutex mtx;

// Packet Capture ( Temporarily )
void pcap(int no, queue<CRawpacket> * p)
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
void proc(int no, queue<CRawpacket> * p)
{ 
    CRawpacket rawpacket;
    u_int8_t * ptr;
    time_t time;
    int size = 0;
    int n = 0;
    int i = 0;
    while(1)
    {
        printf("INPUT : ");
        scanf("%d",&n);

        if(n == 1) {
        rawpacket = p->front();
        size = rawpacket.getSize();
        ptr = rawpacket.getPacket();
        time = rawpacket.getTime();

        cout << "PACKET : ";
        for(int i = 0; i < size; i++){
            if(i % 8 == 0)
                cout << endl;
            printf("%x ", ptr[i]);
        }

        cout << endl << "TIME : " << time << endl;
        cout << endl;
        p->pop();
        }
    }
}
 
int main(){
    queue<CRawpacket> * p;
    p = new queue<CRawpacket>;
    
    thread thread1(pcap, 1, p);
    thread thread2(proc, 2, p);
 
    thread1.join();
    thread2.join();

    delete p;
 
    return 0;
}
