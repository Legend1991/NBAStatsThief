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
        QString strQuery = QString("INSERT INTO Games (Date, HomeTeamId, VisitorTeamId, "
                                   "HQ1, VQ1, "
                                   "HQ2, VQ2, "
                                   "HQ3, VQ3, "
                                   "HQ4, VQ4, "
                                   "HTotal, VTotal) "
                                   "SELECT \"%1\", HomeTeam.Id, VisitorTeam.Id, %4, %5, %6, %7, %8, %9, %10, %11, %12 "
                                   "FROM (SELECT Teams.Id FROM Teams WHERE Teams.ShortName = \"%2\") AS HomeTeam, "
                                   "     (SELECT Teams.Id FROM Teams WHERE Teams.ShortName = \"%3\") AS VisitorTeam;")
                .arg(game.getDate().toString("yyyyMMdd").toInt())
                .arg(game.getHomeTeam())
                .arg(game.getVisitorTeam())
                .arg(game.getHQ1())
                .arg(game.getVQ1())
                .arg(game.getHQ2())
                .arg(game.getVQ2())
                .arg(game.getHQ3())
                .arg(game.getVQ3())
                .arg(game.getHQ4())
                .arg(game.getVQ4())
                .arg(game.getHTotal())
                .arg(game.getVTotal());

        if (!query.exec(strQuery))
        {
            qDebug() << "Can't exec query: " << strQuery << "\tError: " << query.lastError().text();
        }
    }
}
