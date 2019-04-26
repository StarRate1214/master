#include "../HEADER/Packet.h"
#include <string>
#include <cstdlib>

bool TTL(std::string ttl_opt,u_int8_t ttl){
    int ttl_opt_length = ttl_opt.length();
    int ran1,ran2; //범위
    if(ttl_opt[0]=='<')
    {
        if(ttl_opt[1]=='=') // <=
        {
            std::string s= ttl_opt.substr(2,ttl_opt_length-2);
            ran1= atoi(s.c_str());
            return (ttl<=ran1); 
        }
        else // <
        {
            std::string s= ttl_opt.substr(1,ttl_opt_length-1);
            ran1= atoi(s.c_str());
            return (ttl<ran1); 
        }
    }
    else if(ttl_opt[0]=='>')
    {
        if(ttl_opt[1]=='=') // >=
        {
            std::string s= ttl_opt.substr(2,ttl_opt_length-2);
            ran1= atoi(s.c_str());
            return (ttl>=ran1); 
        }
        else // >
        {
            std::string s= ttl_opt.substr(1,ttl_opt_length-1);
            ran1= atoi(s.c_str());
            return (ttl>ran1); 
        }
    }
    else if(ttl_opt[0]=='=') // =
    {
        std::string s= ttl_opt.substr(1,ttl_opt_length-1);
        ran1= atoi(s.c_str());
        return (ttl==ran1); 
    }
    else //'-' 범위 지정 연산자
    {
        int range = ttl_opt.find('-');
        if(range==0) //0~ran1
        {
            std::string s= ttl_opt.substr(1,ttl_opt_length-1);
            ran1= atoi(s.c_str());
            return (ttl>=0 && ttl<=ran1);
        }
        else if(range == ttl_opt_length-1) //ran1 ~ 255
        {
            std::string s= ttl_opt.substr(0,ttl_opt_length-1);
            ran1= atoi(s.c_str());
            return (ttl>=ran1 && ttl<=255);
        }
        else // ran1 ~ ran2
        {
            std::string s1= ttl_opt.substr(0,range);
            ran1= atoi(s1.c_str());
            std::string s2= ttl_opt.substr(range+1,ttl_opt_length-1);
            ran2= atoi(s2.c_str());
            return (ttl>=ran1 && ttl<=ran2);
        }
        
    }
}
