#include "CInherit_CompareHeader.h"

bool CInherit_CompareHeader::CompareHeader(CRule rule)
{
    int ruleProtocol;
    u_int32_t packetSrcNetmask = 0;
    u_int32_t packetDesNetmask = 0;
    u_int16_t packetSrcPort = 0;
    u_int16_t packetDesPort = 0;

    //프로토콜 비교 후 분류
    //동시에 들어온 패킷의 출발지, 목적지 IP의 netmask 변환.
    switch (rule.GetProtocols()) // Rule.h 에서 넘어온 string을 숫자로 바꿔줌
    {
    case TCP:
        ruleProtocol = TCP;
        packetSrcNetmask = rule.GetSrcNetmask() & packet.ip.getSrcIP();
        packetDesNetmask = rule.GetDesNetmask() & packet.ip.getDstIP();
        packetSrcPort = packet.tcp.getSrcPort();
        packetDesPort = packet.tcp.getDstPort();
        break;
    case UDP:
        ruleProtocol = UDP;
        packetSrcNetmask = rule.GetSrcNetmask() & packet.ip.getSrcIP();
        packetDesNetmask = rule.GetDesNetmask() & packet.ip.getDstIP();
        packetSrcPort = packet.udp.getSrcPort();
        packetDesPort = packet.udp.getDstPort();
        break;
    case ICMP:
        ruleProtocol = ICMP;
        packetSrcNetmask = rule.GetSrcNetmask() & packet.ip.getSrcIP();
        packetDesNetmask = rule.GetDesNetmask() & packet.ip.getDstIP();
        break;
    }

    if (ruleProtocol != packet.protocol_type)
        return false; //프로토콜 타입확인

    //ip방향성 확인
    int headerdir;
    if (rule.GetDirOperator() == "<>") headerdir = true;
	else headerdir = false;




	//출발지 ip끼리 비교(ANY,NOT, COMM) && 목적지 ip끼리 비교(ANY,NOT, COMM)
	switch (headerdir)//방향성을 이용해 switch
	{
	case true:
		switch (ruleProtocol)//protocol을 이용해 switch
		{
		case TCP://양방향 TCP ip비교
			switch (rule.GetSrcIPOpt())//출발지 IP 정방향 비교
			{
			case ANY: //넘어감
				switch (rule.GetDesIPOpt())//역방향 비교
				{
				case ANY: //넘어감
					break;
				case NOT: //
					if (rule.GetDesIP() == packetSrcNetmask) return false;
					break;
				case COMM: //단일 ip
					if (rule.GetDesIP() != packetSrcNetmask) return false;
					break;
				}
				break;
			case NOT: //
				if (rule.GetSrcIP() == packetSrcNetmask)
				{
					switch (rule.GetDesIPOpt())//역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetDesIP() == packetSrcNetmask) return false;
						break;
					case COMM: //단일 ip
						if (rule.GetDesIP() != packetSrcNetmask) return false;
						break;
					}
				}
				break;
			case COMM: //단일 ip
				if (rule.GetSrcIP() != packetSrcNetmask)
				{
					switch (rule.GetDesIPOpt())//역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetDesIP() == packetSrcNetmask) return false;
						break;
					case COMM: //단일 ip
						if (rule.GetDesIP() != packetSrcNetmask) return false;
						break;
					}
				}
				break;
			}


			switch (rule.GetDesIPOpt())//목적지 IP 정방향 비교
			{
			case ANY: //넘어감
				switch (rule.GetSrcIPOpt())
				{
				case ANY: //넘어감
					break;
				case NOT: //
					if (rule.GetSrcIP() == packetDesNetmask) return false;
					break;
				case COMM: //단일 ip
					if (rule.GetSrcIP() != packetDesNetmask) return false;
					break;
				}
				break;
			case NOT: //
				if (rule.GetDesIP() == packetDesNetmask)
				{
					switch (rule.GetSrcIPOpt())
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetSrcIP() == packetDesNetmask) return false;
						break;
					case COMM: //단일 ip
						if (rule.GetSrcIP() != packetDesNetmask) return false;
						break;
					}
				}
				break;
			case COMM: //단일 ip
				if (rule.GetDesIP() != packetDesNetmask)
				{
					switch (rule.GetSrcIPOpt())
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetSrcIP() == packetDesNetmask) return false;
						break;
					case COMM: //단일 ip
						if (rule.GetSrcIP() != packetDesNetmask) return false;
						break;
					}
				}
				break;
			}
			break;


		case UDP://양방향 UDP ip비교
			switch (rule.GetSrcIPOpt())//출발지 IP 정방향 비교
			{
			case ANY: //넘어감
				switch (rule.GetDesIPOpt())//출발지 IP 역방향 비교
				{
				case ANY: //넘어감
					break;
				case NOT: //
					if (rule.GetDesIP() == packetSrcNetmask) return false;
					break;
				case COMM: //단일 ip
					if (rule.GetDesIP() != packetSrcNetmask) return false;
					break;
				}
				break;
			case NOT: //
				if (rule.GetSrcIP() == packetSrcNetmask)
				{
					switch (rule.GetDesIPOpt())//출발지 IP 역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetDesIP() == packetSrcNetmask) return false;
						break;
					case COMM: //단일 ip
						if (rule.GetDesIP() != packetSrcNetmask) return false;
						break;
					}
				}
				break;
			case COMM: //단일 ip
				if (rule.GetSrcIP() != packetSrcNetmask)
				{
					switch (rule.GetDesIPOpt())//출발지 IP 역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetDesIP() == packetSrcNetmask) return false;
						break;
					case COMM: //단일 ip
						if (rule.GetDesIP() != packetSrcNetmask) return false;
						break;
					}
				}
				break;
			}


			switch (rule.GetDesIPOpt())//목적지 IP 정방향 비교
			{
			case ANY: //넘어감
				switch (rule.GetSrcIPOpt())//목적지 IP 역방향 비교
				{
				case ANY: //넘어감
					break;
				case NOT: //
					if (rule.GetSrcIP() == packetDesNetmask) return false;
					break;
				case COMM: //단일 ip
					if (rule.GetSrcIP() != packetDesNetmask) return false;
					break;
				}
				break;
			case NOT: //
				if (rule.GetDesIP() == packetDesNetmask)
				{
					switch (rule.GetSrcIPOpt())//목적지 IP 역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetSrcIP() == packetDesNetmask) return false;
						break;
					case COMM: //단일 ip
						if (rule.GetSrcIP() != packetDesNetmask) return false;
						break;
					}
				}
				break;
			case COMM: //단일 ip
				if (rule.GetDesIP() != packetDesNetmask)
				{
					switch (rule.GetSrcIPOpt())//목적지 IP 역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetSrcIP() == packetDesNetmask) return false;
						break;
					case COMM: //단일 ip
						if (rule.GetSrcIP() != packetDesNetmask) return false;
						break;
					}
				}
			}
			break;



		case ICMP://양방향 ICMP ip비교
			break;
		}
		break;

	case false:
		switch (ruleProtocol)
		{
		case TCP://단방향 TCP ip비교
			switch (rule.GetSrcIPOpt())//출발지 IP
			{
			case ANY: //넘어감
				break;
			case NOT: //
				if (rule.GetSrcIP() == packetSrcNetmask) return false;
				break;
			case COMM: //단일 ip
				if (rule.GetSrcIP() != packetSrcNetmask) return false;
				break;
			}

			switch (rule.GetDesIPOpt())//목적지 IP
			{
			case ANY: //넘어감
				break;
			case NOT: //
				if (rule.GetDesIP() == packetDesNetmask) return false;
				break;
			case COMM: //단일 ip
				if (rule.GetDesIP() != packetDesNetmask) return false;
				break;
			}
			break;

		case UDP://단방향 UDP ip비교
			switch (rule.GetSrcIPOpt())//출발지 IP
			{
			case ANY: //넘어감
				break;
			case NOT: //
				if (rule.GetSrcIP() == packetSrcNetmask) return false;
				break;
			case COMM: //단일 ip
				if (rule.GetSrcIP() != packetSrcNetmask) return false;
				break;
			}

			switch (rule.GetDesIPOpt())//목적지 IP
			{
			case ANY: //넘어감
				break;
			case NOT: //
				if (rule.GetDesIP() == packetDesNetmask) return false;
				break;
			case COMM: //단일 ip
				if (rule.GetDesIP() != packetDesNetmask) return false;
				break;
			}
			break;

		case ICMP://단방향 ICMP ip비교
			switch (rule.GetSrcIPOpt())//출발지 IP
			{
			case ANY: //넘어감
				break;
			case NOT: //
				if (rule.GetSrcIP() == packetSrcNetmask) return false;
				break;
			case COMM: //단일 ip
				if (rule.GetSrcIP() != packetSrcNetmask) return false;
				break;
			}

			switch (rule.GetDesIPOpt())//목적지 IP
			{
			case ANY: //넘어감
				break;
			case NOT: //
				if (rule.GetDesIP() == packetDesNetmask) return false;
				break;
			case COMM: //단일 ip
				if (rule.GetDesIP() != packetDesNetmask) return false;
				break;
			}
			break;
		}
		break;
	}



	//출발지 port끼리 비교(ANY,NOT, COMM), 목적지 port끼리 비교(ANY,NOT, COMM)
	switch (headerdir)//방향성을 이용해 switch
	{
	case true:
		switch (ruleProtocol)//protocol을 이용해 switch
		{
		case TCP://양방향 TCP PORT비교
			switch (rule.GetSrcPOpt())//출발지 PORT 정방향 비교
			{
			case ANY:
				switch (rule.GetDesPOpt())//역방향 비교
				{
				case ANY: //넘어감
					break;
				case NOT: //제외
					if (PortCompare(rule.GetDesPort(), packetSrcPort)==true) return false;
					break;
				case COMM: // PORT
					if (PortCompare(rule.GetDesPort(), packetSrcPort)==false) return false;
					break;
				}
				break;
			case NOT: //
				if (PortCompare(rule.GetSrcPort(), packetSrcPort)==true)
				{
					switch (rule.GetDesPOpt())//역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (PortCompare(rule.GetDesPort(), packetSrcPort)==true) return false;
						break;
					case COMM: //단일 PORT
						if (PortCompare(rule.GetDesPort(), packetSrcPort)==false) return false;
						break;
					}
				}
				break;
			case COMM: //단일 PORT
				if (PortCompare(rule.GetSrcPort(), packetSrcPort)==false)
				{
					switch (rule.GetDesPOpt())//역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (PortCompare(rule.GetDesPort(), packetSrcPort)==true) return false;
						break;
					case COMM: //단일 PORT
						if (PortCompare(rule.GetDesPort(), packetSrcPort)==false) return false;
						break;
					}
				}
				break;
			}


			switch (rule.GetDesPOpt())//목적지 PORT 정방향 비교
			{
			case ANY:
				switch (rule.GetSrcPOpt())
				{
				case ANY: //넘어감
					break;
				case NOT: //
					if (PortCompare(rule.GetSrcPort(), packetDesPort)==true) return false;
					break;
				case COMM: //단일 PORT
					if (PortCompare(rule.GetSrcPort(), packetDesPort)==false) return false;
					break;
				}
				break;
			case NOT: //
				if (PortCompare(rule.GetDesPort(), packetDesPort)==true)
				{
					switch (rule.GetSrcPOpt())
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (PortCompare(rule.GetSrcPort(), packetDesPort)==true) return false;
						break;
					case COMM: //단일 PORT
						if (PortCompare(rule.GetSrcPort(), packetDesPort)==false) return false;
						break;
					}
				}
				break;
			case COMM: //단일 PORT
				if (PortCompare(rule.GetDesPort(), packetDesPort)==false)
				{
					switch (rule.GetSrcPOpt())
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (PortCompare(rule.GetSrcPort(), packetDesPort)==true) return false;
						break;
					case COMM: //단일 PORT
						if (PortCompare(rule.GetSrcPort(), packetDesPort)==false) return false;
						break;
					}
				}
				break;
			}
			break;


		case UDP://양방향 UDP PORT비교
			switch (rule.GetSrcPOpt())//출발지 PORT 정방향 비교
			{
			case ANY:
				switch (rule.GetDesPOpt())//출발지 PORT 역방향 비교
				{
				case ANY: //넘어감
					break;
				case NOT: //
					if (PortCompare(rule.GetDesPort(), packetSrcPort)==true) return false;
					break;
				case COMM: //단일 PORT
					if (PortCompare(rule.GetDesPort(), packetSrcPort)==false) return false;
					break;
				}
				break;
			case NOT: //
				if (PortCompare(rule.GetSrcPort(), packetSrcPort)==true)
				{
					switch (rule.GetDesPOpt())//출발지 PORT 역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (PortCompare(rule.GetDesPort(), packetSrcPort)==true) return false;
						break;
					case COMM: //단일 PORT
						if (PortCompare(rule.GetDesPort(), packetSrcPort)==false) return false;
						break;
					}
				}
				break;
			case COMM: //단일 PORT
				if (PortCompare(rule.GetSrcPort(), packetSrcPort)==false)
				{
					switch (rule.GetDesPOpt())//출발지 PORT 역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (PortCompare(rule.GetDesPort(), packetSrcPort)==true) return false;
						break;
					case COMM: //단일 PORT
						if (PortCompare(rule.GetDesPort(), packetSrcPort)==false) return false;
						break;
					}
				}
				break;
			}


			switch (rule.GetDesPOpt())//목적지 PORT 정방향 비교
			{
			case ANY:
				switch (rule.GetSrcPOpt())//목적지 PORT 역방향 비교
				{
				case ANY: //넘어감
					break;
				case NOT: //
					if (PortCompare(rule.GetSrcPort(), packetDesPort)==true) return false;
					break;
				case COMM: //단일 PORT
					if (PortCompare(rule.GetSrcPort(), packetDesPort)==false) return false;
					break;
				}
				break;
			case NOT: //
				if (PortCompare(rule.GetDesPort(), packetDesPort)==true)
				{
					switch (rule.GetSrcPOpt())//목적지 PORT 역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (PortCompare(rule.GetSrcPort(), packetDesPort)==true) return false;
						break;
					case COMM: //단일 PORT
						if (PortCompare(rule.GetSrcPort(), packetDesPort)==false) return false;
						break;
					}
				}
				break;
			case COMM: //단일 PORT
				if (PortCompare(rule.GetDesPort(), packetDesPort)==false)
				{
					switch (rule.GetSrcPOpt())//목적지 PORT 역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (PortCompare(rule.GetSrcPort(), packetDesPort)==true) return false;
						break;
					case COMM: //단일 PORT
						if (PortCompare(rule.GetSrcPort(), packetDesPort)==false) return false;
						break;
					}
				}
			}
			break;



		case ICMP://양방향 ICMP PORT비교 없음
			break;
		}
		break;

	case false:
		switch (ruleProtocol)
		{
		case TCP://단방향 TCP PORT비교
			switch (rule.GetSrcPOpt())//출발지 PORT
			{
			case ANY: //넘어감
				break;
			case NOT: //
				if (PortCompare(rule.GetSrcPort(), packetSrcPort)==true) return false;
				break;
			case COMM: //단일 PORT
				if (PortCompare(rule.GetSrcPort(), packetSrcPort)==false) return false;
				break;
			}

			switch (rule.GetDesPOpt())//목적지 PORT
			{
			case ANY: //넘어감
				break;
			case NOT: //
				if (PortCompare(rule.GetDesPort(), packetDesPort)==true) return false;
				break;
			case COMM: //단일 PORT
				if (PortCompare(rule.GetDesPort(), packetDesPort)==false) return false;
				break;
			}
			break;

		case UDP://단방향 UDP PORT비교
			switch (rule.GetSrcPOpt())//출발지 PORT
			{
			case ANY: //넘어감
				break;
			case NOT: //
				if (PortCompare(rule.GetSrcPort(), packetSrcPort)==true) return false;
				break;
			case COMM: //단일 PORT
				if (PortCompare(rule.GetSrcPort(), packetSrcPort)==false) return false;
				break;
			}

			switch (rule.GetDesPOpt())//목적지 PORT
			{
			case ANY: //넘어감
				break;
			case NOT: //
				if (PortCompare(rule.GetDesPort(), packetDesPort)==true) return false;
				break;
			case COMM: //단일 PORT
				if (PortCompare(rule.GetDesPort(), packetDesPort)==false) return false;
				break;
			}
			break;

		case ICMP://단방향 ICMP PORT비교
			break;
		}
		break;
	}
	return true;
}

bool CInherit_CompareHeader::PortCompare(std::vector<u_int16_t> rulePort, u_int16_t packetPort)
{
    if (rulePort.size()==1)
    {
       if (rulePort[0]== packetPort) return true;
    }
    else
    {
         for (int i = rulePort[0];  i < rulePort[1]; i++)
        {
            if (i == packetPort) return true;
        }
    }
    return false;
}
