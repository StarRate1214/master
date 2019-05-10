#include "RuleEngine.h"

bool CRuleEngine::CompareOption(std::string options)
{
    int semicolon=0;
    int cont=0; //content
    while((cont=(int)options.find("content:"))!=-1) //contents
    {
        std::string contents={0,};
        bool nocase = false;
        std::string depth ={0,};
        std::string offset={0,};
        std::string distance ={0,};
        std::string within ={0,};
        u_int8_t http_option;

        std::string method={0,};
        std::string uri={0,};

        std::string httptemp={0,};

        std::string header={0,};
        std::string cookie={0,};
        std::string body={0,};
        std::string stat_code={0,};
        std::string stat_msg={0,};

        semicolon=options.find(';',cont);
        contents=options.substr(cont+8,semicolon-9);//contents:option parsing
        options.erase(cont,semicolon+1); //erase parsed
        if((cont=(int)options.find("nocase;"))!=-1)
        {
            nocase=true;
        }
        if((cont=(int)options.find("depth:"))!=-1)
        {
            semicolon=options.find(';',cont);
        contents=options.substr(cont+6,semicolon-7);//contents:option parsing
        options.erase(cont,semicolon+1); 
        }

    }
}