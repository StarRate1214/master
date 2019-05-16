#pragma once
#include "ids.h"
#include "Rule.h"
#include "Packet.h"
#include <mysql-cppconn-8/mysql/jdbc.h>
#include <inttypes.h>
class CDB
{
private:
    //sql::SQLString m_hostName; //db주소
    //sql::SQLString m_userName; //아이디
    //sql::SQLString m_dbName;   //db명
    //sql::SQLString m_password; //비밀번호
    sql::mysql::MySQL_Driver *m_driver;
    sql::Connection *m_conn;
    sql::PreparedStatement *m_strEvent;
    sql::PreparedStatement *m_strIPhdr;
    sql::PreparedStatement *m_strTCPhdr;
    sql::PreparedStatement *m_strUDPhdr;
    sql::PreparedStatement *m_strICMPhdr;
    sql::PreparedStatement *m_strPayload;
    sql::Statement *m_statement;
protected:
public:
    CDB(sql::SQLString hostName, sql::SQLString userName, sql::SQLString password, sql::SQLString dbName);
    ~CDB();
    void logging(CPacket *packet, u_int32_t sig_id);
    bool getRule(std::vector<CRule> *rules);
};