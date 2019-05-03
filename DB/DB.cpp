#include "DB.h"

CDB::CDB(sql::SQLString hostName, sql::SQLString userName, sql::SQLString password, sql::SQLString dbName)
    : m_driver(0), m_conn(0), m_statement(0)
{
    this->m_hostName = hostName;
    this->m_userName = userName;
    this->m_password = password;
    this->m_dbName = dbName;
}
CDB::~CDB()
{
    delete m_statement;
    delete m_conn;
}
void CDB::connectDB(void)
{
    sql::SQLString str = "USE ";
    str += m_dbName;
    m_driver = sql::mysql::get_driver_instance();
    m_conn = m_driver->connect(m_hostName, m_userName, m_password);
    m_statement = m_conn->createStatement();
    m_statement->execute(str);
}
void CDB::logging()
{
    m_statement->execute("INSERT INTO test VALUES(1,2)");
}
void CDB::getRule()
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
