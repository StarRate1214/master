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
} sPROTOCOL;

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

    sPROTOCOL Protocol_split(char *proto);
    void Pinsert(sPROTOCOL sp);
    void Pupdate(sPROTOCOL sp);
    void Pdelete(sPROTOCOL sp);

public:
    CMod_Rule(std::vector<CRule> *rules, std::mutex *mtx, int portnum, std::unordered_map<std::string, IP_value> *IP_map, std::unordered_map<std::string, Port_value> *Port_map);
    ~CMod_Rule();
    void MakeSocket();
    void run();
};