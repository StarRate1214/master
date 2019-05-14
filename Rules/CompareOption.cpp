#include "RuleEngine.h"

#define HTTP_CLIENT_BODY    0x40
#define HTTP_COOKIE         0x20
#define HTTP_HEADER         0x10
#define HTTP_METHOD         0x08
#define HTTP_URI            0x04
#define HTTP_STAT_CODE      0x02
#define HTTP_STAT_MSG       0x01

bool CRuleEngine::CompareOption(std::vector<SRule_option> options)
{
    int semicolon=0;
    int colon=0;
    int cont=0; //content
    while((cont=(int)contents.find("content:"))!=-1) //contents
    {
        std::string contents={0,};
        bool nocase = false;
        int depth =0;
        int offset=0;
        int distance=0;
        int within =0;
        u_int8_t http_option;

        std::string method={0,};
        std::string uri={0,};

        std::string httptemp={0,};

        std::string header={0,};
        std::string cookie={0,};
        std::string body={0,};
        std::string stat_code={0,};
        std::string stat_msg={0,};

        colon=contents.find(':',cont);
        semicolon=contents.find(';',colon); 
        contents=contents.substr(colon+1,semicolon-1);//contents:option parsing
        contents.erase(cont,semicolon+1); //erase parsed
        if((cont=(int)contents.find("nocase;"))!=-1) //nocase;
        {
            nocase=true;
        }
        if((cont=(int)contents.find("depth:"))!=-1) //depth:<num>
        {
            semicolon=contents.find(';',cont);
            contents=contents.substr(colon+1,semicolon-1);//contents:option parsing
            contents.erase(cont,semicolon+1); 
        }

    }
}