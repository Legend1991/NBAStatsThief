#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
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
    if (!QFile::copy(":/nbadb.sqlite", name))
    {
        qDebug() << "Can't create database: " << name;
    }
    QFile dbfile(name);
    dbfile.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser | QFile::WriteUser);
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
        QString strQuery = QString("INSERT INTO Games (Date, HomeTeam, HomeScore, VisitorTeam, VisitorScore) "
                                   "SELECT \"%1\", HomeTeam.Id, %3, VisitorTeam.Id, %5 "
                                   "FROM (SELECT Teams.Id FROM Teams WHERE Teams.ShortName = \"%2\") AS HomeTeam, "
                                   "     (SELECT Teams.Id FROM Teams WHERE Teams.ShortName = \"%4\") AS VisitorTeam;")
                                .arg(game.getDate().toString("yyyyMMdd").toInt())
                                .arg(game.getHomeTeam())
                                .arg(game.getHomeScore())
                                .arg(game.getVisitorTeam())
                                .arg(game.getVisitorScore());

        if (!query.exec(strQuery))
        {
            qDebug() << "Can't exec query: " << strQuery << "\tError: " << query.lastError().text();
        }
    }
}
