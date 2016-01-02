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
        if (row == QString("FINAL"))
        {
            m_IsPageValid = true;
            return;
        }
    }
}

QList<GameModel> PageParser::parsePage()
{
    QList<GameModel> games;

    QStringList rows = m_Page.split("\n");

    for (int i = 0; i < rows.length(); ++i)
    {
        QString row = rows[i];
//        qDebug() << row;
        if (row == QString("FINAL"))
        {
            ++i;
            ++i; // for < 2015
            QPair<QString, QString> teamNames = parseTeamNames(rows[i]);
//            ++i;
//            QPair<QString, QString> teamNames;
//            QString guestScoresRow = (rows[++i]).mid(4); // for < 2015
//            teamNames.first = (rows[i]).mid(0, 3); // for < 2015
//            QString homeScoresRow = (rows[++i]).mid(4); // for < 2015
//            teamNames.second = (rows[i]).mid(0, 3); // for < 2015
            QString guestScoresRow = rows[++i];
            QString homeScoresRow = rows[++i];
            guestScoresRow += " " + rows[++i]; // for < 2013
            homeScoresRow += " " + rows[++i]; // for < 2013
//            qDebug () << "======= rows " << teamNames.first << guestScoresRow << "\n" << teamNames.second << homeScoresRow;
            games.append(parseGame(teamNames.first, teamNames.second, guestScoresRow, homeScoresRow));
        }
    }

    return games;
}

QDate PageParser::parseDate(QString &row)
{
    QStringList dateTokens = tokenizeRow(row);
    QLocale locale(QLocale::English, QLocale::UnitedStates);
    QString strDate = QString("%1%2%3").arg(m_GameDate.year()).arg(dateTokens.at(1)).arg(dateTokens.at(2));
    return locale.toDate(strDate, "yyyyMMMdd");
}

GameModel PageParser::parseGame(QString &guestTeam, QString &homeTeam, QString &guestScoresRow, QString &homeScoresRow)
{
    QStringList guestScoresTokens = tokenizeRow(guestScoresRow);
    QStringList homeScoresTokens  = tokenizeRow(homeScoresRow);

    if (guestScoresTokens.length() < 5 || homeScoresTokens.length() < 5)
    {
        qDebug() << "====================== " << guestScoresRow << homeScoresRow;
        return GameModel();
    }

    bool ok;

    int gQ1 = guestScoresTokens.at(0).toInt(&ok);
    int gQ2 = guestScoresTokens.at(1).toInt(&ok);
    int gQ3 = guestScoresTokens.at(2).toInt(&ok);
    int gQ4 = guestScoresTokens.at(3).toInt(&ok);

    int hQ1 = homeScoresTokens.at(0).toInt(&ok);
    int hQ2 = homeScoresTokens.at(1).toInt(&ok);
    int hQ3 = homeScoresTokens.at(2).toInt(&ok);
    int hQ4 = homeScoresTokens.at(3).toInt(&ok);

    int guestTotalScore = guestScoresTokens.last().toInt(&ok);
    int homeTotalScore = homeScoresTokens.last().toInt(&ok);

    if (!ok)
    {
        qDebug() << "====================== " << guestScoresRow << homeScoresRow;
        return GameModel();
    }

//    int guestQ3Score              = gQ1 + gQ2; // + gQ3;
//    int homeQ3Score               = hQ1 + hQ2; // + hQ3;

//    QPair<bool, bool> result;

//    if (qAbs(guestQ3Score - homeQ3Score) >= 5 && qAbs(guestQ3Score - homeQ3Score) <= 8)
//    {
//        if ((guestQ3Score > homeQ3Score && guestTotalScore > homeTotalScore) ||
//                (guestQ3Score < homeQ3Score && guestTotalScore < homeTotalScore))
//        {
//            qDebug() << guestTotalScore << homeTotalScore << guestScoresRow << homeScoresRow;;
//            result = QPair<bool, bool>(true, true);
//        }
//        else
//        {
//            result = QPair<bool, bool>(true, false);
//        }
//    }
//    else
//    {
//        result = QPair<bool, bool>(false, false);
//    }

    return GameModel(m_GameDate, homeTeam, guestTeam,
                     hQ1, gQ1,
                     hQ2, gQ2,
                     hQ3, gQ3,
                     hQ4, gQ4,
                     homeTotalScore, guestTotalScore);
}

QPair<QString, QString> PageParser::parseTeamNames(QString teamNamesRow)
{
    QString guestTeam = teamNamesRow.mid(0, 3);

    int i = 0;
    bool ok = false;

    for (; i < teamNamesRow.length() - 3 && !ok; ++i)
    {
        teamNamesRow.mid(i, 1).toInt(&ok);
    }

    for (; i < teamNamesRow.length() - 3 && ok;)
    {
        ++i;
        teamNamesRow.mid(i, 1).toInt(&ok);
    }

    QString homeTeam = teamNamesRow.mid(i, 3);

    return QPair<QString, QString>(guestTeam, homeTeam);
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
