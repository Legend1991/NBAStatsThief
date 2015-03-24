#include <QWebFrame>
#include "statsloader.h"
#include "pageparser.h"
#include "dbmanager.h"

const int JULY = 7;
const int AUGUST = 8;
const int SEPTEMBER = 9;

StatsLoader::StatsLoader(QObject *parent) :
    QObject(parent),
    m_FromDate(),
    m_ToDate()
{
    m_WebView = new QWebView();
    connect(m_WebView, &QWebView::loadProgress, this, &StatsLoader::loadProgress);
    connect(m_WebView, &QWebView::loadFinished, this, &StatsLoader::loadFinished);
}

StatsLoader::~StatsLoader()
{
    delete m_WebView;
}

void StatsLoader::load()
{
    m_CurrDate = m_FromDate;
    loadCurrentMonth();
}

void StatsLoader::setTimeRange(QDate fromDate, QDate toDate)
{
    QDate currentDate = QDate::currentDate();
    m_ToDate = toDate > currentDate ? currentDate : toDate;
    m_FromDate = fromDate > m_ToDate ? m_ToDate : fromDate;
}

void StatsLoader::loadCurrentMonth()
{
    if (m_CurrDate.month() == JULY ||
            m_CurrDate.month() == AUGUST ||
            m_CurrDate.month() == SEPTEMBER)
    {
        loadNextMonth();
    }

    emit loadStarted(m_CurrDate);
    QString month = QString("%1").arg(m_CurrDate.month(), 2, 10, QChar('0'));
    QUrl url = QUrl(QString("http://mi.nba.com/schedule/#!/%1/%2").arg(month).arg(m_CurrDate.year()));
    m_WebView->setUrl(url);
    m_WebView->load(url);
}

void StatsLoader::loadFinished(bool ok)
{
    if (!ok) return;

    QString page = m_WebView->page()->mainFrame()->toPlainText();
    PageParser parser(page, m_CurrDate);

    if (!parser.isPageValid())
    {
        m_WebView->triggerPageAction(QWebPage::ReloadAndBypassCache);
        return;
    }

    QList<GameModel> games = parser.parsePage();
    DBManager::inst()->add(games);
    loadNextMonth();
}

void StatsLoader::loadNextMonth()
{
    m_CurrDate = m_CurrDate.addMonths(1);

    if (m_CurrDate <= m_ToDate)
    {
        loadCurrentMonth();
    }
    else
    {
        emit loaded();
    }
}
