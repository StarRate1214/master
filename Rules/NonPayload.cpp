#include "NonPayload.h"

bool CNonPayload::TTL(std::string ttl_opt,u_int8_t ttl)
{
    int ttl_opt_length = ttl_opt.length();
    int ran1,ran2; //범위
    if(ttl_opt[0]=='<')
    {
        if(ttl_opt[1]=='=') // <=
        {
            ran1= std::stoi(ttl_opt.substr(2,ttl_opt_length-2));
            return (ttl<=ran1); 
        }
        else // <
        {
            ran1= std::stoi(ttl_opt.substr(1,ttl_opt_length-1));
            return (ttl<ran1); 
        }
    }
    else if(ttl_opt[0]=='>')
    {
        if(ttl_opt[1]=='=') // >=
        {
            ran1= std::stoi(ttl_opt.substr(2,ttl_opt_length-2));
            return (ttl>=ran1); 
        }
        else // >
        {
            ran1= std::stoi(ttl_opt.substr(1,ttl_opt_length-1));
            return (ttl>ran1); 
        }
    }
    else if(ttl_opt[0]=='=') // =
    {
        ran1= std::stoi(ttl_opt.substr(1,ttl_opt_length-1));
        return (ttl==ran1); 
    }
    else //'-' 범위 지정 연산자
    {
        int range = ttl_opt.find('-');
        if(range==0) //0~ran1
        {
            ran1= std::stoi(ttl_opt.substr(1,ttl_opt_length-1));
            return (ttl>=0 && ttl<=ran1);
        }
        else if(range == ttl_opt_length-1) //ran1 ~ 255
        {
            ran1= std::stoi(ttl_opt.substr(0,ttl_opt_length-1));
            return (ttl>=ran1 && ttl<=255);
        }
        else // ran1 ~ ran2
        {
            ran1= std::stoi(ttl_opt.substr(0,range));
            ran2= std::stoi(ttl_opt.substr(range+1,ttl_opt_length-1));
            return (ttl>=ran1 && ttl<=ran2);
        }
    }
}

bool CNonPayload::Tos(std::string tos_opt,u_int8_t tos)
{
    int tos_opt_length = tos_opt.length();
    int _tos;
    if(tos_opt[0]=='!')
    {
        _tos = std::stoi(tos_opt.substr(1,tos_opt_length-1));
        return (_tos != tos);
    }
    else
    {
        _tos = std::stoi(tos_opt);
        return (_tos == tos);
    }    
}

bool CNonPayload::Fragbits(std::string fragbits_opt, bool mfrag, bool dfrag)
{
    bool mf, df;
    mf =(((int)fragbits_opt.find('M')) != -1) ? true: false;
    df =(((int)fragbits_opt.find('D')) != -1) ? true: false;
    if(((int)fragbits_opt.find('*')) != -1)
    {
        if(mf)
        {
            if(mf!= mfrag)
                return false;
        }
        if(df)
        {
            if(df!= dfrag)
                return false;   
        }
    }
    else if(((int)fragbits_opt.find('!')) != -1)
    {
        if(mf)
        {
            if(mf== mfrag)
                return false;
        }
        if(df)
        {
            if(df==dfrag)
                return false;
        }
    }
    return true;
}

bool CNonPayload::dsize(std::string dsize_opt, u_int32_t dsize) // data payload size
{
    int num1 = dsize_opt.find('<'); 
    int num2 = dsize_opt.find('>'); 
    if(( num1 != -1) && ( num2 != -1 )) //range num1~num2
    {
        num1=std::stoi(dsize_opt.substr(0,num1));
        num2=std::stoi(dsize_opt.substr(num2));
        if((num1 < dsize) && (dsize > num2) )
            return true;
    }
    else if(num1 != -1) //dpisze < num
    {
        num1 = std::stoi(dsize_opt.substr(1));
        if(dsize < num1)
            return true;
    }
    else // dpsize > num
    {
        num2 = std::stoi(dsize_opt.substr(1));
        if(dsize > num2)
            return true;
    }
    return false;
}

