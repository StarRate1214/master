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
    std::auto_ptr<sql::Connection> m_conn;
    std::auto_ptr<sql::PreparedStatement> m_strEvent;
    std::auto_ptr<sql::PreparedStatement> m_strIPhdr;
    std::auto_ptr<sql::PreparedStatement> m_strTCPhdr;
    std::auto_ptr<sql::PreparedStatement> m_strUDPhdr;
    std::auto_ptr<sql::PreparedStatement> m_strICMPhdr;
    std::auto_ptr<sql::PreparedStatement> m_strPayload;
    std::auto_ptr<sql::Statement> m_statement;
protected:
public:
    CDB(sql::SQLString hostName, sql::SQLString userName, sql::SQLString password, sql::SQLString dbName);
    ~CDB();
    void logging(CPacket *packet, u_int32_t sig_id);
    void getRule(void);
};