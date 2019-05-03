#pragma once
#include <mysql-cppconn-8/mysql/jdbc.h>

class CDB
{
private:
    sql::SQLString m_hostName;
    sql::SQLString m_userName;
    sql::SQLString m_dbName;
    sql::SQLString m_password;
    sql::mysql::MySQL_Driver *m_driver;
    sql::Connection *m_conn;
    sql::Statement *m_statement;

protected:
public:
    CDB(sql::SQLString hostName, sql::SQLString userName, sql::SQLString password, sql::SQLString dbName);
    ~CDB();
    void connectDB(void);
    void logging(void);
    void getRule(void);
};