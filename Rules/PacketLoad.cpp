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
			packet.tcp.setSeqNum(th->seq);
			packet.tcp.setAckNum(th->ack_seq);
			packet.tcp.setUrg(th->urg);
			packet.tcp.setAck(th->ack);
			packet.tcp.setPsh(th->psh);
			packet.tcp.setRst(th->rst);
			packet.tcp.setSyn(th->syn);
			packet.tcp.setFin(th->fin);
			packet.tcp.setWinSize(th->window);

			//tcp 세그먼트 데이터를 packet.data_payload에 넣자
			u_int32_t seg_size = (u_int32_t)(iph->tot_len) - (u_int32_t)(iph->ihl) - (u_int32_t)(th->doff);
			packet.data_payload_size = seg_size;
			int payload_addr = headerlen + (th->doff * 4);
			packet.data_payload = new u_int8_t[seg_size]();
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
			u_int32_t msg_size = (u_int32_t)uh->len - sizeof(uh->source) - sizeof(uh->dest) - sizeof(uh->check);
			packet.data_payload_size = msg_size;
			int payload_addr = headerlen + 8; //8 = 2(source) + 2(dest) + 4(check)
			packet.data_payload = new u_int8_t[msg_size]();
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
			u_int32_t msg_size = (u_int32_t)iph->tot_len - sizeof(ich->type) - sizeof(ich->code) - sizeof(ich->checksum);
			packet.data_payload_size = msg_size;
			int payload_addr = headerlen + 4; //4 = 1(type) + 1(code) + 2(checksum)
			packet.data_payload = new u_int8_t[msg_size]();
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