#include "CompareHeader.h"

bool CCompareHeader::CompareHeader(CRule rule)
{
    int ruleProtocol = rule.GetProtocols();

        if (rule.GetProtocols() != packet.protocol_type)
        return false; //프로토콜 타입확인

    u_int32_t packetSrcNetmask = 0;
    u_int32_t packetDesNetmask = 0;
    u_int16_t packetSrcPort = 0;
    u_int16_t packetDesPort = 0;

        packetSrcNetmask = rule.GetSrcNetmask() & packet.ip.getSrcIP();
        packetDesNetmask = rule.GetDesNetmask() & packet.ip.getDstIP();
    //프로토콜 비교 후 분류
    //동시에 들어온 패킷의 출발지, 목적지 IP의 netmask 변환.
    switch (ruleProtocol) // Rule.h 에서 넘어온 string을 숫자로 바꿔줌
    {
    case TCP:
        packetSrcPort = packet.tcp.getSrcPort();
        packetDesPort = packet.tcp.getDstPort();
        break;
    case UDP:
        packetSrcPort = packet.udp.getSrcPort();
        packetDesPort = packet.udp.getDstPort();
        break;
    case ICMP:
        break;
    }



    //ip방향성 확인
    int headerdir;
    if (rule.GetDirOperator() == "<>") headerdir = false;
	else headerdir = true;

    if (headerdir)
    {
        if(CompareDirection(rule, packetSrcNetmask, packetSrcPort, packetDesNetmask, packetDesPort))
        return true;
        else
        return false;
        
    }
    else
    {
        if(CompareDirection(rule, packetSrcNetmask, packetSrcPort, packetDesNetmask, packetDesPort))
        return true;
        else
        {
            packetSrcNetmask = rule.GetDesNetmask() & packet.ip.getSrcIP();
            packetDesNetmask = rule.GetSrcNetmask() & packet.ip.getDstIP();
            if(CompareDirection(rule, packetDesNetmask, packetDesPort, packetSrcNetmask, packetSrcPort))
            return true;
            else return false;
        }
    }
}

