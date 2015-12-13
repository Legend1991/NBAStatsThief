#ifndef STATSLOADER_H
#define STATSLOADER_H

#include <QDate>
#include <QWebView>
#include "gamemodel.h"

class StatsLoader : public QObject
{
    Q_OBJECT

public:
    explicit StatsLoader(QObject *parent = 0);
    ~StatsLoader();
    void load();
    void setTimeRange(QDate fromDate, QDate toDate);

signals:
    void loadStarted(QDate currDate);
    void loadProgress(int progress);
    void loaded();

private:
    void loadCurrentDay();
    void loadNextDay();
    void loadFinished(bool ok);

    QDate m_FromDate;
    QDate m_ToDate;
    QDate m_CurrDate;
    QWebView *m_WebView;
    uint _GamesCount;
    uint _WinsCount;
};

#endif // STATSLOADER_H
