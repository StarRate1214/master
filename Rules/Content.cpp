#include "RuleEngine.h"
#include "Charstr.h"

int CRuleEngine::content(std::string cnt, bool nocase, int depth, int offset, int distance, int within, u_int8_t http_option, int prev = 0)
{
	CCharstr cstr;

	int i = 0;

	// const char LF		= '\n';
	const char CRLF[] = "\r\n";
	const char END_HTTP[] = "\r\n\r\n";
	const char QUOT = '\"';
	const char PIPE = '|';
	const char SPACE = ' ';

	std::string s_content;
	std::string temp;

	int pos;				// string at() pos
	int pos_start;			// string find() START pos
	int pos_end;			// string find() END pos
	int pos_hexcnt = 0;		// hexcontent pos

	bool if_rev = false;	// IF content have '!' -> TRUE
	bool if_pipe = false;	// IF pipe ON

	int csize = 0;	// content size

	// content with PIPE LINE "|"
	delete[] hexcontent;
	hexcontent = new char[cnt.size()];
	std::memset(hexcontent, 0, sizeof(char) * cnt.size());
	
	// char array content
	delete[] c_content;
	c_content = new char[cnt.size()];
	strcpy(c_content, cnt.c_str());

	// char array payload
	int psize = packet.data_payload_size;
	delete[] c_payload;
	c_payload = new char[psize];
	for (int i = 0; i < psize; i++)
		c_payload[i] = packet.data_payload[i];

	// char array http match
	int hsize = psize;
	delete[] c_httpmatch;
	c_httpmatch = new char[hsize];
		


	//////////////////////////////////
	// CONTENT DOUBLE QUOTE CUTTING //
	//////////////////////////////////

	// IF content have '!' (RAW)
	if (cnt.at(0) == '!')
		if_rev = true;

	// double quote cutting - "content" => content
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
					hexcontent[pos_hexcnt] = std::stoi(cnt.substr(pos, 2), 0, 16);
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
		csize = pos_hexcnt;

		// strcpy
		for (int i = 0; i < csize; i++)
			c_content[i] = hexcontent[i];
	}
	else
	{
		s_content = cnt.substr(pos_start, pos_end - pos_start + 1);
		strcpy(c_content, s_content.c_str());
		csize = s_content.size();
	}

	////////////////////
	// IF nocase TRUE //
	////////////////////
	if (nocase == true)
	{
		for (i = 0; i < psize; i++)
			if(cstr.at(c_payload, i) >= 97 && cstr.at(c_payload, i) <= 122)
				c_payload[i] = cstr.at(c_payload, i) - 32;

		for(int i=0; i<csize; i++)
			if(cstr.at(c_content, i) >= 97 && cstr.at(c_content, i) <= 122)
				c_content[i] = cstr.at(c_content, i) - 32;
	}

	//////////////
	// IS MATCH //
	//////////////

	pos_start = 0;
	pos_end = 0;
	pos = 0;

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

		if (pos_start >= psize)
			return -100;

		if (pos_start > pos_end)	// 비정상적
			return -101;
		
		if (pos_end != 0)
			cstr.substr(c_payload, &psize, pos_start, pos_end - pos_start + 1);
		else
			cstr.substr(c_payload, &psize, pos_start, 0);
	
		pos = cstr.search(c_payload, psize, c_content, csize, 0);

		if (pos != -1)
		{
			// TRUE
			if (if_rev == false)
				return pos + pos_start - 1 + csize;
			// ! TRUE
			else
				return -1;
		}
		else // NOT FOUND
		{
			// ! FALSE
			if (if_rev == true)
			{
				if (pos_end == 0)
					return psize - 1;
				else
					return pos_end;
			}
			else
				return -1;
		}
	}
	
	switch (http_option)
	{
	case HTTP_CLIENT_BODY: // Request
		cstr.substrto(c_httpmatch, &hsize, c_payload, psize, 0, cstr.search(c_payload, psize, SPACE, 0));

		if(!(cstr.isSame(c_httpmatch, hsize, "GET", 3)) ||
			(cstr.isSame(c_httpmatch, hsize, "POST", 4)) ||
			(cstr.isSame(c_httpmatch, hsize, "HEAD", 4)) ||
			(cstr.isSame(c_httpmatch, hsize, "PUT", 3)) ||
			(cstr.isSame(c_httpmatch, hsize, "DELETE", 6)) ||
			(cstr.isSame(c_httpmatch, hsize, "TRACE", 5)) ||
			(cstr.isSame(c_httpmatch, hsize, "OPTIONS", 7)) ||
			(cstr.isSame(c_httpmatch, hsize, "CONNECT", 7)) ||
			(cstr.isSame(c_httpmatch, hsize, "PATCH", 5)))
		{	// NOT Request
			return -103;
		}

		pos = cstr.search(c_payload, psize, END_HTTP, 4, 0);

		if((pos != -1) && (pos + 4 != psize))
		{
			pos_start = pos + 1;
			cstr.substrto(c_httpmatch, &hsize, c_payload, psize, pos_start, 0);
			
			if(cstr.search(c_httpmatch, hsize, c_content, csize, 0) != -1)
			{
				// TRUE
				if (if_rev == false)
					return pos_start + cstr.search(c_httpmatch, hsize, c_content, csize, 0) + csize - 1;
				// ! TRUE
				else
					return -1;
			}
			else
			{
				// ! FALSE
				if (if_rev == true)
					return pos_start + hsize - 1;
				// FALSE
				else
					return -1;
			}
		}
		else // http_option not matched
			return -104;
		break;
	case HTTP_COOKIE: // Request & Response
		if (nocase == true)
			pos_start = cstr.search(c_payload, psize, "COOKIE: ", 8, 0);
		else
			pos_start = cstr.search(c_payload, psize, "Cookie: ", 8, 0);

		if (pos_start != std::string::npos)
		{
			pos_start += 8;
			pos_end = cstr.search(c_payload, psize, CRLF, 2, pos_start);
			pos_end++;

			cstr.substrto(c_httpmatch, &hsize, c_payload, psize, pos_start, (pos_end - pos_start + 1));

			pos = cstr.search(c_httpmatch, hsize, c_content, csize, 0);
			
			if (pos != std::string::npos)
			{
				// TRUE
				if (if_rev == false)
					return pos_start + pos + csize - 1;
				// ! TRUE
				else
					return -1;
			}
			else
			{
				// ! FALSE
				if (if_rev == true)
					return pos_end;
				// FALSE
				else
					return -1;
			}
		}
		else
			return -105;
		break;
	case HTTP_HEADER: // Request & Response
		pos_start = cstr.search(c_payload, psize, CRLF, 2, 0);
		pos = cstr.search(c_payload, psize, "HTTP/", 5, 0);

		if (pos != 0)
			pos_start += 2;
		else if (pos == std::string::npos)
			return -106;

		pos_end = cstr.search(c_payload, psize, END_HTTP, 4, 0);
		pos_end += 3;

		cstr.substrto(c_httpmatch, &hsize, c_payload, psize, pos_start, (pos_end - pos_start + 1));

		pos = cstr.search(c_httpmatch, hsize, c_content, csize, 0);

		if (pos != std::string::npos)
		{
			// TRUE
			if (if_rev == false)
				return pos_start + pos + csize - 1;
			// ! TRUE
			else
				return -1;
		}
		else // not match
		{
			// ! FALSE
			if (if_rev == true)
				return pos_end;
			// FALSE
			else
				return -1;
		}

		break;
	case HTTP_METHOD: // Request
		pos_start = 0;
		pos_end = cstr.search(c_payload, psize, SPACE, 0);

		cstr.substrto(c_httpmatch, &hsize, c_payload, psize, pos_start, pos_end);

		if ((cstr.isSame(c_httpmatch, hsize, "GET", 3)) ||
			(cstr.isSame(c_httpmatch, hsize, "POST", 4)) ||
			(cstr.isSame(c_httpmatch, hsize, "HEAD", 4)) ||
			(cstr.isSame(c_httpmatch, hsize, "PUT", 3)) ||
			(cstr.isSame(c_httpmatch, hsize, "DELETE", 6)) ||
			(cstr.isSame(c_httpmatch, hsize, "TRACE", 5)) ||
			(cstr.isSame(c_httpmatch, hsize, "OPTIONS", 7)) ||
			(cstr.isSame(c_httpmatch, hsize, "CONNECT", 7)) ||
			(cstr.isSame(c_httpmatch, hsize, "PATCH", 5)))
		{
			pos_end--;

			pos = cstr.search(c_httpmatch, hsize, c_content, csize, 0);

			if (pos != std::string::npos)
			{
				// TRUE
				if (if_rev == false)
					return pos_start + pos + csize - 1;
				// ! TRUE
				else
					return -1;
			}
			else
			{
				// ! FALSE
				if (if_rev == true)
					return pos_end;
				// FALSE
				else
					return -1;
			}
		}
		else
			return -107;
		break;
	case HTTP_URI:
		cstr.substrto(c_httpmatch, &hsize, c_payload, psize, 0, cstr.search(c_payload, psize, SPACE, 0));

		if ((cstr.isSame(c_httpmatch, hsize, "GET", 3)) ||
			(cstr.isSame(c_httpmatch, hsize, "POST", 4)) ||
			(cstr.isSame(c_httpmatch, hsize, "HEAD", 4)) ||
			(cstr.isSame(c_httpmatch, hsize, "PUT", 3)) ||
			(cstr.isSame(c_httpmatch, hsize, "DELETE", 6)) ||
			(cstr.isSame(c_httpmatch, hsize, "TRACE", 5)) ||
			(cstr.isSame(c_httpmatch, hsize, "OPTIONS", 7)) ||
			(cstr.isSame(c_httpmatch, hsize, "CONNECT", 7)) ||
			(cstr.isSame(c_httpmatch, hsize, "PATCH", 5)))
		{
			pos_start = cstr.search(c_payload, psize, SPACE, 0);
			pos_start++;
			pos_end = cstr.search(c_payload, psize, SPACE, pos_start);
			pos_end--;

			cstr.substrto(c_httpmatch, &hsize, c_payload, psize, pos_start, (pos_end - pos_start + 1));

			pos = cstr.search(c_httpmatch, hsize, c_content, csize, 0);

			if (pos != std::string::npos)
			{
				if (if_rev == false)
					return pos_start + pos + csize - 1;
				else
					return -1;
			}
			else // NOT MATCH
			{
				if (if_rev == true)
					return pos_end;
				else
					return -1;
			}
		}
		else
		{
			return -108;
		}
		//////////
		break;
	case HTTP_STAT_CODE:
		pos = cstr.search(c_payload, psize, "HTTP/", 5, 0);

		if (pos == 0)
		{
			pos_start = cstr.search(c_payload, psize, SPACE, 0);
			pos_start++;
			pos_end = cstr.search(c_payload, psize, SPACE, pos_start);
			pos_end--;
			cstr.substrto(c_httpmatch, &hsize, c_payload, psize, pos_start, (pos_end - pos_start + 1));
			pos = cstr.search(c_httpmatch, hsize, c_content, csize, 0);
			if (pos != std::string::npos)
			{
				if (if_rev == false)
					return pos_start + pos + csize - 1;
				else
					return -1;
			}
			else
			{
				if (if_rev == true)
					return pos_end;
				else
					return -1;
			}
		}
		else // NOT Response HTTP
			return -109;
		break;
	case HTTP_STAT_MSG:
		pos = cstr.search(c_payload, psize, "HTTP/", 5, 0);
		if (pos == 0)
		{
			pos_start = cstr.search(c_payload, psize, SPACE, 0);
			pos_start++;
			pos_start = cstr.search(c_payload, psize, SPACE, pos_start);
			pos_end = cstr.search(c_payload, psize, CRLF, 2, 0);
			pos_end--;

			cstr.substrto(c_httpmatch, &hsize, c_payload, psize, pos_start, (pos_end - pos_start + 1));

			pos = cstr.search(c_httpmatch, hsize, c_content, csize, 0);

			if (pos != std::string::npos)
			{
				if (if_rev == false)
					return pos_start + pos + csize - 1;
				else
					return -1;
			}
			else // NOT MATCH
			{
				if (if_rev == true)
					return pos_end;
				else
					return -1;
			}

		}
		else // NOT Response HTTP
			return -110;
		break;
	default:
		return -111;
		break;
	}

	return -200;
}
