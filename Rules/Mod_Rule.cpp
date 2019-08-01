#include "Mod_Rule.h"

CMod_Rule::CMod_Rule(std::vector<CRule> *rules, std::mutex *mtx, int portnum, std::unordered_map<std::string, IP_value> *IP_map, std::unordered_map<std::string, Port_value> *Port_map)
{
    this->portnum = portnum;
    this->rules = rules;
    this->mtx = mtx;
    this->IP_map = IP_map;
    this->Port_map = Port_map;
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
            std::string tmp = buffer;

            if (buffer[0] == 'R') //Rule
            {
                R_PROTOCOL s = R_Protocol_split(buffer);

                if (s.order == INSERT)
                    R_Pinsert(s);
                else if (s.order == UPDATE)
                    R_Pupdate(s);
                else if (s.order == DELETE)
                    R_Pdelete(s);
                syslog(LOG_INFO | LOG_LOCAL0, "[Modify Rule] %s\n", tmp.c_str());
            }
            else if (buffer[0] == 'O') //Object
            {
                if (buffer[1] == 'I') //IP
                {
                    O_PROTOCOL s = O_Protocol_split(buffer);

                    if (s.order == INSERT)
                        OI_Pinsert(s);
                    else if (s.order == UPDATE)
                        OI_Pupdate(s);
                    else if (s.order == DELETE)
                        OI_Pdelete(s);
                    syslog(LOG_INFO | LOG_LOCAL0, "[Modify Rule] %s\n", tmp.c_str());
                }
                else //PORT
                {
                    O_PROTOCOL s = O_Protocol_split(buffer);

                    if (s.order == INSERT)
                        OP_Pinsert(s);
                    else if (s.order == UPDATE)
                        OP_Pupdate(s);
                    else if (s.order == DELETE)
                        OP_Pdelete(s);
                    syslog(LOG_INFO | LOG_LOCAL0, "[Modify Rule] %s\n", tmp.c_str());
                }
            }
            else if (buffer[0] == 'S') //Sig_run
            {
                Change_SigRun(buffer);
            }
        }
        close(connfd);
    }
}

R_PROTOCOL CMod_Rule::R_Protocol_split(char *proto)
{
    R_PROTOCOL ret;
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

void CMod_Rule::R_Pinsert(R_PROTOCOL rp)
{
    CRule tmp(rp.sig_id, 1, rp.header, rp.option, IP_map, Port_map);
    rules->push_back(tmp);
    //std::cout << rules->at(0).GetSig_id() << std::endl;
    //std::cout << rules->at(0).GetAction() << std::endl;
    //std::cout << rules->at(0).GetProtocols() << std::endl;
    //std::cout << rules->at(0).GetRuleOptions().at(0).option<< std::endl;
}

void CMod_Rule::R_Pdelete(R_PROTOCOL rp)
{
    for (int i = 0; i < rules->size(); i++)
    {
        if (rules->at(i).GetSig_id() == rp.sig_id)
        {
            rules->erase(rules->begin() + i);
        }
    }
}

void CMod_Rule::R_Pupdate(R_PROTOCOL rp)
{
    for (int i = 0; i < rules->size(); i++)
    {
        if (rules->at(i).GetSig_id() == rp.sig_id)
        {
            if (!rp.header.sig_action.empty())
                rules->at(i).SetHeader(rp.header);
            if (!rp.option.empty())
                rules->at(i).SetOptions(rp.option);
            //std::cout << rules->at(i).GetSig_id() << std::endl;
            //std::cout << rules->at(i).GetAction() << std::endl;
            //std::cout << rules->at(i).GetProtocols() << std::endl;
            //std::cout << rules->at(i).GetRuleOptions().at(1).option<< std::endl;
        }
    }
}

void CMod_Rule::Change_SigRun(char *proto)
{
    char *ret_ptr;
    char *next_ptr;
    char *value;
    int sig_id;

    ret_ptr = strtok_r(proto, ",", &next_ptr);

    strtok_r(ret_ptr, "=", &value);
    sig_id = atoi(value);
    for (int i = 0; i < rules->size(); i++)
    {
        if (rules->at(i).GetSig_id() == sig_id)
        {
            if (next_ptr[1] == 't') //true
                rules->at(i).SetSig_run(true);
            else //false
                rules->at(i).SetSig_run(false);
        }
    }
}

O_PROTOCOL CMod_Rule::O_Protocol_split(char *proto)
{
    O_PROTOCOL ret;
    char *ret_ptr;
    char *next_ptr;
    char *value;

    if (proto[3] == 'I') //INSERT
    {
        ret.order = INSERT;
        strtok_r(proto, " ", &next_ptr);

        ret_ptr = strtok_r(NULL, " ", &next_ptr);
        strtok_r(ret_ptr, "=", &value);
        ret.name = value;

        strtok_r(NULL, "=", &value);
        ret.value = value;
    }
    else if (proto[3] == 'U') //UPDATE
    {
        ret.order = UPDATE;
        strtok_r(proto, " ", &next_ptr);

        ret_ptr = strtok_r(proto, " ", &next_ptr);
        strtok_r(ret_ptr, "=", &value);
        ret.name = value;
        
        strtok_r(NULL, "=", &value);
        ret.value = value;
    }
    else if (proto[3] == 'D') //DELETE
    {
        ret.order = DELETE;
        strtok_r(proto, "=", &value);
        ret.name = value;
    }

    return ret;
}

void CMod_Rule::OI_Pinsert(O_PROTOCOL oi)
{
}
void CMod_Rule::OI_Pupdate(O_PROTOCOL oi)
{
}
void CMod_Rule::OI_Pdelete(O_PROTOCOL oi)
{
}

void CMod_Rule::OP_Pinsert(O_PROTOCOL op)
{
}
void CMod_Rule::OP_Pupdate(O_PROTOCOL op)
{
}
void CMod_Rule::OP_Pdelete(O_PROTOCOL op)
{
}