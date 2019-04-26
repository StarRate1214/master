#include <string>
#include <cstdlib>

bool TTL(std::string ttl_opt,u_int8_t ttl)
{
    int ttl_opt_length = ttl_opt.length();
    int ran1,ran2; //범위
    if(ttl_opt[0]=='<')
    {
        if(ttl_opt[1]=='=') // <=
        {
            ran1= stoi(ttl_opt.substr(2,ttl_opt_length-2));
            return (ttl<=ran1); 
        }
        else // <
        {
            ran1= stoi(ttl_opt.substr(1,ttl_opt_length-1));
            return (ttl<ran1); 
        }
    }
    else if(ttl_opt[0]=='>')
    {
        if(ttl_opt[1]=='=') // >=
        {
            ran1= stoi(ttl_opt.substr(2,ttl_opt_length-2));
            return (ttl>=ran1); 
        }
        else // >
        {
            ran1= stoi(ttl_opt.substr(1,ttl_opt_length-1));
            return (ttl>ran1); 
        }
    }
    else if(ttl_opt[0]=='=') // =
    {
        ran1= stoi(ttl_opt.substr(1,ttl_opt_length-1));
        return (ttl==ran1); 
    }
    else //'-' 범위 지정 연산자
    {
        int range = ttl_opt.find('-');
        if(range==0) //0~ran1
        {
            ran1= stoi(ttl_opt.substr(1,ttl_opt_length-1));
            return (ttl>=0 && ttl<=ran1);
        }
        else if(range == ttl_opt_length-1) //ran1 ~ 255
        {
            ran1= stoi(ttl_opt.substr(0,ttl_opt_length-1));
            return (ttl>=ran1 && ttl<=255);
        }
        else // ran1 ~ ran2
        {
            ran1= stoi(ttl_opt.substr(0,range));
            ran2= stoi(ttl_opt.substr(range+1,ttl_opt_length-1));
            return (ttl>=ran1 && ttl<=ran2);
        }
    }
}

bool Tos(std::string tos_opt,u_int8_t tos)
{
    int tos_opt_length = tos_opt.length();
    int _tos;
    if(tos_opt[0]=='!')
    {
        _tos = stoi(tos_opt.substr(1,tos_opt_length-1));
        return (_tos != tos);
    }
    else
    {
        _tos = stoi(tos_opt);
        return (_tos == tos);
    }    
}