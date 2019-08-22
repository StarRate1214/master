#include "Nation.h"

CNation::CNation(sql::SQLString hostName, sql::SQLString userName, sql::SQLString password, sql::SQLString dbName)
//생성자
{

    // std::cout << "geodb 연동 시작.." << std::endl;
    m_driver = sql::mysql::get_driver_instance();
    m_conn = m_driver->connect(hostName, userName, password);
    m_conn->setSchema(dbName);
    m_statement = m_conn->createStatement();
}

CNation::~CNation()
{
    delete m_statement;
    delete m_conn;
}

int CNation::Nation(CPacket &packet, std::string cou)
{
    sql::ResultSet *res;
    std::string geo_name; //나중에 지울것
    in_addr sip;
    in_addr dip;
    sip.s_addr = (packet.ip.getSrcIP());
    dip.s_addr = (packet.ip.getDstIP());
    std::string srcip = inet_ntoa(sip);
    std::string dstip = inet_ntoa(dip);

    try
    {
        res = m_statement->executeQuery("SELECT * FROM ip_locations WHERE geoname_id = IP2Location('" + srcip + "');");
        while (res->next())
        {
            geo_name = res->getString("country_iso_code");

            if (geo_name == cou)
            {
                return true;
            }
        }

        res = m_statement->executeQuery("SELECT * FROM ip_locations WHERE geoname_id = IP2Location('" + dstip + "');");
        while (res->next())
        {
            geo_name = res->getString("country_iso_code");

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
        return false;
    }
}