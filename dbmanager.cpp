#include <QSqlQuery>
#include <QDebug>
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

void DBManager::setPath(QString name)
{
    m_DB.close();
    m_DB.setDatabaseName(name);
    if (!m_DB.open())
    {
        qDebug() << "Can't open database: " << name;
    }
}

void DBManager::add(QList<GameModel> games)
{
    QSqlQuery query(m_DB);

    foreach (GameModel game, games)
    {
        QString strQuery = QString("INSERT INTO Games"
                                   "(Date, HomeTeam, HomeScore, VisitorTeam, VisitorScore)"
                                   "VALUES (%1, '%2', %3, '%4', %5);")
                                .arg(game.getDate().toString("dd.MM.yyyy"))
                                .arg(game.getHomeTeam()).arg(game.getHomeScore())
                                .arg(game.getVisitorTeam()).arg(game.getVisitorScore());

        if (!query.exec(strQuery))
        {
            qDebug() << "Can't exec query: " << strQuery;
        }
    }
}
