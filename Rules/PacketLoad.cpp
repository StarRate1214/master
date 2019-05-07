#include "RuleEngine.h"
#include "rawpacket.h"
void CRuleEngine::PacketLoad(CRawpacket *rwpack)
{
	u_int8_t *buff = rwpack->getPacket();
	packet.time = rwpack->getTime;

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
					packet.tcp.setDontFrag(ntohs(iph->frag_off)&CHECK_DF);
					packet.tcp.setMoreFrag(ntohs(iph->frag_off)&CHECK_MF);
					packet.tcp.setTTL(iph->ttl);
					
                    // TCP
					packet.tcp.setSrcPort(ntohs(th->source));
					packet.tcp.setDstPort(ntohs(th->dest));
					packet.tcp.setSeqNum(ntohl(th->seq));
					packet.tcp.setAckNum(ntohl(th->ack_seq));
					packet.tcp.setUrg(ntohs(th->urg));
					packet.tcp.setAck(ntohs(th->ack));
					packet.tcp.setPsh(ntohs(th->psh));
					packet.tcp.setRst(ntohs(th->rst));
					packet.tcp.setSyn(ntohs(th->syn));
					packet.tcp.setFin(ntohs(th->fin));
					packet.tcp.setWinSize(ntohs(th->window));

                    //tcp 세그먼트 데이터를 packet.data_payload에 넣자
                    int payload_addr = headerlen+(th->doff*4);
					u_int32_t seg_size = rwpack->getSize() - payload_addr;
					packet.data_payload_size = seg_size;

                    packet.data_payload = new u_int8_t[seg_size]();
					packet.protocol_type = TCP;

                    for(int i=0; i<seg_size;i++)
						packet.data_payload[i] = buff[payload_addr+i];
					
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
					packet.udp.setDontFrag(ntohs(iph->frag_off)&CHECK_DF);
					packet.udp.setMoreFrag(ntohs(iph->frag_off)&CHECK_MF);
					packet.udp.setTTL(iph->ttl);

					// UDP
					packet.udp.setSrcPort(ntohs(uh->source));
					packet.udp.setDstPort(ntohs(uh->dest));

					//udp 메세지 데이터를 packet.data_payload에 넣자
					int payload_addr = headerlen+8;//8 = 2(source) + 2(dest) + 4(check)

                    u_int32_t msg_size = rwpack->getSize() - payload_addr; //udp헤더는 8고정
					packet.data_payload_size = msg_size;
                    
                    packet.data_payload = new u_int8_t[msg_size]();
					packet.protocol_type = UDP;

                    for(int i=0; i<msg_size;i++)
						packet.data_payload[i] = buff[payload_addr+i];
					
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
					packet.icmp.setDontFrag(ntohs(iph->frag_off)&CHECK_DF);
					packet.icmp.setMoreFrag(ntohs(iph->frag_off)&CHECK_MF);
					packet.icmp.setTTL(iph->ttl);

					// ICMP
					packet.icmp.setICMPtype(ich->type);
					packet.icmp.setICMPcode(ich->code);

					//icmp msg데이터를 packet.data_payload에 넣자
					int payload_addr = headerlen+8;// icmp헤더크기 8고정

                    u_int32_t msg_size = rwpack->getSize() - payload_addr; //icmp헤더는 8고정
					packet.data_payload_size = msg_size;

                    packet.data_payload = new u_int8_t[msg_size]();
					packet.protocol_type = ICMP;

                    for(int i=0; i<msg_size;i++)
						packet.data_payload[i] = buff[payload_addr+i];
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