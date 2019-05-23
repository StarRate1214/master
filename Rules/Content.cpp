#include <RuleEngine.h>

#define HTTP_CLIENT_BODY    0x40
#define HTTP_COOKIE         0x20
#define HTTP_HEADER         0x10
#define HTTP_METHOD         0x08
#define HTTP_URI            0x04
#define HTTP_STAT_CODE      0x02
#define HTTP_STAT_MSG       0x01

int CRuleEngine::content(std::string cnt, bool nocase, int depth, int offset, int distance, int within, u_int8_t http_option, int prev = 0)
{
	int i = 0;

	const char			LF			= '\n';
	const std::string	CRLF		= "\r\n";
	const std::string	END_HTTP	= "\r\n\r\n";
	const char			QUOT		= '\"';
	const char			PIPE		= '|';
	const char			SPACE		= ' ';

	std::string content;	// CONTENT
	std::string http_match;
	std::string payload = (char *)packet.data_payload;
	std::string temp;

	int pos;				// string at() pos
	int pos_start;			// string find() START pos
	int pos_end;			// string find() END pos
	int pos_hexcnt = 0;		// hexcontent pos

	bool if_rev = false;	// IF content have '!' -> TRUE
	bool if_pipe = false;	// IF pipe ON
	
	 // IF content have '|'
	delete[] hexcontent;
	hexcontent = new char[cnt.size()]{ 0, };
	
	

	
	//##########// content " cutting //##########//

	// IF content have '!' (RAW)
	if (cnt.at(0) == '!')
		if_rev = true;

	// " cutting
	pos_start = cnt.find(QUOT);
	pos_start++;
	pos_end = cnt.find(QUOT, pos_start);
	pos_end--;

	// IF content have '|'
	if (cnt.find(PIPE) != std::string::npos)
	{
		//if_raw = true;
		pos = pos_start;
		while (cnt.at(pos) != QUOT)
		{
			if (cnt.at(pos) == PIPE)
			{
				if (if_pipe == false)
					if_pipe = true;
				else
					if_pipe = false;

				pos++;
			}

			else if (cnt.at(pos) == SPACE)
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

	//////////////////////////////////////////////////////////////
	// IF nocase TRUE
	if (nocase == true)
	{
		for (i = 0; i < payload.size(); i++)
		{
			if (payload.at(i) >= 97 && payload.at(i) <= 122)
			{
				temp = payload.at(i) - 32;
				payload.replace(i, 1, temp);
			}
		}
		for (i = 0; i < content.size(); i++)
		{
			if (content.at(i) >= 97 && content.at(i) <= 122)
			{
				temp = content.at(i) - 32;
				content.replace(i, 1, temp);
			}
		}
	}

	////////////////////////////////////////////////////////////
	// match
	pos_start	= 0;
	pos_end		= 0;
	pos			= 0;

	if (pos_start > packet.data_payload_size)
		return -3;

	if (http_option == 0)
	{
		if (offset != 0)
			pos_start = offset;
		if (depth != 0)
			pos_end = depth;
		if (distance != 0)
			pos_start = prev + distance + 1;	
		if (within != 0)
			pos_end = prev + within + 1;	


		if (pos_start > pos_end)	// 비정상적
			return -2;

		if (pos_end != 0)
			payload = payload.substr(pos_start, pos_end - pos_start + 1);
		else
			payload = payload.substr(pos_start);

		pos = payload.find(content);

		if (pos != std::string::npos)
			return pos + pos_start - 1 + content.size();
		else // NOT FOUND
			return -1;
	}

	// IF have HTTP OPTION
	switch (http_option)
	{
	case HTTP_CLIENT_BODY: // Request
		if (!(http_match == "GET" ||
			http_match == "POST" ||
			http_match == "HEAD" ||
			http_match == "PUT" ||
			http_match == "DELETE" ||
			http_match == "TRACE" ||
			http_match == "OPTIONS" ||
			http_match == "CONNECT" ||
			http_match == "PATCH"))
		{	// NOT Request
			return -1;
		}

		pos = payload.find(END_HTTP);
		if ((pos != -1) && (pos + 4 != payload.size()))
		{
			pos_start = pos + 1;
			http_match = payload.substr(pos_start);

			if (http_match.find(content) != std::string::npos)
				return pos_start + http_match.find(content) + content.size() - 1;
			else
				return -1;
		}
		else // NOT Found
			return -1;
		break;
	case HTTP_COOKIE: // Request & Response
		if (nocase == true)
			pos_start = payload.find("COOKIE: ");
		else
			pos_start = payload.find("Cookie: ");

		if (pos_start != std::string::npos)
		{
			http_match = payload.substr(pos_start, payload.find(CRLF, pos_start+1) + 1 - pos_start + 1);
			pos = http_match.find(content);
			if (pos != std::string::npos)
				return pos_start + pos + content.size() - 1;
			else //Cookie: NOT FOUND
				return -1;
		}
		else//NOT FOUND
			return -1;
		break;
	case HTTP_HEADER: // Request & Response
		pos_start = payload.find(CRLF);
		if (payload.find("HTTP/") != 0)
			pos_start+=2;
		pos_end = payload.find(END_HTTP);
		pos_end += 3;

		http_match = payload.substr(pos_start, pos_end - pos_start + 1);
		pos = http_match.find(content);

		if (pos != std::string::npos)
		{
			return pos_start + pos + content.size() - 1;
		}
		else // not match
			return -1;

		break;
	case HTTP_METHOD: // Request

		http_match = payload.substr(0, payload.find(SPACE, 0));
		if (http_match == "GET"		||
			http_match == "POST"	||
			http_match == "HEAD"	||
			http_match == "PUT"		||
			http_match == "DELETE"	||
			http_match == "TRACE"	||
			http_match == "OPTIONS" ||
			http_match == "CONNECT" ||
			http_match == "PATCH")
		{

			if (http_match.find(content) != std::string::npos)
				return http_match.find(content) + content.size() - 1;
			else
			{
					return -1;
			}
		}
		else
			return -2;
		break;
	case HTTP_URI:
		break;
	case HTTP_STAT_CODE:
		pos = payload.find("HTTP/");
		if (pos == 0)
		{
			pos_start = payload.find(SPACE);
			pos_start++;
			pos_end = payload.find(SPACE, pos_start);
			pos_end--;
			http_match = payload.substr(pos_start, pos_end - pos_start + 1);
			pos = http_match.find(content);
			if (pos != std::string::npos)
			{
				return pos_start + pos + content.size() - 1;
			}
			else
			{
				return -1;
			}
		}
		else // NOT Response HTTP
			return -1;
		break;
	case HTTP_STAT_MSG:
		pos = payload.find("HTTP/");
		if (pos == 0)
		{
			pos_start = payload.find(SPACE);
			pos_start++;
			pos_start = payload.find(SPACE, pos_start);
			pos_end = payload.find(CRLF);
			pos_end--;

			http_match = payload.substr(pos_start, pos_end - pos_start + 1);
			pos = http_match.find(content);

			if (pos != std::string::npos)
			{
				return pos_start + pos + content.size() - 1;
			}
			else // NOT MATCH
				return -1;
		}
		else // NOT Response HTTP
			return -1;
		break;
	default:
		return -2;
		break;
	}
	return -1234;
}