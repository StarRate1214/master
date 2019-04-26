#include "ids.h"

union UPacket{
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
						packet.tcp.setDstMac(eh->ether_dhost);
						packet.tcp.setSrcMac(eh->ether_shost);
						packet.tcp.setEtherType(ntohs(eh->ether_type)); 
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
