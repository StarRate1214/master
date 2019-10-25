#include "RuleEngine.h"

bool CRuleEngine::CompareOption(std::vector<SRule_option> options, CNation *country)
{
    std::vector<SRule_option>::iterator i;
    std::string content = {
        0,
    };
    int semicolon;
    bool nocase = false;
    int depth = 0;
    int offset = 0;
    int distance = 0;
    int within = 0;
    int pos = 0;
    int contents = 0;
    u_int8_t http_option;
    int count = 0;
    

    for (i = options.begin(); i != options.end(); i++)
    {
        switch (i->rule)
        {

        case CONTENTS:
            pos = 0;
            if ((pos = (int)i->option.find("content:")) != -1) //contents:option parsing
            {
                semicolon = i->option.find(';', pos);
                content = i->option.substr(pos + 8, semicolon - 8 - pos);
            }
            if ((pos = (int)i->option.find("nocase;")) != -1) //nocase;
            {
                nocase = true;
            }
            if ((pos = (int)i->option.find("depth:")) != -1) //depth:<num>
            {
                semicolon = i->option.find(';', pos);
                depth = stoi(i->option.substr(pos + 6, semicolon - 6 - pos));
            }
            if ((pos = (int)i->option.find("offset:")) != -1) //offset:<num>
            {
                semicolon = i->option.find(';', pos);
                offset = stoi(i->option.substr(pos + 7, semicolon - 7 - pos));
            }
            if ((pos = (int)i->option.find("distance:")) != -1) //distance:<num>
            {
                semicolon = i->option.find(';', pos);
                distance = stoi(i->option.substr(pos + 9, semicolon - 9 - pos));
            }
            if ((pos = (int)i->option.find("within:")) != -1) //within:<num>
            {
                semicolon = i->option.find(';', pos);
                within = stoi(i->option.substr(pos + 7, semicolon - 7 - pos));
            }
            if ((pos = (int)i->option.find("http_client_body;")) != -1)
                http_option = HTTP_CLIENT_BODY;
            else if ((pos = (int)i->option.find("http_cookie;")) != -1)
                http_option = HTTP_COOKIE;
            else if ((pos = (int)i->option.find("http_header;")) != -1)
                http_option = HTTP_HEADER;
            else if ((pos = (int)i->option.find("http_method;")) != -1)
                http_option = HTTP_METHOD;
            else if ((pos = (int)i->option.find("http_uri;")) != -1)
                http_option = HTTP_URI;
            else if ((pos = (int)i->option.find("http_stat_code;")) != -1)
                http_option = HTTP_STAT_CODE;
            else if ((pos = (int)i->option.find("http_stat_msg;")) != -1)
                http_option = HTTP_STAT_MSG;
            else
                http_option = 0;
            if (count == 0)
                contents = this->content(content, nocase, depth, offset, distance, within, http_option, 0);
            else
                contents = this->content(content, nocase, depth, offset, distance, within, http_option, contents);
            if (contents < 0)
                return false;
            else
                count++;
            break;

        case PCRE:
            if (!pcre(i->option))
                return false;
            break;
        case NPTTL:
            if (!TTL(i->option, packet.ip.getTTL()))
                return false;
            break;
        case NPTOS:
            if (!Tos(i->option, packet.ip.getTos()))
                return false;
            break;
        case NPFBITS:
            if (!Fragbits(i->option, packet.ip.getMoreFrag(), packet.ip.getDontFrag()))
                return false;
            break;
        case NPDSIZE:
            if (!dsize(i->option, packet.data_payload_size))
                return false;
            break;
        case NPFLAGS:
            if (!flags(i->option, packet.tcp.getFin(), packet.tcp.getSyn(), packet.tcp.getRst(), packet.tcp.getPsh(), packet.tcp.getAck(), packet.tcp.getUrg()))
                return false;
            break;
        case NPSEQ:
            if (!seq(i->option, packet.tcp.getSeqNum()))
                return false;
            break;
        case NPACK:
            if (!ack(i->option, packet.tcp.getAckNum()))
                return false;
            break;
        case NPWINDOW:
            if (!window(i->option, packet.tcp.getWinSize()))
                return false;
            break;
        case NPITYPE:
            if (!itype(i->option, packet.icmp.getICMPtype()))
                return false;
            break;
        case NPICODE:
            if (!icode(i->option, packet.icmp.getICMPcode()))
                return false;
            break;
        case NPSAMEIP:
            if (packet.ip.getSrcIP() != packet.ip.getDstIP())
                return false;
            break;
        case NATION:
            if (!country->Nation(packet, i->option))
            {
                return false;
            }
            break;
        case ML:
            if(!ml(i->option))
                return false;
            break;
        }
    }
    return true;
}
