#include "Mod_Rule.h"

CMod_Rule::CMod_Rule(std::vector<CRule> *rules, std::mutex *mtx, int portnum)
{
    this->portnum = portnum;
    this->rules = rules;
    this->mtx = mtx;
}

CMod_Rule::~CMod_Rule()
{
}

void CMod_Rule::MakeSocket()
{
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error");
        exit(S_SOCKET_ERROR);
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(portnum);
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind error");
        exit(S_BIND_ERROR);
    }
}

void CMod_Rule::run()
{
    if (listen(listenfd, SOMAXCONN) < 0)
    {
        perror("listen error");
        exit(S_LISTEN_ERROR);
    }
    while (true)
    {
        int connfd;
        int n;
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len)) < 0)
        {
            perror("accept error");
            exit(S_ACCEPT_ERROR);
        }
        while (true)
        {
            std::cout << "recv" << std::endl;
            if ((n = recv(connfd, buffer, MAXBUFFER, 0)) < 0)
            {
                perror("recv error");
                break;
            }
            else if (n == 0)
            {
                std::cout << "closed" << std::endl;
                break;
            }
            buffer[n] = 0;

            sPROTOCOL s = Protocol_split(buffer);
            // Pinsert(&rules, s);
            std::cout << s.order << std::endl;
            std::cout << s.sig_id << std::endl;
            std::cout << s.header.sig_action << std::endl;
            std::cout << s.header.sig_protocol << std::endl;
            std::cout << s.header.sig_srcIP << std::endl;
            std::cout << s.header.sig_srcPort << std::endl;
            std::cout << s.header.sig_direction << std::endl;
            std::cout << s.header.sig_dstIP << std::endl;
            std::cout << s.header.sig_dstPort << std::endl;
            std::cout << s.option << std::endl;
        }
        close(connfd);
    }
}

sPROTOCOL CMod_Rule::Protocol_split(char *proto)
{
    sPROTOCOL ret;
    char *ret_ptr;
    char *next_ptr;
    char *value;
    std::string *ptr = &ret.header.sig_action;

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
    }
    else if (proto[0] == 'U') //UPDATE
    {
        ret.order = UPDATE;
        ret_ptr = strtok_r(proto, ",", &next_ptr);
        strtok_r(ret_ptr, "=", &value);
        ret.sig_id = atoi(value); //sig_id

        ret_ptr = strtok_r(NULL, ",", &next_ptr);
        strtok_r(ret_ptr, "=", &value);

        if (*value != 0)
        {
            for (int i = 0; i < 7; i++)
            {
                *ptr = strtok_r(NULL, " ", &value);
                ptr++;
            }
        }

        strtok_r(NULL, "=", &next_ptr);
        if (*next_ptr != 0)
        {
            ret.option = next_ptr;
        }
    }
    else if (proto[0] == 'D') //DELETE
    {
        ret.order = DELETE;
        strtok_r(proto, "=", &value);
        ret.sig_id = atoi(value);
    }

    return ret;
}

void CMod_Rule::Pinsert(std::vector<CRule> *rules, sPROTOCOL sp)
{
    CRule tmp(sp.sig_id, 1, sp.header, sp.option);
    rules->push_back(tmp);
}

void CMod_Rule::Pdelete(std::vector<CRule> *rules, sPROTOCOL sp)
{
    for (int i = 0; i < rules->size(); i++)
    {
        if (rules->at(i).GetSig_id() == sp.sig_id)
        {
            rules->erase(rules->begin() + i);
        }
    }
}