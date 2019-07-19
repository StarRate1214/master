#pragma once
#include "ids.h"
#include "Rule.h"
#include "Packet.h"
#include <mysql-cppconn-8/mysql/jdbc.h>
#include <inttypes.h>
#include <streambuf>
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
    sql::PreparedStatement *m_strVariable;
    sql::Statement *m_statement;

protected:
    int CheckisVariable(std::string &str);

public:
    CDB(sql::SQLString hostName, sql::SQLString userName, sql::SQLString password, sql::SQLString dbName);
    ~CDB();
    unsigned int logging(CPacket &packet, u_int32_t sig_id);
    int getRule(std::vector<CRule> *rules);
};

struct membuf : std::streambuf
{
    membuf(char *begin, char *end) { this->setg(begin, begin, end); }
};