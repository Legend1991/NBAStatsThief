#include <QWebFrame>
#include <QWebSettings>
#include "statsloader.h"
#include "pageparser.h"
#include "dbmanager.h"

const int JULY = 7;
const int AUGUST = 8;
const int SEPTEMBER = 9;

typedef QPair<bool, bool> boolPair;

StatsLoader::StatsLoader(QObject *parent) :
    QObject(parent),
    m_FromDate(),
    m_ToDate()
{
    _GamesCount = 0;
    _WinsCount = 0;
    m_WebView = new QWebView();
    connect(m_WebView, &QWebView::loadProgress, this, &StatsLoader::loadProgress);
    connect(m_WebView->page()->mainFrame(), &QWebFrame::loadFinished, this, &StatsLoader::loadFinished);
//    connect(m_WebView, &QWebView::loadFinished, this, &StatsLoader::loadFinished);
}

StatsLoader::~StatsLoader()
{
    delete m_WebView;
}

void StatsLoader::load()
{
//    m_CurrDate = m_FromDate;
    m_CurrDate.setDate(2014, 6, 5);
    m_ToDate.setDate(2014, 7, 4);
    loadCurrentDay();
}

void StatsLoader::setTimeRange(QDate fromDate, QDate toDate)
{
    QDate currentDate = QDate::currentDate();
    m_ToDate = toDate > currentDate ? currentDate : toDate;
    m_FromDate = fromDate > m_ToDate ? m_ToDate : fromDate;
}

void StatsLoader::loadCurrentDay()
{
    if (m_CurrDate.month() == JULY ||
            m_CurrDate.month() == AUGUST ||
            m_CurrDate.month() == SEPTEMBER)
    {
        loadNextDay();
        return;
    }

    emit loadStarted(m_CurrDate);
    QString currDate = m_CurrDate.toString("yyyyMMdd");
    QUrl url = QUrl(QString("http://www.nba.com/gameline/%1/").arg(currDate));
    m_WebView->load(url);
}

void StatsLoader::loadFinished(bool ok)
{
    if (!ok) return;

    QString page = m_WebView->page()->mainFrame()->toPlainText();
    PageParser parser(page, m_CurrDate);

    if (!parser.isPageValid())
    {
        qDebug() << "======================== Invalid " << m_CurrDate;
//        qDebug() << page;
//        m_WebView->triggerPageAction(QWebPage::ReloadAndBypassCache);
        loadNextDay();
        return;
    }

//    qDebug() << m_CurrDate;
//    QList<boolPair> result = parser.parsePage();
//    foreach (boolPair game, result)
//    {
//        if (game.first)
//        {
//            _GamesCount++;
//            if (game.second)
//            {
//                _WinsCount++;
//            }
//        }
//    }
//    qDebug() << "Games " << _GamesCount << "\tWins " << _WinsCount;

    QList<GameModel> result = parser.parsePage();
    qDebug() << m_CurrDate;
    foreach (GameModel game, result) {
        qDebug() << game.getVisitorTeam() << game.getHomeTeam() << game.getVTotal() << game.getHTotal();
    }
    DBManager::inst()->add(result);

    loadNextDay();
}

void StatsLoader::loadNextDay()
{
    m_CurrDate = m_CurrDate.addDays(1);

    if (m_CurrDate <= m_ToDate)
    {
//        m_WebView->stop();
//        disconnect(m_WebView, &QWebView::loadProgress, this, &StatsLoader::loadProgress);
//        disconnect(m_WebView, &QWebView::loadFinished, this, &StatsLoader::loadFinished);
//        delete m_WebView;
//        m_WebView = new QWebView();
//        connect(m_WebView, &QWebView::loadProgress, this, &StatsLoader::loadProgress);
//        connect(m_WebView, &QWebView::loadFinished, this, &StatsLoader::loadFinished);
        loadCurrentDay();
    }
    else
    {
        emit loaded();
        qDebug() << "Final:\tGames " << _GamesCount << "\tWins " << _WinsCount;
    }
}
