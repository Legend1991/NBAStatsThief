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
    void loadCurrentMonth();
    void loadNextMonth();
    void loadFinished(bool ok);

    QDate m_FromDate;
    QDate m_ToDate;
    QDate m_CurrDate;
    QWebView *m_WebView;
};

#endif // STATSLOADER_H
