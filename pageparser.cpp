#include <QDebug>
#include "pageparser.h"

PageParser::PageParser(QString page, QDate currDate) :
    m_Page(page),
    m_IsPageValid(false),
    m_GameDate(currDate)
{
    validate();
}

void PageParser::validate()
{
    QStringList rows = m_Page.split("\n");

    foreach (QString row, rows)
    {
        if (isGameRow(row) || isDateRow(row))
        {
            m_IsPageValid = true;
        }
    }
}

void PageParser::parsePage()
{
    QStringList rows = m_Page.split("\n");

    foreach (QString row, rows)
    {
        if (isDateRow(row))
        {
            m_GameDate = parseDate(row);
        }

        if (isGameRow(row))
        {
            GameModel game = parseGame(m_GameDate, row);
            m_Games.append(game);
            qDebug() << game.getDate()
                     << game.getHomeTeam() << game.getHomeScore()
                     << game.getVisitorScore() << game.getVisitorTeam();
        }
    }
}

QDate PageParser::parseDate(QString &row)
{
    QStringList dateTokens = tokenizeRow(row);
    QLocale locale(QLocale::English, QLocale::UnitedStates);
    QString strDate = QString("%1%2%3").arg(m_GameDate.year()).arg(dateTokens.at(1)).arg(dateTokens.at(2));
    return locale.toDate(strDate, "yyyyMMMdd");
}

GameModel PageParser::parseGame(QDate date, QString &row)
{
    QStringList gameTokens = tokenizeRow(row);
    QString visitorTeam = gameTokens.at(2);
    QString homeTeam = gameTokens.at(4);
    int visitorScore = gameTokens.at(5).toInt();
    int homeScore = gameTokens.at(7).toInt();

    return GameModel(date, homeTeam, homeScore, visitorTeam, visitorScore);
}

bool PageParser::isDateRow(QString &row)
{
    QStringList splitedRow = tokenizeRow(row);
    return splitedRow.count() == 3 && QRegExp(".*\\d$").exactMatch(row);
}

bool PageParser::isGameRow(QString &row)
{
    QStringList splitedRow = tokenizeRow(row);
    return splitedRow.count() == 8;
}

QStringList PageParser::tokenizeRow(QString &row)
{
    return row.trimmed().split(QRegExp("\\s+|\\t+"));
}