bool CNonPayload::flags(std::string flags_opt, bool fin, bool syn, bool rst, bool psh, bool ack, bool urg)
{ //확인 후 콜론 추가
    bool F, S, R, P, A, U;
    F =(((int)flags_opt.find('F')) != -1) ? true: false;
    S =(((int)flags_opt.find('S')) != -1) ? true: false;
    R =(((int)flags_opt.find('R')) != -1) ? true: false;
    P =(((int)flags_opt.find('P')) != -1) ? true: false;
    A =(((int)flags_opt.find('A')) != -1) ? true: false;
    U =(((int)flags_opt.find('U')) != -1) ? true: false;
    if(((int)flags_opt.find('*')) != -1)
    {
       if(F)
       {
            if(F!=fin)
                return false;
       }
       if(S)
       {
            if(S!=syn)
                return false;
       }
       if(R)
       {
            if(R!=rst)
                return false;
       }
       if(P)
       {
            if(P!=psh)
                return false;
       }
       if(A)
       {
            if(A!=ack)
                return false;
       }
       if(U)
       {
            if(U!=urg)
                return false;
       }
    }
    else if(((int)flags_opt.find('!')) != -1)
    {
        if(F)
       {
            if(F==fin)
                return false;
       }
       if(S)
       {
            if(S==syn)
                return false;
       }
       if(R)
       {
            if(R==rst)
                return false;
       }
       if(P)
       {
            if(P==psh)
                return false;
       }
       if(A)
       {
            if(A==ack)
                return false;
       }
       if(U)
       {
            if(U==urg)
                return false;
       }
    }
    return true;
}


bool CNonPayload::seq(std::string seq_opt, u_int32_t seq_num)
{
    if(((u_int32_t)std::stoi(seq_opt))==seq_num)
        return true;
    else
        return false;    
}
bool CNonPayload::ack(std::string ack_opt, u_int32_t ack_num)
{
    if(((u_int32_t)std::stoi(ack_opt))==ack_num)
        return true;
    else
        return false;    
}

bool CNonPayload::window(std::string window_opt, u_int16_t win_size)
{
    if(window_opt[0]=='!')
    {
        if(((u_int16_t)std::stoi(window_opt))!=win_size)
            return true; 
    }
    else
    {
        if(((u_int16_t)std::stoi(window_opt))==win_size)
            return true; 
    }
    return false;
}
bool CNonPayload::itype(std::string itype_opt, u_int8_t icmp_type)
{
    int num1 = itype_opt.find('<'); 
    int num2 = itype_opt.find('>'); 
    if(( num1 != -1) && ( num2 != -1 )) //range num1~num2
    {
        num1=std::stoi(itype_opt.substr(0,num1));
        num2=std::stoi(itype_opt.substr(num2));
        if((num1 < icmp_type) && (icmp_type > num2) )
            return true;
    }
    else if(num1 != -1) //dpisze < num
    {
        num1 = std::stoi(itype_opt.substr(1));
        if(icmp_type < num1)
            return true;
    }
    else // dpsize > num
    {
        num2 = std::stoi(itype_opt.substr(1));
        if(icmp_type > num2)
            return true;
    }
    return false;
}
bool CNonPayload::icode(std::string icode_opt, u_int8_t icmp_code)
{
    int num1 = icode_opt.find('<'); 
    int num2 = icode_opt.find('>'); 
    if(( num1 != -1) && ( num2 != -1 )) //range num1~num2
    {
        num1=std::stoi(icode_opt.substr(0,num1));
        num2=std::stoi(icode_opt.substr(num2));
        if((num1 < icmp_code) && (icmp_code > num2) )
            return true;
    }
    else if(num1 != -1) //dpisze < num
    {
        num1 = std::stoi(icode_opt.substr(1));
        if(icmp_code < num1)
            return true;
    }
    else // dpsize > num
    {
        num2 = std::stoi(icode_opt.substr(1));
        if(icmp_code > num2)
            return true;
    }
    return false;
}