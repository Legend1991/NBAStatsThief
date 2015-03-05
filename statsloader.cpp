#include <QWebFrame>
#include "statsloader.h"

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
    m_FromDate = fromDate > currentDate ? currentDate : fromDate;
    m_ToDate = toDate > currentDate ? currentDate : toDate;
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
//    qDebug() << "\n\n>>>>>>>>>>>" <<m_CurrDate.toString("MMMM yyyy") << "\n\n";
    QString month = QString("%1").arg(m_CurrDate.month(), 2, 10, QChar('0'));
    QUrl url = QUrl(QString("http://mi.nba.com/schedule/#!/%1/%2").arg(month).arg(m_CurrDate.year()));
//    m_WebView->setUrl(url);
//    m_WebView->pageAction(QWebPage::ReloadAndBypassCache);
    m_WebView->setUrl(url);
    m_WebView->load(url);
}

void StatsLoader::loadFinished(bool ok)
{
    QString page = m_WebView->page()->mainFrame()->toPlainText();

//    qDebug() << m_WebView->url() << endl << page;

    if (!ok || !isValidPage(page))
    {
//        qDebug() << "Load empty!" << endl << m_WebView->url() << endl << page;
//        m_WebView->pageAction(QWebPage::ReloadAndBypassCache);
        m_WebView->load(m_WebView->url());
        return;
    }

    parsePage(page);

    loadNextMonth();
}

void StatsLoader::loadNextMonth()
{
    m_CurrDate = m_CurrDate.addMonths(1);

    if (m_CurrDate <= m_ToDate)
    {
        loadCurrentMonth();
    }
}

bool StatsLoader::isDateRow(QString &row)
{
    QStringList splitedRow = tokenizeRow(row);
    return splitedRow.count() == 3 && QRegExp(".*\\d$").exactMatch(row);
}

bool StatsLoader::isGameRow(QString &row)
{
    QStringList splitedRow = tokenizeRow(row);
    return splitedRow.count() == 8;
}

bool StatsLoader::isValidPage(QString &page)
{
    if (page.isEmpty())
    {
        return false;
    }

    QStringList rows = page.split("\n");

    foreach (QString row, rows)
    {
        if (isGameRow(row) || isDateRow(row))
        {
            return true;
        }
    }

    return false;
}

QStringList StatsLoader::tokenizeRow(QString &row)
{
    return row.trimmed().split(QRegExp("\\s+|\\t+"));
}

QDate StatsLoader::parseDate(QString &row)
{
    QStringList dateTokens = tokenizeRow(row);
    QLocale locale(QLocale::English, QLocale::UnitedStates);
    QString strDate = QString("%1%2%3").arg(m_CurrDate.year()).arg(dateTokens.at(1)).arg(dateTokens.at(2));
    return locale.toDate(strDate, "yyyyMMMdd");
}

void StatsLoader::parsePage(QString &page)
{
    QStringList rows = page.split("\n");
    QStringList validRows;

    foreach (QString row, rows)
    {
        if (isGameRow(row) || isDateRow(row))
        {
            validRows.append(row);
        }
    }

    foreach (QString row, validRows)
    {
        if (isDateRow(row))
        {
            QDate date = parseDate(row);
            qDebug() << date;
        }
        qDebug() << row;
    }
}
