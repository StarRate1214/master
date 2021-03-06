#include "RuleEngine.h"
void CRuleEngine::PacketLoad(CRawpacket *rwpack)
{
	u_int8_t *buff = rwpack->getPacket();
	pcap_pkthdr pkthdr = rwpack->getPkthdr();
	packet.time = (time_t)pkthdr.ts.tv_sec;

	struct ether_header *eh = (struct ether_header *)buff;
	switch (ntohs(eh->ether_type))
	{
	case ETHERTYPE_IP:
	{
		struct iphdr *iph = (struct iphdr *)&buff[ETH_HLEN];
		int headerlen = (iph->ihl * 4) + ETH_HLEN;

		// IP
		packet.ip.setSrcIP(iph->saddr);
		packet.ip.setDstIP(iph->daddr);
		packet.ip.setTos(iph->tos);
		packet.ip.setDontFrag(iph->frag_off & CHECK_DF);
		packet.ip.setMoreFrag(iph->frag_off & CHECK_MF);
		packet.ip.setTTL(iph->ttl);

		switch (iph->protocol)
		{
		case IPPROTO_TCP:
		{
			struct tcphdr *th = (struct tcphdr *)&buff[headerlen];
			/*					
                    // Ethernet
					packet.tcp.setDstMac(eh->ether_dhost);
					packet.tcp.setSrcMac(eh->ether_shost);
					packet.tcp.setEtherType(ntohs(eh->ether_type));
					
                    // IP
					packet.tcp.setSrcIP(ntohl(iph->saddr));
					packet.tcp.setDstIP(ntohl(iph->daddr));
					packet.tcp.setTos(iph->tos);
					packet.tcp.setDontFrag(ntohs(iph->frag_off)&CHECK_DF);
					packet.tcp.setMoreFrag(ntohs(iph->frag_off)&CHECK_MF);
					packet.tcp.setTTL(iph->ttl);
*/
			// TCP
			packet.tcp.setSrcPort(th->source);
			packet.tcp.setDstPort(th->dest);
			packet.tcp.setSeqNum(ntohl(th->seq));
			packet.tcp.setAckNum(ntohl(th->ack_seq));
			packet.tcp.setUrg(th->urg);
			packet.tcp.setAck(th->ack);
			packet.tcp.setPsh(th->psh);
			packet.tcp.setRst(th->rst);
			packet.tcp.setSyn(th->syn);
			packet.tcp.setFin(th->fin);
			packet.tcp.setWinSize(ntohs(th->window));

			//tcp 세그먼트 데이터를 packet.data_payload에 넣자
			u_int32_t seg_size = (u_int32_t)ntohs(iph->tot_len) - headerlen - (u_int32_t)(th->doff * 4) + ETH_HLEN;
			/*
			std::cout<<std::endl<<"D_seg_size: "<<seg_size<<std::endl;
			std::cout<<"D_ntohs(iph->tot_len): "<<ntohs(iph->tot_len)<<std::endl;
			std::cout<<"D_headerlen: "<<headerlen<<std::endl;
			std::cout<<"D_th->doff: "<<th->doff<<std::endl;
			*/
			packet.data_payload_size = seg_size;
			int payload_addr = headerlen + (th->doff * 4);
			packet.data_payload = new u_int8_t[seg_size+1]();//pcre에서 사용할 null문자 추가하는 공간1
			packet.data_payload[packet.data_payload_size]='\0';//pcre에서 사용할 null문자 추가
			packet.protocol_type = TCP;

			for (int i = 0; i < seg_size; i++)
				packet.data_payload[i] = buff[payload_addr + i];

			break;
		}
		case IPPROTO_UDP:
		{
			struct udphdr *uh = (struct udphdr *)&buff[headerlen];
			/*					
                    // Ethernet
					packet.udp.setDstMac(eh->ether_dhost);
					packet.udp.setSrcMac(eh->ether_shost);
					packet.udp.setEtherType(ntohs(eh->ether_type));

					// IP
					packet.udp.setSrcIP(ntohl(iph->saddr));
					packet.udp.setDstIP(ntohl(iph->daddr));
					packet.udp.setTos(iph->tos);
					packet.udp.setDontFrag(ntohs(iph->frag_off)&CHECK_DF);
					packet.udp.setMoreFrag(ntohs(iph->frag_off)&CHECK_MF);
					packet.udp.setTTL(iph->ttl);
*/
			// UDP
			packet.udp.setSrcPort(uh->source);
			packet.udp.setDstPort(uh->dest);

			//udp 메세지 데이터를 packet.data_payload에 넣자
			u_int32_t msg_size = (u_int32_t)ntohs(uh->len) - 8;
			/*
			std::cout<<"U_\nU_ntohs(iph->tot_len): "<<ntohs(iph->tot_len)<<std::endl;
			std::cout<<std::endl<<"U_msg_size: "<<msg_size<<std::endl;
			std::cout<<"U_ntohs(uh->len) "<<ntohs(uh->len)<<std::endl;
			*/
			packet.data_payload_size = msg_size;
			int payload_addr = headerlen + 8; //8 = 2(source) + 2(dest) + 4(check)
			packet.data_payload = new u_int8_t[msg_size+1]();//pcre에서 사용할 null문자 추가하는 공간1
			packet.data_payload[packet.data_payload_size]='\0';//pcre에서 사용할 null문자 추가
			packet.protocol_type = UDP;

			for (int i = 0; i < msg_size; i++)
				packet.data_payload[i] = buff[payload_addr + i];

			break;
		}
		case IPPROTO_ICMP:
		{
			struct icmphdr *ich = (struct icmphdr *)&buff[headerlen];
			/*					
                    // Ethernet
					packet.icmp.setDstMac(eh->ether_dhost);
					packet.icmp.setSrcMac(eh->ether_shost);
					packet.icmp.setEtherType(ntohs(eh->ether_type));

					// IP
					packet.icmp.setSrcIP(ntohl(iph->saddr));
					packet.icmp.setDstIP(ntohl(iph->daddr));
					packet.icmp.setTos(iph->tos);
					packet.icmp.setDontFrag(ntohs(iph->frag_off)&CHECK_DF);
					packet.icmp.setMoreFrag(ntohs(iph->frag_off)&CHECK_MF);
					packet.icmp.setTTL(iph->ttl);
*/
			// ICMP
			packet.icmp.setICMPtype(ich->type);
			packet.icmp.setICMPcode(ich->code);

			//icmp msg데이터를 packet.data_payload에 넣자
			u_int32_t msg_size = (u_int32_t)ntohs(iph->tot_len) - (iph->ihl * 4) - 8;

			//std::cout<<"I_msg_size: "<<msg_size<<std::endl;

			packet.data_payload_size = msg_size;
			int payload_addr = headerlen + 8; //8 = 1(type) + 1(code) + 2(checksum) + 2(identifier) +2(seqNum)

			packet.data_payload = new u_int8_t[msg_size+1]();//pcre에서 사용할 null문자 추가하는 공간1
			packet.data_payload[packet.data_payload_size]='\0';//pcre에서 사용할 null문자 추가
			packet.protocol_type = ICMP;

			for (int i = 0; i < msg_size; i++)
				packet.data_payload[i] = buff[payload_addr + i];
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