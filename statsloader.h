#ifndef STATSLOADER_H
#define STATSLOADER_H

#include <QDate>
#include <QWebView>

class StatsLoader : public QObject
{
    Q_OBJECT

public:
    explicit StatsLoader(QObject *parent = 0);
    ~StatsLoader();
    void load();
    void setTimeRange(QDate fromDate, QDate toDate) {
        m_FromDate = fromDate;
        m_ToDate = toDate;
    }

signals:
    void loadStarted(QDate currDate);
    void loadProgress(int progress);

private:
    void loadCurrentMonth();
    void loadNextMonth();
    void loadFinished(bool ok);
    bool isDateRow(QString &row);
    bool isGameRow(QString &row);
    QStringList tokenizeRow(QString &row);
    QDate parseDate(QString &row);

    QDate m_FromDate;
    QDate m_ToDate;
    QDate m_CurrDate;
    QWebView *m_WebView;
};

#endif // STATSLOADER_H
