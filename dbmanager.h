#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QList>
#include "gamemodel.h"

class DBManager
{
public:
    static DBManager *inst();
    void createDB(QString name);
    void add(QList<GameModel> games);

private:
    DBManager();
    ~DBManager();

    static DBManager *pInst;

    QSqlDatabase m_DB;
};

#endif // DBMANAGER_H
