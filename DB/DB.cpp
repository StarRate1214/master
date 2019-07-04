#include "DB.h"

CDB::CDB(sql::SQLString hostName, sql::SQLString userName, sql::SQLString password, sql::SQLString dbName) //생성자
{
    //this->m_hostName = hostName;
    //this->m_userName = userName;
    //this->m_password = password;
    //this->m_dbName = dbName;
    m_driver = sql::mysql::get_driver_instance();
    m_conn = m_driver->connect(hostName, userName, password);
    m_conn->setSchema(dbName);
    m_statement = m_conn->createStatement();
    m_strEvent = m_conn->prepareStatement("INSERT INTO event(sig_id, time) VALUES( ? ,FROM_UNIXTIME( ? ))");
    //eid U_INT, src_ip U_INT, dst_ip U_INT, tos U_TINYINT, ttl U_TINYINT, more_frag BOOLEAN, dont_frag BOOLEAN
    m_strIPhdr = m_conn->prepareStatement("INSERT INTO  iphdr VALUES( ?, ?, ?, ?, ?, ?,?)");
    //eid U_INT, src_port U_SMALLINT, dst_port U_SMALLINT, seq_num U_INT, ack_num U_INT, urg BOOLEAN, ack BOOLEAN, psh BOOLEAN, rst BOOLEAN, syn BOOLEAN, fin BOOLEAN, win_size U_SMALLINT
    m_strTCPhdr = m_conn->prepareStatement("INSERT INTO tcphdr VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    //eid U_INT, src_port U_SMALLINT, dst_port    U_SMALLINT
    m_strUDPhdr = m_conn->prepareStatement("INSERT INTO udphdr VALUES(?, ?, ?)");
    //eid U_INT, type   U_TINYINT, code U_TINYINT
    m_strICMPhdr = m_conn->prepareStatement("INSERT INTO icmphdr VALUES(?, ?, ?)");
    //eid U_INT,  data_payload  TEXT
    m_strPayload = m_conn->prepareStatement("INSERT INTO data VALUES(?, ?)");
}
CDB::~CDB() //소멸자
{
    delete m_strICMPhdr;
    delete m_strUDPhdr;
    delete m_strTCPhdr;
    delete m_strIPhdr;
    delete m_strEvent;
    delete m_statement;
    delete m_conn;
}

unsigned int CDB::logging(CPacket &packet, u_int32_t sig_id, unsigned int eid = 0) //패킷과 룰 번호를 받아 db에 로그를 남김
{
    if (!eid)
    {
        //event table에 로그 저장
        m_strEvent->setUInt(1, sig_id);
        m_strEvent->setUInt(2, packet.time);
        m_strEvent->executeUpdate();

        //방금 남긴 로그의 eid를 가져옴
        sql::ResultSet *res;
        res = m_statement->executeQuery("SELECT MAX(eid) AS eid FROM event");
        res->next();
        eid = res->getUInt("eid");
        delete res;
    }
    //eid U_INT, src_ip  U_INT, dst_ip  U_INT, tos  U_TINYINT, ttl  U_TINYINT, more_frag   BOOLEAN, dont_frag   BOOLEAN
    m_strIPhdr->setUInt(1, eid);
    m_strIPhdr->setUInt(2, ntohl(packet.ip.getSrcIP()));
    m_strIPhdr->setUInt(3, ntohl(packet.ip.getDstIP()));
    m_strIPhdr->setUInt(4, packet.ip.getTos());
    m_strIPhdr->setUInt(5, packet.ip.getTTL());
    m_strIPhdr->setBoolean(6, packet.ip.getMoreFrag());
    m_strIPhdr->setBoolean(7, packet.ip.getDontFrag());
    m_strIPhdr->executeUpdate();

    switch (packet.protocol_type)
    {
    case TCP:
        //eid U_INT, src_port U_SMALLINT, dst_port U_SMALLINT, seq_num U_INT, ack_num U_INT, urg BOOLEAN, ack BOOLEAN, psh BOOLEAN, rst BOOLEAN, syn BOOLEAN, fin BOOLEAN, win_size U_SMALLINT
        m_strTCPhdr->setUInt(1, eid);
        m_strTCPhdr->setUInt(2, ntohs(packet.tcp.getSrcPort()));
        m_strTCPhdr->setUInt(3, ntohs(packet.tcp.getDstPort()));
        m_strTCPhdr->setUInt(4, packet.tcp.getSeqNum());
        m_strTCPhdr->setUInt(5, packet.tcp.getAckNum());
        m_strTCPhdr->setBoolean(6, packet.tcp.getUrg());
        m_strTCPhdr->setBoolean(7, packet.tcp.getAck());
        m_strTCPhdr->setBoolean(8, packet.tcp.getPsh());
        m_strTCPhdr->setBoolean(9, packet.tcp.getRst());
        m_strTCPhdr->setBoolean(10, packet.tcp.getSyn());
        m_strTCPhdr->setBoolean(11, packet.tcp.getFin());
        m_strTCPhdr->setUInt(12, packet.tcp.getWinSize());
        m_strTCPhdr->executeUpdate();
        break;
    case UDP:
        //eid U_INT, src_port U_SMALLINT, dst_port U_SMALLINT
        m_strUDPhdr->setUInt(1, eid);
        m_strUDPhdr->setUInt(2, ntohs(packet.udp.getSrcPort()));
        m_strUDPhdr->setUInt(3, ntohs(packet.udp.getDstPort()));
        m_strUDPhdr->executeUpdate();
        break;
    case ICMP:
        //eid U_INT, type   U_TINYINT, code U_TINYINT
        m_strICMPhdr->setUInt(1, eid);
        m_strICMPhdr->setUInt(2, packet.icmp.getICMPtype());
        m_strICMPhdr->setUInt(3, packet.icmp.getICMPcode());
        m_strICMPhdr->executeUpdate();
        break;
    default:
        break;
    }
    if (packet.data_payload_size > 0)
    {
        m_strPayload->setUInt(1, eid);
        struct membuf buff((char *)packet.data_payload, (char *)packet.data_payload + packet.data_payload_size);
        std::istream buf(&buff);
        m_strPayload->setBlob(2, &buf); //need to change
        m_strPayload->executeUpdate();
    }
    return eid;
}
int CDB::getRule(std::vector<CRule> *rules, std::unordered_map<std::string, std::string> vmap) //db에서 룰을 가져옴 CRule을 포인터(초기화 필요 없음)로 아니면 일반변수(초기화 필요?)로?
{
    sql::ResultSet *res;
    //sig_id  U_INT, sig_rule_header VARCHAR(255), sig_rule_option VARCHAR(255)
    u_int32_t sig_id;
    std::string rule_header;
    std::string rule_option;
    try
    {
        res = m_statement->executeQuery("SELECT sig_id, sig_rule_header, sig_rule_option FROM signature");
        while (res->next())
        {
            sig_id = res->getInt(1);
            rule_header = res->getString(2);
            int pos = 0, space = 0;
            std::string tmp;
            while ((pos = rule_header.find('$')) != -1)
            { //변수 찾기
                space = rule_header.find(' ', pos);
                tmp = rule_header.substr(pos + 1, space - 1 - pos);
                if (vmap[tmp] == "\0") //사용자가 입력한 변수가 없을 경우
                    return sig_id;
                rule_header.replace(pos, space - pos, vmap[tmp]);
            }
            rule_option = res->getString(3);
            CRule rule(sig_id, rule_header, rule_option);
            rules->push_back(rule);
        }
        delete res;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    return 0;
}
