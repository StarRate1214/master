#include "DB.h"
#include "RuleEngine.h"
#include "Capture.h"
#include "Mod_Rule.h"
#include <thread>
#include <queue>
#include <libconfig.h++>

void compareRules(std::queue<CRawpacket *> *packetQueue, std::vector<CRule> *rules, CDB *db, std::mutex *mtx, CNation *country);
void modifyRules(std::vector<CRule> * rules, std::mutex *mtx, std::unordered_map<std::string, IP_value> *IP_map, std::unordered_map<std::string, Port_value> *Port_map);
int main()
{
    openlog("[Observer]", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog(LOG_INFO | LOG_LOCAL0, "[Observer Start]\n");
    //config 파일 읽어오기
    std::cout << "load settting......" << std::endl;
    const char *config_path = "Observer.conf";
    libconfig::Config config;
    try
    {
        config.readFile(config_path);
    }
    catch (const libconfig::ConfigException &e)
    {
        std::cerr << "libconfig : " << e.what() << '\n';
        syslog(LOG_INFO | LOG_LOCAL0, "[File Error] Observer.conf File Read Error\n");
        return C_FILE_ERROR;
    }

    //모든 설정 로드
    const libconfig::Setting &root = config.getRoot();
    std::string hostName;
    std::string userName;
    std::string password;
    std::string dbName;

    std::string g_hostName;
    std::string g_userName;
    std::string g_password;
    std::string g_dbName;

    //db정보 입력
    try
    {
        const libconfig::Setting &dbinfo = root["dbinfo"];
        if (!(dbinfo.lookupValue("hostName", hostName) && dbinfo.lookupValue("userName", userName) && dbinfo.lookupValue("password", password) && dbinfo.lookupValue("dbName", dbName)))
        {
            std::cerr << "dbinfo needs hostName, userName, password, dbName\n";
            syslog(LOG_INFO | LOG_LOCAL0, "[DB Error] Setting DB information in setup file\n");
            return C_DBINFO_ERROR;
        }
    }
    catch (const libconfig::ConfigException &e)
    {
        std::cerr << "Needs dbinfo option" << '\n';
        syslog(LOG_INFO | LOG_LOCAL0, "[Setting Error] Needs dbinfo option\n");
        return C_DBINFO_ERROR;
    }

    try
    {
        const libconfig::Setting &geoinfo = root["geoinfo"];

        if (!(geoinfo.lookupValue("g_hostName", g_hostName) && geoinfo.lookupValue("g_userName", g_userName) && geoinfo.lookupValue("g_password", g_password) && geoinfo.lookupValue("g_dbName", g_dbName)))
        {
            std::cerr << "geoinfo needs g_hostName, g_userName, g_password, g_dbName\n";
            syslog(LOG_INFO | LOG_LOCAL0, "[DB Error] Setting DB information for geoinfo in setup file\n");
            return C_GEOINFO_ERROR;
        }
    }
    catch (const libconfig::ConfigException &e)
    {
        std::cerr << "Needs geoinfo option" << '\n';
        syslog(LOG_INFO | LOG_LOCAL0, "[Setting Error] Needs geoinfo option\n");
        return C_GEOINFO_ERROR;
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
        syslog(LOG_INFO | LOG_LOCAL0, "[File Error] Setting interface information in setup file\n");
        return C_INTERFACE_ERROR;
    }

    std::cout << "rule load......" << std::endl;
    //룰 백터와 패킷 큐 생성
    std::mutex *mtx = new std::mutex();
    std::queue<CRawpacket *> *packetQueue = new std::queue<CRawpacket *>;
    std::vector<CRule> *rules = new std::vector<CRule>;

    //DB연결
    CDB *db = new CDB(hostName, userName, password, dbName);
    CNation *country = new CNation(g_hostName, g_userName, g_password, g_dbName);

    std::unordered_map<std::string, IP_value> *IP_map = new std::unordered_map<std::string, IP_value>;
    std::unordered_map<std::string, Port_value> *Port_map = new std::unordered_map<std::string, Port_value>;

    if(!db->getRule(rules, IP_map, Port_map))
    {
        std::cerr << "get rules from db error" << '\n';
        return D_GETRULE_ERROR;
    }
        

    

    if(!db->getVariable(IP_map, Port_map))
    {
        std::cerr << "get variables from db error" << '\n';
        return D_GETVARIABLE_ERROR;
    }

    std::cout << "start......" << std::endl;
    CCapture capture(interface);
    std::thread thread1([&]() { capture.packetCapture(packetQueue, mtx); });
    std::thread thread2(compareRules, packetQueue, rules, db, mtx, country);
    std::thread thread3(modifyRules, rules, mtx, IP_map, Port_map);

    thread1.join();
    thread2.join();
    thread3.join();

    delete Port_map;
    delete IP_map;
    delete packetQueue;
    delete rules;
    delete mtx;
    closelog();
    return 0;
}

void compareRules(std::queue<CRawpacket *> *packetQueue, std::vector<CRule> *rules, CDB *db, std::mutex *mtx, CNation *country)
{
    CRuleEngine ruleEngine;
    CRawpacket *rwpack;

    int ruleNumber;

    while (1)
    {
        //패킷 큐가 비어있는지 확인
        if (packetQueue->empty())
            continue;
        //큐 잠그기
        mtx->lock();
        rwpack = packetQueue->front();
        packetQueue->pop();
        mtx->unlock();
        //잠금해재
        //패킷을 가공
        ruleEngine.PacketLoad(rwpack);
        delete rwpack;

        ruleNumber = 0;
        //룰 백터 잠그기
        mtx->lock();
        while (1)
        {
            ruleNumber = ruleEngine.Compare(rules, country, ruleNumber);
            if (ruleNumber < 0)
                break;
            
            if (rules->at(ruleNumber).GetAction() == ALERT)
            {
                std::cout << rules->at(ruleNumber).GetSig_id() << " is matched." << std::endl;
                db->logging(ruleEngine.getPacket(), rules->at(ruleNumber).GetSig_id());
            }
            else if (rules->at(ruleNumber).GetAction() == LOG)
            {
                db->logging(ruleEngine.getPacket(), rules->at(ruleNumber).GetSig_id());
            }
            else if (rules->at(ruleNumber).GetAction() == PASS)
            {
                break;
            }
            ruleNumber++;
        }
        mtx->unlock();
        //룰 백터 잠그기
    }
}
void modifyRules(std::vector<CRule> * rules, std::mutex *mtx, std::unordered_map<std::string, IP_value> *IP_map, std::unordered_map<std::string, Port_value> *Port_map)
{
    CMod_Rule mod_rule(rules, mtx, 5252, IP_map, Port_map);
    mod_rule.MakeSocket();
    mod_rule.run();
}