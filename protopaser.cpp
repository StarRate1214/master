
#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>

enum Variable
{
    //Packet.h
    TCP,
    UDP,
    ICMP,
    //Rule.h IP,port option
    NOT,
    COMM,
    ANY,
    //Rule.h Option
    CONTENTS,
    PCRE,
    NPTTL,
    NPTOS,
    NPFBITS,
    NPDSIZE,
    NPFLAGS,
    NPSEQ,
    NPACK,
    NPWINDOW,
    NPITYPE,
    NPICODE,
    NPSAMEIP,
    NATION,
    //detection_filter.h
    SRC,
    DST,
    //Rule Action
    ALERT,
    LOG,
    PASS,
    //Protocols
    INSERT,
    UPDATE,
    DELETE
};

typedef struct
{
    std::string sig_action;
    std::string sig_protocol;
    std::string sig_srcIP;
    std::string sig_srcPort;
    std::string sig_direction;
    std::string sig_dstIP;
    std::string sig_dstPort;
} SRule_header;

typedef struct
{
    int order;
    int sig_id;
    SRule_header header;
    std::string option;
} sPROTOCOL;

sPROTOCOL Protocol_split(char *proto);
// void Pinsert(std::vector<CRule> *rules, sPROTOCOL sp);
// void Pdelete(std::vector<CRule> *rules, sPROTOCOL sp);

int main()
{
    char proto[1024] = "UPDATE sig_id=777, header=, option=content:\"HT,TP\";";

    sPROTOCOL ret = Protocol_split(proto);

    return 0;
}

sPROTOCOL Protocol_split(char *proto)
{
    //INSERT sig_id=777, header=alert tcp any any -> any any, option=content:\"HTTP\";
    //DELETE sig_id=1\n
    sPROTOCOL ret;
    char *ret_ptr;
    char *next_ptr;
    char *value;
    std::string *ptr = &ret.header.sig_action;
    //INSERT sig_id=777, header=alert tcp any any -> any any, option=content:\"HT,TP\";";
    if (proto[0] == 'I') //INSERT
    {
        ret.order = INSERT;

        ret_ptr = strtok_r(proto, ",", &next_ptr);
        strtok_r(ret_ptr, "=", &value);
        ret.sig_id = atoi(value);

        ret_ptr = strtok_r(NULL, ",", &next_ptr);
        strtok_r(ret_ptr, "=", &value);
        
        for (int i = 0; i < 7; i++)
        {
            *ptr = strtok_r(NULL, " ", &value);
            ptr++;
        }

        strtok_r(NULL, "=", &next_ptr);
        ret.option = next_ptr;

        std::cout << ret.header.sig_action << std::endl;
        std::cout << ret.header.sig_protocol << std::endl;
        std::cout << ret.header.sig_srcIP << std::endl;
        std::cout << ret.header.sig_srcPort << std::endl;
        std::cout << ret.header.sig_direction << std::endl;
        std::cout << ret.header.sig_dstIP << std::endl;
        std::cout << ret.header.sig_dstPort << std::endl;
        std::cout << ret.option << std::endl;
    }
    else if (proto[0] == 'U') //UPDATE
    {
        //UPDATE sig_id=777, header=, option=content:\"HT,TP\";
        ret.order = UPDATE;
        ret_ptr = strtok_r(proto, ",", &next_ptr);
        strtok_r(ret_ptr, "=", &value);
        ret.sig_id = atoi(value); //sig_id

        ret_ptr = strtok_r(NULL, ",", &next_ptr);
        strtok_r(ret_ptr, "=", &value);

        std::cout << ret_ptr << "_" << "_" << value << "_" << std::endl;
        if (value != NULL)
        {
            for (int i = 0; i < 7; i++)
            {
                *ptr = strtok_r(NULL, " ", &value);
                ptr++;
            }
        }

        // strtok_r(NULL, "=", &next_ptr);
        // ret.option = next_ptr;

        // std::cout << ret.header.sig_action << std::endl;
        // std::cout << ret.header.sig_protocol << std::endl;
        // std::cout << ret.header.sig_srcIP << std::endl;
        // std::cout << ret.header.sig_srcPort << std::endl;
        // std::cout << ret.header.sig_direction << std::endl;
        // std::cout << ret.header.sig_dstIP << std::endl;
        // std::cout << ret.header.sig_dstPort << std::endl;
    }
    else if (proto[0] == 'D') //DELETE
    {
        ret.order = DELETE;

        ret_ptr = strtok_r(NULL, ",", &next_ptr);
        strtok_r(ret_ptr, "=", &value);
        ret.sig_id = atoi(value);

        std::cout << ret.order << std::endl;
        std::cout << ret.sig_id << std::endl;
    }

    return ret;
}

// void Pinsert(std::vector<CRule> *rules, sPROTOCOL sp)
// {
//     CRule tmp(sp.sig_id, 1, sp.header, sp.option);
//     rules->push_back(tmp);
// }

// void Pdelete(std::vector<CRule> *rules, sPROTOCOL sp)
// {
//     for (int i = 0; i < rules->size(); i++)
//     {
//         if (rules->at(i).GetSig_id() == sp.sig_id)
//         {
//             rules->erase(rules->begin() + i);
//         }
//     }
// }