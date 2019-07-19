#include "Mod_Rule.h"

CMod_Rule::CMod_Rule(std::vector<CRule> * rules,std::mutex *mtx, int portnum)
{
    this->portnum=portnum;
    this->rules=rules;
    this->mtx=mtx;
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
            //std::cout << buffer << std::endl;
            //코드 삽입 위치
        }
        close(connfd);
    }
}
