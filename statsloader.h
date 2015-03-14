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
    void load(int avLeagScore);
    void setTimeRange(QDate fromDate, QDate toDate);

signals:
    void loadStarted(QDate currDate);
    void loadProgress(int progress);
    void loaded();
    void noGames();

private:
    void loadCurrentMonth();
    void loadNextMonth();
    void loadFinished(bool ok);
    bool isDateRow(QString &row);
    bool isGameRow(QString &row);
    bool isValidPage(QString &page);
    void parsePage(QString &page);
    QStringList tokenizeRow(QString &row);
    QDate parseDate(QString &row);
    GameModel parseGame(QDate date, QString &row);

    QDate m_FromDate;
    QDate m_ToDate;
    QDate m_CurrDate;
    QDate m_GameDate;
    QList<GameModel> m_Games;
    QList<QPair<QString, QString> > m_GamesForCalc;
    int m_AvLeagScore;
    QWebView *m_WebView;
};

#endif // STATSLOADER_H
