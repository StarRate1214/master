#include "../HEADER/RuleEngine.h"

bool CRuleEngine::compareHeader(CRule rule)
{
	int ruleProtocol;
	u_int32_t packetSrcNetmask = 0;
	u_int32_t packetDesNetmask = 0;
	u_int16_t packetSrcPort = 0;
	u_int16_t packetDesPort = 0;

	//프로토콜 비교 후 분류
	//동시에 들어온 패킷의 출발지, 목적지 IP의 netmask 변환.
	switch (rule.GetProtocols())// Rule.h 에서 넘어온 string을 숫자로 바꿔줌
	{
	case "TCP":
		ruleProtocol = TCP;
		packetSrcNetmask = rule.GetSrcNetmask & packet.tcp.getSrcIP();
		packetDesNetmask = rule.GetDesNetmask & packet.tcp.getDstIP();
		packetSrcPort = packet.tcp.getSrcPort();
		packetDesPort = packet.tcp.getDstPort();
		break;
	case "UDP":
		ruleProtocol = UDP;
		packetSrcNetmask = rule.GetSrcNetmask & packet.udp.getSrcIP();
		packetDesNetmask = rule.GetDesNetmask & packet.udp.getDstIP();
		packetSrcPort = packet.udp.getSrcPort();
		packetDesPort = packet.udp.getDstPort();
		break;
	case "ICMP":
		ruleProtocol = ICMP;
		packetSrcNetmask = rule.GetSrcNetmask & packet.icmp.getSrcIP();
		packetDesNetmask = rule.GetDesNetmask & packet.icmp.getDstIP();
		break;
	}

	if (ruleProtocol != packet.protocol_type) return false;//프로토콜 타입확인

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
				case NOT: //
					if (rule.GetDesPort() == packetSrcPort) return false;
					break;
				case COMM: //단일 PORT
					if (rule.GetDesPort() != packetSrcPort) return false;
					break;
				}
				break;
			case NOT: //
				if (rule.GetSrcPort() == packetSrcPort)
				{
					switch (rule.GetDesPOpt())//역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetDesPort() == packetSrcPort) return false;
						break;
					case COMM: //단일 PORT
						if (rule.GetDesPort() != packetSrcPort) return false;
						break;
					}
				}
				break;
			case COMM: //단일 PORT
				if (rule.GetSrcPort() != packetSrcPort)
				{
					switch (rule.GetDesPOpt())//역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetDesPort() == packetSrcPort) return false;
						break;
					case COMM: //단일 PORT
						if (rule.GetDesPort() != packetSrcPort) return false;
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
					if (rule.GetSrcPort() == packetDesPort) return false;
					break;
				case COMM: //단일 PORT
					if (rule.GetSrcPort() != packetDesPort) return false;
					break;
				}
				break;
			case NOT: //
				if (rule.GetDesPort() == packetDesPort)
				{
					switch (rule.GetSrcPOpt())
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetSrcPort() == packetDesPort) return false;
						break;
					case COMM: //단일 PORT
						if (rule.GetSrcPort() != packetDesPort) return false;
						break;
					}
				}
				break;
			case COMM: //단일 PORT
				if (rule.GetDesPort() != packetDesPort)
				{
					switch (rule.GetSrcPOpt())
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetSrcPort() == packetDesPort) return false;
						break;
					case COMM: //단일 PORT
						if (rule.GetSrcPort() != packetDesPort) return false;
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
					if (rule.GetDesPort() == packetSrcPort) return false;
					break;
				case COMM: //단일 PORT
					if (rule.GetDesPort() != packetSrcPort) return false;
					break;
				}
				break;
			case NOT: //
				if (rule.GetSrcPort() == packetSrcPort)
				{
					switch (rule.GetDesPOpt())//출발지 PORT 역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetDesPort() == packetSrcPort) return false;
						break;
					case COMM: //단일 PORT
						if (rule.GetDesPort() != packetSrcPort) return false;
						break;
					}
				}
				break;
			case COMM: //단일 PORT
				if (rule.GetSrcPort() != packetSrcPort)
				{
					switch (rule.GetDesPOpt())//출발지 PORT 역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetDesPort() == packetSrcPort) return false;
						break;
					case COMM: //단일 PORT
						if (rule.GetDesPort() != packetSrcPort) return false;
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
					if (rule.GetSrcPort() == packetDesPort) return false;
					break;
				case COMM: //단일 PORT
					if (rule.GetSrcPort() != packetDesPort) return false;
					break;
				}
				break;
			case NOT: //
				if (rule.GetDesPort() == packetDesPort)
				{
					switch (rule.GetSrcPOpt())//목적지 PORT 역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetSrcPort() == packetDesPort) return false;
						break;
					case COMM: //단일 PORT
						if (rule.GetSrcPort() != packetDesPort) return false;
						break;
					}
				}
				break;
			case COMM: //단일 PORT
				if (rule.GetDesPort() != packetDesPort)
				{
					switch (rule.GetSrcPOpt())//목적지 PORT 역방향 비교
					{
					case ANY: //넘어감
						break;
					case NOT: //
						if (rule.GetSrcPort() == packetDesPort) return false;
						break;
					case COMM: //단일 PORT
						if (rule.GetSrcPort() != packetDesPort) return false;
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
				if (rule.GetSrcPort() == packetSrcPort) return false;
				break;
			case COMM: //단일 PORT
				if (rule.GetSrcPort() != packetSrcPort) return false;
				break;
			}

			switch (rule.GetDesPOpt())//목적지 PORT
			{
			case ANY: //넘어감
				break;
			case NOT: //
				if (rule.GetDesPort() == packetDesPort) return false;
				break;
			case COMM: //단일 PORT
				if (rule.GetDesPort() != packetDesPort) return false;
				break;
			}
			break;

		case UDP://단방향 UDP PORT비교
			switch (rule.GetSrcPOpt())//출발지 PORT
			{
			case ANY: //넘어감
				break;
			case NOT: //
				if (rule.GetSrcPort() == packetSrcPort) return false;
				break;
			case COMM: //단일 PORT
				if (rule.GetSrcPort() != packetSrcPort) return false;
				break;
			}

			switch (rule.GetDesPOpt())//목적지 PORT
			{
			case ANY: //넘어감
				break;
			case NOT: //
				if (rule.GetDesPort() == packetDesPort) return false;
				break;
			case COMM: //단일 PORT
				if (rule.GetDesPort() != packetDesPort) return false;
				break;
			}
			break;

		case ICMP://단방향 ICMP PORT비교
			switch (rule.GetSrcPOpt())//출발지 PORT
			{
			case ANY: //넘어감
				break;
			case NOT: //
				if (rule.GetSrcPort() == packetSrcPort) return false;
				break;
			case COMM: //단일 PORT
				if (rule.GetSrcPort() != packetSrcPort) return false;
				break;
			}

			switch (rule.GetDesPOpt())//목적지 PORT
			{
			case ANY: //넘어감
				break;
			case NOT: //
				if (rule.GetDesPort() == packetDesPort) return false;
				break;
			case COMM: //단일 PORT
				if (rule.GetDesPort() != packetDesPort) return false;
				break;
			}
			break;
		}
		break;
	}
	return true;
}