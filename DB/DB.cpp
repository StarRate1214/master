#include "DB.h"

CDB::CDB(sql::SQLString hostName, sql::SQLString userName, sql::SQLString password, sql::SQLString dbName)//생성자
    : m_driver(0), m_conn(0),
{
    //this->m_hostName = hostName;
    //this->m_userName = userName;
    //this->m_password = password;
    //this->m_dbName = dbName;
    m_driver = sql::mysql::get_driver_instance();
    m_conn = m_driver->connect(hostName, userName, password);
    m_conn->setSchema(dbName);
    m_strEvent.reset(m_conn->prepareStatement("INSERT INTO event(sig_id, time) VALUES( ? ,FROM_UNIXTIME( ? ))"));
    m_statement = m_conn->createStatement();
/*
    sql::SQLString str = "USE ";
    str += dbName;
    std::auto_ptr<sql::Statement> statement;
    statement = m_conn->createStatement();
    statement->execute(str);
    delete statement;
*/
}
CDB::~CDB()//소멸자
{
    delete m_strEvent;
    delete m_statement;
    delete m_conn;
}

void CDB::logging(CPacket *packet, u_int32_t sig_id)//패킷과 룰 번호를 받아 db에 로그를 남김
{
    //event table에 로그 저장
    m_strEvent->setInt(1,sig_id);
    m_strEvent->setInt(2,packet->time);
    m_strEvent->execute();

    //방금 남긴 로그의 eid를 가져옴
    std::auto_ptr<sql::ResultSet> res;
    m_statement->execute("SELECT MAX(eid) AS eid FROM event");
    res.reset(m_statement->getResultSet());
    res->next();
    std::string eid=res->getString("eid");
    delete res;
/*
    switch (packet->protocol_type)
    {
    case TCP:
        //iphdr table에 저장 eid U_INT, src_ip  U_INT, dst_ip  U_INT, tos  U_TINYINT, ttl  U_TINYINT, more_frag   BOOLEAN, dont_frag   BOOLEAN,
        snprintf(buff, 512, "INSERT INTO iphdr VALUES (%s, %u, %u, %u, %u, %s, %s)", 
            eid, packet->tcp.getSrcIP(), packet->tcp.getDstIP(), packet->tcp.getTos(), packet->tcp.getTTL(), 
            packet->tcp.getMoreFrag()?"true":"false", packet->tcp.getDontFrag()?"true":"false");
        break;
    case UDP:
        break;
    case ICMP:
        break;
    default:
        break;
    }
    //iphdr table에 데이터 저장
    
*/
    //std::cout<<eid<<std::endl;
    /*
    if(packet->protocol_type==TCP)
    {

    }
    else if(packet->protocol_type==UDP)
    {

    }
    else if(packet->protocol_type==ICMP)
    {

    }*/
    //m_statement->execute("INSERT INTO test VALUES(1,2)");
}
void CDB::getRule()//db에서 룰을 가져옴
{
    sql::ResultSet *res;
    res = m_statement->executeQuery("SELECT * FROM test");
    int i;
    while (res->next())
    {
        std::cout<<i<<"line--------\n";
        std::cout << "0= " << res->getInt(0)<<std::endl;
        std::cout << "1= " << res->getInt(1)<<std::endl;
        std::cout << "2= " << res->getInt(2)<<std::endl;
        i++;
    }
    delete res;
}
