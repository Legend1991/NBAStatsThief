#include "dbmanager.h"

DBManager *DBManager::pInst = 0;

DBManager *DBManager::inst()
{
    if (!pInst) {
        pInst = new DBManager();
    }
    return pInst;
}

DBManager::DBManager()
{
    m_DB = QSqlDatabase::addDatabase("QSQLITE");
}

DBManager::~DBManager()
{

}

void DBManager::setDatabaseName(QString name)
{
    m_DB.setDatabaseName(name);
    if (!m_DB.open()) {
        qDebug() << "Can't open database: " << name;
    }
}
