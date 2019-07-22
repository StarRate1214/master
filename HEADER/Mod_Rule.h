#pragma once
#include <ids.h>
#include <cstdio>
#include<Rule.h>
#define MAXBUFFER 1024

class CMod_Rule
{
private:
    int listenfd;
    char buffer[MAXBUFFER];
    struct sockaddr_in servaddr;
    int portnum;
    std::vector<CRule> * rules;
    std::mutex *mtx;
public:
    CMod_Rule(std::vector<CRule> * rules, std::mutex *mtx, int portnum = 5252);
    ~CMod_Rule();
    void MakeSocket();
    int UpdateRule(std::string header, std::string option);
    void run();
};