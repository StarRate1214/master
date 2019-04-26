#include "ids.h"

#define CHECK_DF 0x4000
#define CHECK_MF 0x2000

struct UPacket{
	CTCP tcp;
	CUDP udp;
	CICMP icmp;
	UPacket() {}
	~UPacket() {}
};



int main(void)
{
	int sockfd, i, n;
	int j, k;
	UPacket packet;

	u_int8_t buff[ETH_FRAME_LEN]; 
	if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
    {
        perror("SOCKET ERROR");
        exit(1);
    }
	while(1){
		if ((n = recv(sockfd, buff, ETHER_MAX_LEN, 0)) < 0)
		{
        	perror("RECV ERROR");
        	exit(1);
		}
		printf("Total %d bytes received ...\n", n);
		
		puts("========== Ethernet Header ==========");
		struct ether_header *eh = (struct ether_header*)buff;
		switch(ntohs(eh->ether_type))
		{
			case ETHERTYPE_IP:
			{
				struct iphdr *iph = (struct iphdr*)&buff[ETH_HLEN];
				int headerlen = (iph->ihl*4) + ETH_HLEN;
				switch(iph->protocol)
				{
					case IPPROTO_TCP:
					{
						struct tcphdr *tcp = (struct tcphdr *)&buff[headerlen];

						// Ethernet
						packet.tcp.setDstMac(eh->ether_dhost);
						packet.tcp.setSrcMac(eh->ether_shost);
						packet.tcp.setEtherType(ntohs(eh->ether_type));

						// IP
						//struct sockaddr_in sadr, dadr;
						//sadr.sin_addr.s_addr = iph->saddr;
						//dadr.sin_addr.s_addr = iph->daddr;

						packet.tcp.setSrcIP(ntohl(iph->saddr));
						packet.tcp.setDstIP(ntohl(iph->daddr));
						packet.tcp.setTos(iph->tos);
						packet.tcp.setDontFrag(iph->frag_off&CHECK_DF);
						packet.tcp.setMoreFrag(iph->frag_off&CHECK_MF);
						packet.tcp.setTTL(iph->ttl);

						// TCP
						packet.tcp.setSrcPort(ntohs(tcp->source));
						packet.tcp.setDstPort(ntohs(tcp->dest));
						packet.tcp.setSeqNum(ntohs(tcp->seq));
						packet.tcp.setAckNum(ntohs(tcp->ack_seq));
						packet.tcp.setUrg(ntohs(tcp->urg));
						packet.tcp.setAck(ntohs(tcp->ack));
						packet.tcp.setPsh(ntohs(tcp->psh));
						packet.tcp.setRst(ntohs(tcp->rst));
						packet.tcp.setSyn(ntohs(tcp->syn));
						packet.tcp.setFin(ntohs(tcp->fin));
						packet.tcp.setWinSize(ntohs(tcp->window));

						std::cout << packet.tcp.getDstPort() << std::endl;
						std::cout << packet.tcp.getSrcIP() << std::endl;
						std::cout << (int)packet.tcp.getTTL() << std::endl;
						break;
					}
					case IPPROTO_UDP:
					{
						struct udphdr *udh = (struct udphdr *)&buff[headerlen];
						break;
					}
					case IPPROTO_ICMP:
					{
						struct icmphdr *ich = (struct icmphdr *)&buff[headerlen];
						break;
					}
				}
				break;
			}
			default:
			{
				break;
			}
		}
	}
	return 0;
}
