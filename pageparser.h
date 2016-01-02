#ifndef PAGEPARSER_H
#define PAGEPARSER_H

#include <QString>
#include <QStringList>
#include "gamemodel.h"

class PageParser
{
public:
    PageParser(QString page, QDate currDate);
    bool isPageValid() { return m_IsPageValid; }
    QList<GameModel> parsePage();

private:
    void validate();
    QDate parseDate(QString &row);
    GameModel parseGame(QString &guestTeam, QString &homeTeam, QString &guestScoresRow, QString &homeScoresRow);
    QPair<QString, QString> parseTeamNames(QString teamNamesRow);
    bool isDateRow(QString &row);
    bool isGameRow(QString &row);
    QStringList tokenizeRow(QString &row);

    QString m_Page;
    bool m_IsPageValid;
    QDate m_GameDate;
};

#endif // PAGEPARSER_H
