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
	int sockfd, n;
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
						struct tcphdr *th = (struct tcphdr *)&buff[headerlen];

						// Ethernet
						packet.tcp.setDstMac(eh->ether_dhost);
						packet.tcp.setSrcMac(eh->ether_shost);
						packet.tcp.setEtherType(ntohs(eh->ether_type));

						// IP
						packet.tcp.setSrcIP(ntohl(iph->saddr));
						packet.tcp.setDstIP(ntohl(iph->daddr));
						packet.tcp.setTos(iph->tos);
						packet.tcp.setDontFrag(iph->frag_off&CHECK_DF);
						packet.tcp.setMoreFrag(iph->frag_off&CHECK_MF);
						packet.tcp.setTTL(iph->ttl);

						// TCP
						packet.tcp.setSrcPort(ntohs(th->source));
						packet.tcp.setDstPort(ntohs(th->dest));
						packet.tcp.setSeqNum(ntohs(th->seq));
						packet.tcp.setAckNum(ntohs(th->ack_seq));
						packet.tcp.setUrg(ntohs(th->urg));
						packet.tcp.setAck(ntohs(th->ack));
						packet.tcp.setPsh(ntohs(th->psh));
						packet.tcp.setRst(ntohs(th->rst));
						packet.tcp.setSyn(ntohs(th->syn));
						packet.tcp.setFin(ntohs(th->fin));
						packet.tcp.setWinSize(ntohs(th->window));
						break;
					}
					case IPPROTO_UDP:
					{
						struct udphdr *uh = (struct udphdr *)&buff[headerlen];
						// Ethernet
						packet.udp.setDstMac(eh->ether_dhost);
						packet.udp.setSrcMac(eh->ether_shost);
						packet.udp.setEtherType(ntohs(eh->ether_type));

						// IP
						packet.udp.setSrcIP(ntohl(iph->saddr));
						packet.udp.setDstIP(ntohl(iph->daddr));
						packet.udp.setTos(iph->tos);
						packet.udp.setDontFrag(iph->frag_off&CHECK_DF);
						packet.udp.setMoreFrag(iph->frag_off&CHECK_MF);
						packet.udp.setTTL(iph->ttl);

						// UDP
						
						break;
					}
					case IPPROTO_ICMP:
					{
						struct icmphdr *ich = (struct icmphdr *)&buff[headerlen];
						// Ethernet
						packet.icmp.setDstMac(eh->ether_dhost);
						packet.icmp.setSrcMac(eh->ether_shost);
						packet.icmp.setEtherType(ntohs(eh->ether_type));

						// IP
						packet.icmp.setSrcIP(ntohl(iph->saddr));
						packet.icmp.setDstIP(ntohl(iph->daddr));
						packet.icmp.setTos(iph->tos);
						packet.icmp.setDontFrag(iph->frag_off&CHECK_DF);
						packet.icmp.setMoreFrag(iph->frag_off&CHECK_MF);
						packet.icmp.setTTL(iph->ttl);

						// ICMP
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
