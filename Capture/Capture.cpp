#include "ids.h"

int main(void)
{
	int sockfd, i, n;
	uint8_t buff[ETH_FRAME_LEN]; 
	//raw socket 생성
	if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
    //if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0)
    {
        perror("SOCKET ERROR");
        exit(1);
    }
	while(1){
	// raw data 수신
		if ((n = recv(sockfd, buff, ETHER_MAX_LEN, 0)) < 0)
			return -1;
		printf("Total %d bytes received ...\n", n);
		


		// Ethernet header
		puts("========== Ethernet Header ==========");
		struct ether_header *eh = (struct ether_header*)buff;
		
		int j, k;
		printf("[ Dest MAC ] ");
		for(j=0; j<6; j++)
			printf("%02x ", eh->ether_dhost[j]);
		printf("\n[ Sour MAC ] ");
		for(j=0; j<6; j++)
			printf("%02x ", eh->ether_shost[j]);
		uint16_t ether_type = ntohs(eh->ether_type);
		printf("\n[ Eth TYPE ] 0x%04x\n", ether_type);



		switch (ether_type)
		{
			case ETHERTYPE_IP:
            {
				puts("========== IP Header ==========");
				struct iphdr *iph = (struct iphdr*)&buff[14];
				struct sockaddr_in sadr;
				sadr.sin_addr.s_addr = iph->saddr;
				struct sockaddr_in dadr;
				dadr.sin_addr.s_addr = iph->daddr;
				//uint8_t * iph_daddr[4] = &buff[30];
				printf("[ IP VER ] %x\n", iph->version);
				printf("[ IP LEN ] %d bytes\n", (iph->ihl)*4);
				printf("[ TTL    ] %d\n", iph->ttl);
				printf("[ PROTCO ] %d\n", iph->protocol);
				printf("[ CKKSUM ] %d\n", iph->check);
				printf("[ SRC IP ] %s\n", inet_ntoa(sadr.sin_addr));
				printf("[ DES IP ] %s\n", inet_ntoa(dadr.sin_addr));
				
				int pkhl = (iph->ihl*4) + 14;
				switch(iph->protocol)
				{
					case IPPROTO_ICMP:
                    {
						puts("========== ICMP Header ==========");
						struct icmphdr *ich = (struct icmphdr *)&buff[pkhl];
						printf("[ TYPE ] %d\n", ich->type);
						printf("[ CODE ] %d\n", ich->code);
						printf("[ CSUM ] %d\n", ich->checksum);
						break;
                    }
					case IPPROTO_TCP:
                    {
						puts("========== TCP Header ==========");
						struct tcphdr *tcp = (struct tcphdr *)&buff[pkhl];
						printf("[ SOUR PORT ] %d\n", ntohs(tcp->source));
						printf("[ DEST PORT ] %d\n", ntohs(tcp->dest));
						printf("[ SEQ NUM   ] %d\n", ntohs(tcp->seq));
						printf("[ ACK NUM   ] %d\n", ntohs(tcp->ack_seq));
						printf("[ DATA OFFS ] %d\n", ntohs(tcp->doff));
						//printf("[ RESERVED  ] %d\n", ntohs(tcp->res1));
						printf("[ FLAG      ] URG ACK PSH RST SYN FIN\n");
						printf("              %3d %3d %3d %3d %3d %3d\n", tcp->urg, tcp->ack, tcp->psh, tcp->rst, tcp->syn, tcp->fin);
						printf("[ WINDOWS   ] %d\n", ntohs(tcp->window));
						printf("[ CHECKSUM  ] %d\n", ntohs(tcp->check));
						//printf("[ URG PTR   ] %d\n", ntohs(tcp->urg_ptr));
                    }
						break;
					/*case IPPROTO_UDP:
						puts("========== UDP Header==========");
						struct udphdr *udh = (struct udphdr *)&buff[pkhl];
						printf("[ SOUR PORT ] %d\n", ntohs(udh->uh_sport));
						printf("[ DEST PORT ] %d\n", ntohs(udh->uh_dport));
						printf("[ UDP LEN   ] %d\n", ntohs(udh->uh_ulen));
						printf("[ UDP CSUM  ] %d\n", ntohs(udh->uh_sum));
						break;*/
				}
				break;
            }
			default:
            {
				printf("unhandled ether_type(0x%04x)\n", ether_type);
				break;
            }
		}

			


		// All packet data
		puts("========== Frame Data ==========");
		for(i=0; i<n; i++) {
			printf("%02x ", buff[i]);
			if(i%16==15)
				printf("\n");
		}
		printf("\n\n\n");
	}	
	
	
	return 0;
}
