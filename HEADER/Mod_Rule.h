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
    std::string option;
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

    sPROTOCOL Protocol_split(char *proto);
    void Pinsert(std::vector<CRule> *rules, sPROTOCOL sp);
    void Pdelete(std::vector<CRule> *rules, sPROTOCOL sp);

public:
    CMod_Rule(std::vector<CRule> *rules, std::mutex *mtx, int portnum = 5252);
    ~CMod_Rule();
    void MakeSocket();
    int UpdateRule(std::string header, std::string option);
    void run();
};