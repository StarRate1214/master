#include "RuleEngine.h"

#define HTTP_CLIENT_BODY    0x40
#define HTTP_COOKIE         0x20
#define HTTP_HEADER         0x10
#define HTTP_METHOD         0x08
#define HTTP_URI            0x04
#define HTTP_STAT_CODE      0x02
#define HTTP_STAT_MSG       0x01

int CRuleEngine::content(
    const std::string cnt, 
    //int semicolon, 
    const bool nocase, 
    const int depth, 
    const int offset, 
    const int distance, 
    const int within, 
    const u_int8_t http_option
    const int prev)
{
    const char CRLF = '\n';
    
    std::string content;
    std::string http_mathc;

    int pos         = 0;
    int pos_start   = 0;
    int pos_end     = 0;
    int pos_hexcnt  = 0;

    bool if_ref     = false;
    bool if_pipe    = false;

    std::string payload = packet.data_payload;

    delete[] hexcontent;
    hexcontent = new char[cnt.size()] {0, };

    /*__________ CONTENT PARSING __________*/
    
    // IF content have <!>
    if(cnt.at(0) == '!')
        if_rev = true;
    
    // " cutting
    pos_start = cnt.find('\"');
    pos_start++;
    pos_end = cnt.find('\"', pos_start);
	pos_end--;

	// IF content have <|>
	if (cnt.find('|') != std::string::npos)
	{
		pos = pos_start;
		while (cnt.at(pos) != '\"')
		{
			if (cnt.at(pos) == '|')
			{
				if (if_pipe == false)
					if_pipe = true;
				else
					if_pipe = false;

				pos++;
			}

			else if (cnt.at(pos) == ' ')
			{
				if (if_pipe == false)
				{
					hexcontent[pos_hexcnt] = cnt.at(pos);
					pos_hexcnt++;
				}
				pos++;
			}
			else
			{
				if (if_pipe == true)
				{
					hexcontent[pos_hexcnt] = stoi(cnt.substr(pos, 2), 0, 16);
					pos_hexcnt++;
					pos++;
					pos++;
				}
				else
				{
					hexcontent[pos_hexcnt] = cnt.at(pos);
					pos_hexcnt++;
					pos++;
				}
			}
		}
		content = hexcontent;

	}
	else
		content = cnt.substr(pos_start, pos_end-pos_start+1);

	std::cout << "FINAL CONTENT : " << content << std::endl;


	/*__________ HTTP MATCH __________*/


	if (http_option == 0)
	{
		pos_start = 0;
		pos_end = 0;
		pos = 0;

		if (offset != 0)
			pos_start = offset;
		if (depth != 0)
			pos_end = depth;
		if (distance != 0)
			pos_start = prev + distance;	// +1을 해줘야하는가? 안해줘도 되는가?
		if (within != 0)
			pos_end = prev + within;

		if (pos_start > pos_end)	// 비정상적
			return -1;

		if (pos_end != 0)
			payload = payload.substr(pos_start, pos_end - pos_start + 1);
		else
			payload = payload.substr(pos_start);
		std::cout << "http option : " << http_option << std::endl;
		std::cout << "##########PAYLOAD PRINT##########\n" << payload << std::endl;

		pos = payload.find(content);

		std::cout << "POS : " << pos << std::endl;
		if (pos != std::string::npos)
			return pos + pos_start - 1 + content.size();
		else
			return -1;
	}

	// IF have HTTP OPTION
	pos = 0;
	pos_start = 0;
	pos_end = 0;

	switch (http_option)
	{
	case HTTP_CLIENT_BODY:
		std::cout << "/ / / / / HTTP CLIENT BODY\n";
		pos = payload.find("\r\n\r\n");
		// std::cout << "POS : " << pos << std::endl;
		if ((pos != -1) && (pos + 4 != payload.size()))
		{
			pos_start = pos + 1;
			http_match = payload.substr(pos_start);
			std::cout << "C BODY : " << http_match << std::endl;

			
		}
		else // end of string
			return -1;

		break;
	case HTTP_COOKIE:
		break;
	case HTTP_HEADER:
		break;
	case HTTP_METHOD:
		std::cout << "/ / / / / HTTP METHOD\n";

		http_match = payload.substr(0, payload.find(' ', 0));
		if (http_match == "GET" || http_match == "POST")
		{
			std::cout << "HTTP_METHOD : " << http_match << std::endl;
			if (http_match.find(content) != std::string::npos)
				return http_match.find('T');
		}
		else
			return -1;
		break;
	case HTTP_URI:
		std::cout << "/ / / / / HTTP URI\n";

		http_match = payload.substr(0, payload.find(' ', 0));
		if (http_match == "GET" || http_match == "POST")
		{
			pos_start = payload.find(' ');
			pos_start++;
			pos_end = payload.find(' ', pos_start);
			pos_end--;

			http_match = payload.substr(pos_start, pos_end - pos_start + 1);
			pos = http_match.find(content);

			std::cout << "URI : " << http_match << std::endl;

			if (pos != -1)
				return pos + pos_start - 1 + content.size();
			else
				return -1;
		}
		else
			return -1;
		break;
	case HTTP_STAT_CODE:
		break;
	case HTTP_STAT_MSG:
		break;
	default:
		return -1;
		break;
	}










	return true;
}