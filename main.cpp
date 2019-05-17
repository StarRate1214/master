#include "DB.h"
#include "RuleEngine.h"
#include"Capture.h"
#include <thread>
#include <queue>
#include <libconfig.h++>
//void compareRules(std::queue<CRawpacket> *p);
//void packetCapture(std::queue<CRawpacket> *p);

int main()
{
    //config 파일 읽어오기
    const char *config_path = "Observer.conf";
    libconfig::Config config;
    try
    {
        config.readFile(config_path);
    }
    catch (const libconfig::ConfigException &e)
    {
        std::cerr << "libconfig : " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    //모든 설정 로드
    const libconfig::Setting &root = config.getRoot();
    std::string hostName;
    std::string userName;
    std::string password;
    std::string dbName;

    //db정보 입력
    try
    {
        int error = 0;
        const libconfig::Setting &dbinfo = root["dbinfo"];
        error += dbinfo.lookupValue("hostName", hostName);
        error += dbinfo.lookupValue("userName", userName);
        error += dbinfo.lookupValue("password", password);
        error += dbinfo.lookupValue("dbName", dbName);
        if (error < 4)
            throw libconfig::ConfigException();
    }
    catch (const libconfig::ConfigException &e)
    {
        std::cerr << "Needs dbinfo option" << '\n';
    }

    //인터페이스 정보 입력
    std::string interface;
    try
    {
        interface = root["interface"].c_str();
    }
    catch (const libconfig::ConfigException &e)
    {
        std::cerr << "Needs interface option interface=\"interface name\"" << e.what() << '\n';
    }
    std::cout<<interface<<std::endl;
    //룰 백터와 패킷 큐 생성
    std::mutex *mtx= new std::mutex();
    std::queue<CRawpacket> *packetQueue = new std::queue<CRawpacket>;
    std::vector<CRule> *rules = new std::vector<CRule>;
    
    //DB연결
    CDB *db = new CDB(hostName, userName, password, dbName);
    if(!db->getRule(rules))
    {
        std::cerr<<"get rules from db error"<<'\n';
    }
    
    //CCapture capture(interface);
    
    //thread thread1(capture.packetCapture, packetQueue, mtx);
    //thread thread2(compareRules, packetQueue);

    //thread1.join();
    //thread2.join();

    delete packetQueue;
    delete rules;

    return 0;
}
/*
void packetCapture(std::queue<CRawpacket> *p)
{
    int sockfd, n;
    u_int8_t buff[ETH_FRAME_LEN];

    if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
    {
        // socket error
        perror("packet capture socket error: ");
    }
    while (1)
    {
        // get packet
        if ((n = recv(sockfd, buff, ETHER_MAX_LEN, 0)) < 0)
        {
            // recv error
            perror("packet capture recv error: ");
        }

        // ethernet type
        // u_int16_t ethertype = ((u_int16_t)buff[12] << 8) | buff[13];
        u_int16_t ethertype = (u_int16_t)buff[12];
        ethertype <<= 8;
        ethertype += buff[13];

        if (ethertype == ETHERTYPE_IP)
        {
            // ethernet + ip header length
            // int pkhl = (buff[14]&0x0F)*4;
            if (buff[23] == IPPROTO_TCP)
            {
                // input packet data in queue
                CRawpacket rawpacket(buff, n, time(NULL));

                mtx.lock();
                p->push(rawpacket);
                mtx.unlock();
            }
        }
    }
}

// pop data from queue ( Temporarily )
void compareRules(std::queue<CRawpacket> *p)
{
}*/