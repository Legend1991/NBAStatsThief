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
    QList<QPair<bool, bool> > parsePage();

private:
    void validate();
    QDate parseDate(QString &row);
    QPair<bool, bool> parseGame(QString &guestScoresRow, QString &homeScoresRow);
    bool isDateRow(QString &row);
    bool isGameRow(QString &row);
    QStringList tokenizeRow(QString &row);

    QString m_Page;
    bool m_IsPageValid;
    QDate m_GameDate;
};

#endif // PAGEPARSER_H
