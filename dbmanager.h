#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>

class DBManager
{
public:
    static DBManager *inst();
    void setDatabaseName(QString name);

private:
    DBManager();
    ~DBManager();

    static DBManager *pInst;

    QSqlDatabase m_DB;
};

#endif // DBMANAGER_H
