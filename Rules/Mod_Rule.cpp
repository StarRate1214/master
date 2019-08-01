#include "Mod_Rule.h"

CMod_Rule::CMod_Rule(std::vector<CRule> *rules, std::mutex *mtx, int portnum, std::unordered_map<std::string, IP_value> *IP_map, std::unordered_map<std::string, Port_value> *Port_map)
{
    this->portnum = portnum;
    this->rules = rules;
    this->mtx = mtx;
    this->IP_map=IP_map;
    this->Port_map=Port_map;
}

CMod_Rule::~CMod_Rule()
{
}

void CMod_Rule::MakeSocket()
{
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error");
        syslog(LOG_INFO | LOG_LOCAL0, "[Socket Error] Socket Generation Error\n");
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
        syslog(LOG_INFO | LOG_LOCAL0, "[Socket Error] Socket Bind Error\n");
        exit(S_BIND_ERROR);
    }
}

void CMod_Rule::run()
{
    if (listen(listenfd, SOMAXCONN) < 0)
    {
        perror("listen error");
        syslog(LOG_INFO | LOG_LOCAL0, "[Socket Error] Socket Listen Error\n");
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
            syslog(LOG_INFO | LOG_LOCAL0, "[Socket Error] Socket Accept Error\n");
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
            std::string tmp=buffer;
            sPROTOCOL s = Protocol_split(buffer);

            if (s.order == INSERT)
                Pinsert(s);
            else if (s.order == UPDATE)
                Pupdate(s);
            else if (s.order == DELETE)
                Pdelete(s);
            syslog(LOG_INFO | LOG_LOCAL0, "[Modify Rule] %s\n",tmp.c_str());
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

void CMod_Rule::Pinsert(sPROTOCOL sp)
{
    CRule tmp(sp.sig_id, 1, sp.header, sp.option, IP_map, Port_map);
    rules->push_back(tmp);
    //std::cout << rules->at(0).GetSig_id() << std::endl;
    //std::cout << rules->at(0).GetAction() << std::endl;
    //std::cout << rules->at(0).GetProtocols() << std::endl;
    //std::cout << rules->at(0).GetRuleOptions().at(0).option<< std::endl;

}

void CMod_Rule::Pdelete(sPROTOCOL sp)
{
    for (int i = 0; i < rules->size(); i++)
    {
        if (rules->at(i).GetSig_id() == sp.sig_id)
        {
            rules->erase(rules->begin() + i);
        }
    }
}

void CMod_Rule::Pupdate(sPROTOCOL sp)
{
    for (int i = 0; i < rules->size(); i++)
    {
        if (rules->at(i).GetSig_id() == sp.sig_id)
        {
            if (!sp.header.sig_action.empty())
                rules->at(i).SetHeader(sp.header);
            if (!sp.option.empty())
                rules->at(i).SetOptions(sp.option);
        //std::cout << rules->at(i).GetSig_id() << std::endl;
        //std::cout << rules->at(i).GetAction() << std::endl;
        //std::cout << rules->at(i).GetProtocols() << std::endl;
        //std::cout << rules->at(i).GetRuleOptions().at(1).option<< std::endl;
        }
        
    }
}