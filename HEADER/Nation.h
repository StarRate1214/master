#pragma once
#include "ids.h"
#include "Rule.h"
#include "Packet.h"
#include <mysql-cppconn-8/mysql/jdbc.h>

//룰 바디에 옵션 추가 국가 필터링 함. bool 함수 (string con){1. db에서 국가들 가져와서 벡터에 만들어줌 2. 국가 - ip 만들어서 약간 리스트처럼 해서 이제 패킷의 아이피 가져와서 비교 3. 참 거짓 해서 참이면 알아서 하길..}
class CNation
{
private:
    sql::mysql::MySQL_Driver *m_driver;
    sql::Connection *m_conn;
    sql::Statement *m_statement;

public:
    CNation(sql::SQLString hostName, sql::SQLString userName, sql::SQLString password, sql::SQLString dbName);
    ~CNation();
    int Nation(CPacket &packet, std::string cou); //국가명은 대문자로 iso_code에 따른다. 예) KR
};