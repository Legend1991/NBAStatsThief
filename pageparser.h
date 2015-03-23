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
    void parsePage();

private:
    void validate();
    QDate parseDate(QString &row);
    GameModel parseGame(QDate date, QString &row);
    bool isDateRow(QString &row);
    bool isGameRow(QString &row);
    QStringList tokenizeRow(QString &row);

    QString m_Page;
    bool m_IsPageValid;
    QDate m_GameDate;
    QList<GameModel> m_Games;
};

#endif // PAGEPARSER_H
