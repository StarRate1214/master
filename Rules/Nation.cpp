#include "Nation.h"

CNation::CNation(sql::SQLString hostName, sql::SQLString userName, sql::SQLString password, sql::SQLString dbName)
//생성자
{
    //this->m_hostName = hostName;
    //this->m_userName = userName;
    //this->m_password = password;
    //this->m_dbName = dbName;
    std::cout << "db 연동 시작.." << std::endl;
    m_driver = sql::mysql::get_driver_instance();
    m_conn = m_driver->connect(hostName, userName, password);
    m_conn->setSchema(dbName);
    m_statement = m_conn->createStatement();
    std::cout << "db 연동 끝.." << std::endl;
}

CNation::~CNation()
{
    std::cout << "정리.." << std::endl;
    delete m_statement;
    delete m_conn;
}

int CNation::Nation(CPacket &packet, std::string cou)
{
    sql::ResultSet *res;
    std::string geo_name;//나중에 지울것
    in_addr sip;
    in_addr dip;
    sip.s_addr = (packet.ip.getSrcIP());
    dip.s_addr = (packet.ip.getDstIP());
    std::string srcip = inet_ntoa(sip);
    std::string dstip = inet_ntoa(dip);

    std::cout << "sIP: " << srcip<<", "<<sip.s_addr<<", "<<packet.ip.getSrcIP()<< " dIP: " << dstip <<", "<<dip.s_addr<<", "<<packet.ip.getDstIP() <<std::endl;    
    std::cout << "Nation 함수 진입.." << std::endl;

    try
    {
        res = m_statement->executeQuery("SELECT * FROM ip_locations WHERE geoname_id = IP2Location('" + srcip + "');");
        while (res->next())
        {
            geo_name = res->getString("country_iso_code");
            std::cout << "db에서 geo_name가져옴.." << geo_name << std::endl;

            if (geo_name == cou)
            {
                return true;
            }
        }

        res = m_statement->executeQuery("SELECT * FROM ip_locations WHERE geoname_id = IP2Location('" + dstip + "');");
        while (res->next())
        {
            geo_name = res->getString("country_iso_code");
            std::cout << "db에서 geo_name가져옴.." << geo_name << std::endl;
            if (geo_name == cou)
            {
                return true;
            }
        }

        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
}