bool CCompareHeader::CompareDirection(CRule rule, u_int32_t packetNetmaskA, u_int16_t packetPortA, u_int32_t packetNetmaskB, u_int16_t packetPortB)
{
    switch (rule.GetSrcIPOpt())//룰의 IP 출발지 ANY NOT COMM
	{
	case ANY: //넘어감
		switch (rule.GetDesIPOpt())//룰의 IP 목적지 ANY NOT COMM
		{
		case ANY: //넘어감
			if (rule.GetProtocols() == ICMP)
			{
				return true;
			}
			else
			{
				switch (rule.GetSrcPOpt())//룰의 포트 출발지 ANY NOT COMM
				{
				case ANY:
					switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
					{
					case ANY:
						return true;
						break;
					case NOT:
						if (!PortCompare(rule.GetDesPort(), packetPortB))
						{
							return true;
						}
						else
						{
							return false;
						}
						break;
					case COMM:
						if (PortCompare(rule.GetDesPort(), packetPortB))
						{
							return true;
						}
						else
						{
							return false;
						}
						break;
					}
					break;
				case NOT:
					if (!PortCompare(rule.GetSrcPort(), packetPortA))
					{
						switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
						{
						case ANY:
							return true;
							break;
						case NOT:
							if (!PortCompare(rule.GetDesPort(), packetPortB))
							{
								return true;
							}
							else
							{
								return false;
							}
							break;
						case COMM:
							if (PortCompare(rule.GetDesPort(), packetPortB))
							{
								return true;
							}
							else
							{
								return false;
							}
							break;
						}
					}
					else
					{
						return false;
					}
					break;
				case COMM:
					if (PortCompare(rule.GetSrcPort(), packetPortA))
					{
						switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
						{
						case ANY:
							return true;
							break;
						case NOT:
							if (!PortCompare(rule.GetDesPort(), packetPortB))
							{
								return true;
							}
							else
							{
								return false;
							}
							break;
						case COMM:
							if (PortCompare(rule.GetDesPort(), packetPortB))
							{
								return true;
							}
							else
							{
								return false;
							}
							break;
						}
					}
					else
					{
						return false;
					}
					break;
				}			
			}
			break;
		case NOT: //
			if (rule.GetDesIP() != packetNetmaskB)
			{
				if (rule.GetProtocols() == ICMP)
				{
					return true;
				}
				else
				{
					switch (rule.GetSrcPOpt())//룰의 포트 출발지 ANY NOT COMM
					{
					case ANY:
						switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
						{
						case ANY:
							return true;
							break;
						case NOT:
							if (!PortCompare(rule.GetDesPort(), packetPortB))
							{
								return true;
							}
							else
							{
								return false;
							}
							break;
						case COMM:
							if (PortCompare(rule.GetDesPort(), packetPortB))
							{
								return true;
							}
							else
							{
								return false;
							}
							break;
						}
						break;
					case NOT:
						if (!PortCompare(rule.GetSrcPort(), packetPortA))
						{
							switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
							{
							case ANY:
								return true;
								break;
							case NOT:
								if (!PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							case COMM:
								if (PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							}
						}
						else
						{
							return false;
						}
						break;
					case COMM:
						if (PortCompare(rule.GetSrcPort(), packetPortA))
						{
							switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
							{
							case ANY:
								return true;
								break;
							case NOT:
								if (!PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							case COMM:
								if (PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							}
						}
						else
						{
							return false;
						}
						break;
					}
				}
			}
			break;
		case COMM: //단일 ip
			if (rule.GetDesIP() == packetNetmaskB)
			{
				if (rule.GetProtocols() == ICMP)
				{
					return true;
				}
				else
				{
					switch (rule.GetSrcPOpt())//룰의 포트 출발지 ANY NOT COMM
					{
					case ANY:
						switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
						{
						case ANY:
							return true;
							break;
						case NOT:
							if (!PortCompare(rule.GetDesPort(), packetPortB))
							{
								return true;
							}
							else
							{
								return false;
							}
							break;
						case COMM:
							if (PortCompare(rule.GetDesPort(), packetPortB))
							{
								return true;
							}
							else
							{
								return false;
							}
							break;
						}
						break;
					case NOT:
						if (!PortCompare(rule.GetSrcPort(), packetPortA))
						{
							switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
							{
							case ANY:
								return true;
								break;
							case NOT:
								if (!PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							case COMM:
								if (PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							}
						}
						else
						{
							return false;
						}
						break;
					case COMM:
						if (PortCompare(rule.GetSrcPort(), packetPortA))
						{
							switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
							{
							case ANY:
								return true;
								break;
							case NOT:
								if (!PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							case COMM:
								if (PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							}
						}
						else
						{
							return false;
						}
						break;
					}
				}
			}
			break;
		}
		break;


	case NOT:
		if (rule.GetSrcIP() != packetNetmaskA)
		{
			switch (rule.GetDesIPOpt())//룰의 IP 목적지 ANY NOT COMM
			{
			case ANY: //넘어감
				if (rule.GetProtocols() == ICMP)
				{
					return true;
				}
				else
				{
					switch (rule.GetSrcPOpt())//룰의 포트 출발지 ANY NOT COMM
					{
					case ANY:
						switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
						{
						case ANY:
							return true;
							break;
						case NOT:
							if (!PortCompare(rule.GetDesPort(), packetPortB))
							{
								return true;
							}
							else
							{
								return false;
							}
							break;
						case COMM:
							if (PortCompare(rule.GetDesPort(), packetPortB))
							{
								return true;
							}
							else
							{
								return false;
							}
							break;
						}
						break;
					case NOT:
						if (!PortCompare(rule.GetSrcPort(), packetPortA))
						{
							switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
							{
							case ANY:
								return true;
								break;
							case NOT:
								if (!PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							case COMM:
								if (PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							}
						}
						else
						{
							return false;
						}
						break;
					case COMM:
						if (PortCompare(rule.GetSrcPort(), packetPortA))
						{
							switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
							{
							case ANY:
								return true;
								break;
							case NOT:
								if (!PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							case COMM:
								if (PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							}
						}
						else
						{
							return false;
						}
						break;
					}
				}
				break;
			case NOT: //
				if (rule.GetDesIP() != packetNetmaskB)
				{
					if (rule.GetProtocols() == ICMP)
					{
						return true;
					}
					else
					{
						switch (rule.GetSrcPOpt())//룰의 포트 출발지 ANY NOT COMM
						{
						case ANY:
							switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
							{
							case ANY:
								return true;
								break;
							case NOT:
								if (!PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							case COMM:
								if (PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							}
							break;
						case NOT:
							if (!PortCompare(rule.GetSrcPort(), packetPortA))
							{
								switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
								{
								case ANY:
									return true;
									break;
								case NOT:
									if (!PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								case COMM:
									if (PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								}
							}
							else
							{
								return false;
							}
							break;
						case COMM:
							if (PortCompare(rule.GetSrcPort(), packetPortA))
							{
								switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
								{
								case ANY:
									return true;
									break;
								case NOT:
									if (!PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								case COMM:
									if (PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								}
							}
							else
							{
								return false;
							}
							break;
						}
					}
				}
				break;
			case COMM: //단일 ip
				if (rule.GetDesIP() == packetNetmaskB)
				{
					if (rule.GetProtocols() == ICMP)
					{
						return true;
					}
					else
					{
						switch (rule.GetSrcPOpt())//룰의 포트 출발지 ANY NOT COMM
						{
						case ANY:
							switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
							{
							case ANY:
								return true;
								break;
							case NOT:
								if (!PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							case COMM:
								if (PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							}
							break;
						case NOT:
							if (!PortCompare(rule.GetSrcPort(), packetPortA))
							{
								switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
								{
								case ANY:
									return true;
									break;
								case NOT:
									if (!PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								case COMM:
									if (PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								}
							}
							else
							{
								return false;
							}
							break;
						case COMM:
							if (PortCompare(rule.GetSrcPort(), packetPortA))
							{
								switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
								{
								case ANY:
									return true;
									break;
								case NOT:
									if (!PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								case COMM:
									if (PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								}
							}
							else
							{
								return false;
							}
							break;
						}
					}
				}
				break;
			}
		}
		else return false;
		break;


	case COMM:
		if (rule.GetSrcIP() == packetNetmaskA)
		{
			switch (rule.GetDesIPOpt())//룰의 IP 목적지 ANY NOT COMM
			{
			case ANY: //넘어감
				if (rule.GetProtocols() == ICMP)
				{
					return true;
				}
				else
				{
					switch (rule.GetSrcPOpt())//룰의 포트 출발지 ANY NOT COMM
					{
					case ANY:
						switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
						{
						case ANY:
							return true;
							break;
						case NOT:
							if (!PortCompare(rule.GetDesPort(), packetPortB))
							{
								return true;
							}
							else
							{
								return false;
							}
							break;
						case COMM:
							if (PortCompare(rule.GetDesPort(), packetPortB))
							{
								return true;
							}
							else
							{
								return false;
							}
							break;
						}
						break;
					case NOT:
						if (!PortCompare(rule.GetSrcPort(), packetPortA))
						{
							switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
							{
							case ANY:
								return true;
								break;
							case NOT:
								if (!PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							case COMM:
								if (PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							}
						}
						else
						{
							return false;
						}
						break;
					case COMM:
						if (PortCompare(rule.GetSrcPort(), packetPortA))
						{
							switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
							{
							case ANY:
								return true;
								break;
							case NOT:
								if (!PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							case COMM:
								if (PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							}
						}
						else
						{
							return false;
						}
						break;
					}
				}
				break;
			case NOT: //
				if (rule.GetDesIP() != packetNetmaskB)
				{
					if (rule.GetProtocols() == ICMP)
					{
						return true;
					}
					else
					{
						switch (rule.GetSrcPOpt())//룰의 포트 출발지 ANY NOT COMM
						{
						case ANY:
							switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
							{
							case ANY:
								return true;
								break;
							case NOT:
								if (!PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							case COMM:
								if (PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							}
							break;
						case NOT:
							if (!PortCompare(rule.GetSrcPort(), packetPortA))
							{
								switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
								{
								case ANY:
									return true;
									break;
								case NOT:
									if (!PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								case COMM:
									if (PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								}
							}
							else
							{
								return false;
							}
							break;
						case COMM:
							if (PortCompare(rule.GetSrcPort(), packetPortA))
							{
								switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
								{
								case ANY:
									return true;
									break;
								case NOT:
									if (!PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								case COMM:
									if (PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								}
							}
							else
							{
								return false;
							}
							break;
						}
					}
				}
				break;
			case COMM: //단일 ip
				if (rule.GetDesIP() == packetNetmaskB)
				{
					if (rule.GetProtocols() == ICMP)
					{
						return true;
					}
					else
					{
						switch (rule.GetSrcPOpt())//룰의 포트 출발지 ANY NOT COMM
						{
						case ANY:
							switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
							{
							case ANY:
								return true;
								break;
							case NOT:
								if (!PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							case COMM:
								if (PortCompare(rule.GetDesPort(), packetPortB))
								{
									return true;
								}
								else
								{
									return false;
								}
								break;
							}
							break;
						case NOT:
							if (!PortCompare(rule.GetSrcPort(), packetPortA))
							{
								switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
								{
								case ANY:
									return true;
									break;
								case NOT:
									if (!PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								case COMM:
									if (PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								}
							}
							else
							{
								return false;
							}
							break;
						case COMM:
							if (PortCompare(rule.GetSrcPort(), packetPortA))
							{
								switch (rule.GetDesPOpt())//룰의 포트 목적지 ANY NOT COMM
								{
								case ANY:
									return true;
									break;
								case NOT:
									if (!PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								case COMM:
									if (PortCompare(rule.GetDesPort(), packetPortB))
									{
										return true;
									}
									else
									{
										return false;
									}
									break;
								}
							}
							else
							{
								return false;
							}
							break;
						}
					}
				}
				break;
			}
		}
		else return false;
		break;
	}
}

bool CCompareHeader::PortCompare(std::vector<u_int16_t> rulePort, u_int16_t packetPort)
{
	if (rulePort.size() == 1)
	{
		if (rulePort[0] == packetPort) return true;//발견하면 true 리턴
	}
	else
	{
        int i = rulePort[0], j = rulePort[1];
		for (; i < j; i++)
		{
			if (i == packetPort) return true;//발견하면 true 리턴
		}
	}
	return false;//못찾으면 false 리턴
}
