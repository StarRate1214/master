#pragma once
#include "ids.h"
#include <cstdio>
#include "Rule.h"
#include "Capture.h"

#define MAXBUFFER 1024

typedef struct
{
    int order;
    int sig_id;
    SRule_header header;
    std::string option="";
} R_PROTOCOL;

typedef struct
{
    int order;
    std::string name;
    std::string value;
}O_PROTOCOL;

class CMod_Rule
{
private:
    int listenfd;
    char buffer[MAXBUFFER];
    struct sockaddr_in servaddr;
    int portnum;
    std::vector<CRule> *rules;
    std::mutex *mtx;
    std::unordered_map<std::string, IP_value> *IP_map;
    std::unordered_map<std::string, Port_value> *Port_map;

    R_PROTOCOL R_Protocol_split(char *proto);
    void R_Pinsert(R_PROTOCOL rp);
    void R_Pupdate(R_PROTOCOL rp);
    void R_Pdelete(R_PROTOCOL rp);

    O_PROTOCOL O_Protocol_split(char *proto);
    void OI_Pinsert(O_PROTOCOL oi);
    void OI_Pupdate(O_PROTOCOL oi);
    void OI_Pdelete(O_PROTOCOL oi);

    void OP_Pinsert(O_PROTOCOL op);
    void OP_Pupdate(O_PROTOCOL op);
    void OP_Pdelete(O_PROTOCOL op);

    void Change_SigRun(char* proto);
public:
    CMod_Rule(std::vector<CRule> *rules, std::mutex *mtx, int portnum, std::unordered_map<std::string, IP_value> *IP_map, std::unordered_map<std::string, Port_value> *Port_map);
    ~CMod_Rule();
    void MakeSocket();
    void run();
};