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

// g++ pthreadtest.cpp -o pthreadtest --std=c+11 -pthread
using namespace std;
struct Rawpacket
{
    u_int8_t * packet;
    time_t time;
    u_int8_t size;
};

void pcap(int no, queue<u_int8_t *> * p, queue<u_int8_t> * s)
{
    int sockfd, n;

    u_int8_t buff[ETH_FRAME_LEN];

    if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
    {
        // socket error
    }
    while(1)
    {
        if ((n = recv(sockfd, buff, ETHER_MAX_LEN, 0)) < 0)
        {
            // recv error
        }
        struct ether_header *eh = (struct ether_header*)buff;
        u_int16_t ether_type = ntohs(eh->ether_type);

        if(ether_type == ETHERTYPE_IP)
        {
            struct iphdr *iph = (struct iphdr*)&buff[ETH_HLEN];
            int pkhl = (iph->ihl*4) + ETH_HLEN;
            if(iph->protocol == IPPROTO_TCP)
            {
                p->push(buff);
                s->push(n);
            }
        }
    }

}

void proc(int no, queue<u_int8_t *> * p, queue<u_int8_t> * s)
{
    u_int8_t * ptr;

    int size = 0;
    int n = 0;
    int i = 0;
    while(1)
    {
        printf("INPUT : ");
        scanf("%d",&n);

        if(n == 1) {
        ptr = p->front();
        size = p->size();
        cout << "size : " << size << endl;
        cout << "MAC ADDRESS : ";
        for(int i = 0; i < size; i++){
            if(i % 12 == 0)
                cout << endl;
            printf("%x ", ptr[i]);
        }
        cout << endl;
        p->pop();
        }
    }
}
 
int main(){
    queue<u_int8_t *> * p;
    p = new queue<u_int8_t *>;
    queue<u_int8_t> * s;
    s = new queue<u_int8_t>;


    
    thread thread1(pcap, 1, p, s);
    thread thread2(proc, 2, p, s);
 
    thread1.join();
    thread2.join();
 
    return 0;
}
