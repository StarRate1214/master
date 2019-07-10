#include "DB.h"
#include "RuleEngine.h"
#include "Capture.h"
#include <thread>
#include <queue>
#include <libconfig.h++>
#include "detection_filter.h"
void compareRules(std::queue<CRawpacket *> *packetQueue, std::vector<CRule> *rules, CDB *db, std::mutex *mtx);
bool CompareConditions(u_int32_t sig_id, int limit, int timeout, CPacket packet);
int main()
{
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
        const libconfig::Setting &dbinfo = root["dbinfo"];
        if (!(dbinfo.lookupValue("hostName", hostName) && dbinfo.lookupValue("userName", userName) && dbinfo.lookupValue("password", password) && dbinfo.lookupValue("dbName", dbName)))
        {
            std::cerr << "dbinfo needs hostName, userName, password, dbName\n";
            return EXIT_FAILURE;
        }
    }
    catch (const libconfig::ConfigException &e)
    {
        std::cerr << "Needs dbinfo option" << '\n';
        return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    }

    std::string vname;
    std::string value;
    std::unordered_map<std::string, std::string> vmap;
    //변수 정보 입력
    try
    {
        const libconfig::Setting &variables = root["variables"];
        for (int i = 0; i < variables.getLength(); i++)
        {
            vname = variables[i].getName();
            if (!variables.lookupValue(vname, value))
            {
                std::cerr << "value must be string\n";
                return EXIT_FAILURE;
            }
            vmap[vname] = value;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    std::cout << "rule load......" << std::endl;
    //룰 백터와 패킷 큐 생성
    std::mutex *mtx = new std::mutex();
    std::queue<CRawpacket *> *packetQueue = new std::queue<CRawpacket *>;
    std::vector<CRule> *rules = new std::vector<CRule>;

    //DB연결
    CDB *db = new CDB(hostName, userName, password, dbName);
    switch (int sig = db->getRule(rules, vmap))
    {
    case -1:
        std::cerr << "get rules from db error" << '\n';
        break;
    case 0:
        break;
    default:
        std::cerr << "sig_id : " << sig << " has invalid variable\n";
    }
    std::cout << "start......" << std::endl;
    CCapture capture(interface);

    std::thread thread1([&]() { capture.packetCapture(packetQueue, mtx); });
    std::thread thread2(compareRules, packetQueue, rules, db, mtx);

    thread1.join();
    thread2.join();

    delete packetQueue;
    delete rules;
    delete mtx;
    return 0;
}

void compareRules(std::queue<CRawpacket *> *packetQueue, std::vector<CRule> *rules, CDB *db, std::mutex *mtx)
{
    CRuleEngine ruleEngine;
    CRawpacket *rwpack;
    std::vector<CDetection_filter> d_filter;
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
        ruleNumber = 0;
        while (1)
        {
            ruleNumber = ruleEngine.Compare(rules, ruleNumber);
            if (ruleNumber < 0)
                break;
            ////////////////여기부터 d_filter씀
            time_t timeout = rules->at(ruleNumber).GetD_filter().timeout;
            int limit = rules->at(ruleNumber).GetD_filter().limit;
            u_int32_t ip_addr;
            if(rules->at(ruleNumber).GetD_filter().track==SRC)
                ip_addr=ruleEngine.getPacket().ip.getSrcIP();
            else
                ip_addr=ruleEngine.getPacket().ip.getDstIP();

            if (timeout!=0)
            {
                //로그남기기 & alert
                u_int32_t sig_id = rules->at(ruleNumber).GetSig_id();
                u_int8_t rev = rules->at(ruleNumber).GetRev();
                int i;
                for (i = 0; i < d_filter.size(); i++)
                {
                    if (d_filter[i].getsig_id() == sig_id && d_filter[i].getip_addr() == ip_addr)
                    {
                        if(d_filter[i].getrev()!=rev)
                        {
                            d_filter.erase(d_filter.begin()+i);
                            i=d_filter.size();
                        }
                        break;
                    }
                }
                if (i == d_filter.size())
                {
                    CDetection_filter c(sig_id, rev, limit, timeout, ip_addr);
                    d_filter.push_back(c);
                }
                d_filter[i].insertPacket(ruleEngine.getPacket()); // 패킷넣는 함수
                d_filter[i].deleteTimeOutPacket();                //<-이라인에 지우는함수

                if (d_filter[i].isMatched())
                {
                    d_filter[i].logging(db);                             //로그 남기는 함수
                    if (rules->at(ruleNumber).GetAction() == "alert") //액션이 alert일때
                        std::cout << rules->at(ruleNumber).GetSig_id() << " is AAAAAmatched." << std::endl;
                }
                ruleNumber++;
                continue;
            }

            //////////////////////////여기까지
            if (rules->at(ruleNumber).GetAction() == "alert")
            {

                std::cout << rules->at(ruleNumber).GetSig_id() << " is matched." << std::endl;
                db->logging(ruleEngine.getPacket(), rules->at(ruleNumber).GetSig_id());
            }
            else if (rules->at(ruleNumber).GetAction() == "log")
            {
                db->logging(ruleEngine.getPacket(), rules->at(ruleNumber).GetSig_id());
            }
            else if (rules->at(ruleNumber).GetAction() == "pass")
            {
                break;
            }
            ruleNumber++;
        }
        delete rwpack;
    }
}

bool CompareConditions(u_int32_t sig_id, int limit, int timeout, CPacket packet)
{
}