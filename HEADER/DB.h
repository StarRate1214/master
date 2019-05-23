#pragma once
#include "ids.h"
#include "Rule.h"
#include "Packet.h"
#include <mysql-cppconn-8/mysql/jdbc.h>
#include <inttypes.h>
class CDB
{
private:
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
    void logging(CPacket& packet, u_int32_t sig_id);
    int getRule(std::vector<CRule> *rules, std::unordered_map<std::string, std::string> vmap);
};