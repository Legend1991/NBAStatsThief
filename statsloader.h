#ifndef STATSLOADER_H
#define STATSLOADER_H

#include <QDate>
#include <QWebPage>

class StatsLoader : public QObject
{
    Q_OBJECT

public:
    explicit StatsLoader(QDate fromDate, QDate toDate, QObject *parent = 0);
    ~StatsLoader();
    void load();

private:
    void loadFinished(bool ok);

    QDate m_FromDate;
    QDate m_ToDate;
    QWebPage m_Page;
};

#endif // STATSLOADER_H
