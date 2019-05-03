#include "RuleEngine.h"
#include "ids.h"
void TcpIpCompare();
void UdpIpCompare();
void IcmpIpCompare();
void TcpPortCompare();
void UdpPortCompare();
void IcmpPortCompare();
void TcpOptionCompare();
void UdpOptionCompare();
void IcmpOptionCompare();

int CRuleEngine::Compare()
{
 static int m_srcIp,m_srcPort,m_dstIp,m_dstPort;
        for(unsigned i = 0; i < rules.size(); ++i) 
        {
          m_srcIp=0, m_srcPort=0, m_dstIp=0, m_dstPort=0;
               switch(packet.protocol_type)//패킷의 프로토콜 비교
               {
                 case TCP:
                  TcpIpCompare();
                  if (m_srcIp&&m_srcPort&&m_dstIp&&m_dstPort) return i;
                 break;
                 case UDP:
                  UdpIpCompare();
                  if (m_srcIp&&m_srcPort&&m_dstIp&&m_dstPort) return i;
                 break;
                 case ICMP:
                  IcmpIpCompare();
                  if (m_srcIp&&m_srcPort&&m_dstIp&&m_dstPort) return i;
                 break;
               }
        } 
}
u_int32_t Cidr()
{
  u_int32_t mask=0x0000;
  
  for(unsigned i = 0; i < rules(i).GetSrcNetmask(); ++i) 
  {
	  mask++;
	  mask<<1;
  }
  
}

void TcpIpCompare()
{
  switch(rules(i).GetSrcAOpt())
  {
    case ANY://바로 포트 옵션으로
    m_srcIp=1;
    break;
    case NOT://
    if(rules(i).GetSrcIP()!=packet.tcp.getSrcIP())
    {

    }
    }
    break;
    case COMM://단일 ip
    if(rules(i).GetSrcIP()==packet.tcp.getSrcIP()) 
    {
      
    }
    break;
  }
  switch(rules(i).GetDesAOpt())
  {
    case ANY://바로 포트 옵션으로
    m_dstIp=1;
    break;
    case NOT://
    if(rules(i).GetDstIP()!=packet.tcp.getDstIP())
    {}
    }
    break;
    case COMM://단일 ip
    if(rules(i).GetDstIP()==packet.tcp.getDstIP()) 
    {

    }
    break;
  }
}
void UdpIpCompare()
{
  switch(rules(i).GetSrcAOpt())
  {
    case ANY://바로 포트 옵션으로
    m_srcIp=1;
    break;
    case NOT://
    if(rules(i).GetSrcIP()!=packet.udp.getSrcIP())
    {

    }
    break;
    case COMM://단일 ip
    if(rules(i).GetSrcIP()==packet.udp.getSrcIP())
    {

    }
    break;
  }
  switch(rules(i).GetDesAOpt())
  {
    case ANY://바로 포트 옵션으로
    m_dstIp=1;
    break;
    case NOT://
    if(rules(i).GetDstIP()!=packet.udp.getDstIP()) 
    {

    }
    break;
    case COMM://단일 ip
    if(rules(i).GetDstIP()==packet.udp.getDstIP()) 
    {

    }
    break;
  }
}
void IcmpIpCompare()
{
  switch(rules(i).GetSrcAOpt())
  {
    case ANY://바로 포트 옵션으로
    m_srcIp=1;
    break;
    case NOT://
    if(rules(i).GetSrcIP()!=packet.icmp.getSrcIP()) 
    {
      
    }
    break;
    case COMM://단일 ip
    if(rules(i).GetSrcIP()==packet.icmp.getSrcIP())
	{

	}
    break;
  }
   switch(rules(i).GetDesAOpt())
  {
    case ANY://바로 포트 옵션으로
    m_dstIp=1;
    break;
    case NOT://
    if(rules(i).GetDstIP()!=packet.icmp.getDstIP()) 
	{

	}
    break;
    case COMM://단일 ip
    if(rules(i).GetDstIP()==packet.icmp.getDstIP())
	{

	}
    break;
  }
}
void TcpPortCompare()
{

}
void UdpPortCompare()
{

}

void TcpOptionCompare()
{

}
void UdpOptionCompare()
{

}
void IcmpOptionCompare()
{

}