#include "DB.h"
#include "RuleEngine.h"
#include "Capture.h"
#include <thread>
#include <queue>
#include <libconfig.h++>

void compareRules(std::queue<CRawpacket*> *packetQueue, std::vector<CRule> *rules, CDB *db, std::mutex *mtx);

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

    std::string variable;
    std::string value;
    //변수 정보 입력
    try
    {
        const libconfig::Setting &variables = root["variables"];
        for (int i = 0; i < variables.getLength(); i++)
        {
            variable=variables[i].getName();
        }
        std::cout<<variable<<std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    //룰 백터와 패킷 큐 생성
    std::mutex *mtx = new std::mutex();
    std::queue<CRawpacket*> *packetQueue = new std::queue<CRawpacket*>;
    std::vector<CRule> *rules = new std::vector<CRule>;

    //DB연결
    CDB *db = new CDB(hostName, userName, password, dbName);
    if (!db->getRule(rules))
    {
        std::cerr << "get rules from db error" << '\n';
    }

    //CCapture capture(interface);

    //thread thread1(capture.packetCapture, packetQueue, mtx);
    //thread thread2(compareRules, packetQueue, rules, db, mtx);

    //thread1.join();
    //thread2.join();

    delete packetQueue;
    delete rules;

    return 0;
}

void compareRules(std::queue<CRawpacket*> *packetQueue, std::vector<CRule> *rules, CDB *db, std::mutex *mtx)
{
    CRuleEngine ruleEngine;
    CRawpacket *rwpack;
    u_int32_t ruleNumber;
    while (1)
    {
        //패킷 큐가 비어있는지 확인
        if (packetQueue->empty())
            continue;
        //큐 잠그기
        mtx->lock();
        rwpack = packetQueue->front();
        packetQueue->pop();
        mtx->unlock(); //잠금해재
        ruleEngine.PacketLoad(rwpack);
        ruleNumber = 0;
        while (1)
        {
            ruleNumber = ruleEngine.Compare(rules, ruleNumber);
            if (ruleNumber < 0)
                break;
            db->logging(ruleEngine.getPacket(), rules->at(ruleNumber));
        }
        delete rwpack;
    }
}