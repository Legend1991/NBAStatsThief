#include <QWebFrame>
#include "statsloader.h"
#include "persistentcookiejar.h"

const int JULY = 7;
const int AUGUST = 8;
const int SEPTEMBER = 9;

StatsLoader::StatsLoader(QObject *parent) :
    QObject(parent),
    m_FromDate(),
    m_ToDate()
{
    PersistentCookieJar* jar = new PersistentCookieJar(this);
    m_Page = new QWebPage(this);
    m_Page->networkAccessManager()->setCookieJar(jar);
    jar->setParent(this);
    connect(m_Page, &QWebPage::loadProgress, this, &StatsLoader::loadProgress);
    connect(m_Page, &QWebPage::loadFinished, this, &StatsLoader::loadFinished);
}

StatsLoader::~StatsLoader()
{
}

void StatsLoader::load()
{
    m_CurrDate = m_FromDate;
    loadCurrentMonth();
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
    m_Page->mainFrame()->load(url);
}

void StatsLoader::loadFinished(bool ok)
{
    QString page = m_Page->mainFrame()->toPlainText();

    QStringList rows = page.split("\n");
    QStringList validRows;
    foreach (QString row, rows)
    {
        QStringList splitedRow = row.trimmed().split(QRegExp("\\s+|\\t+"));
        if (splitedRow.count() == 8 ||
                (splitedRow.count() == 3 && QRegExp(".*\\d$").exactMatch(row)))
        {
            validRows.append(row);
        }
    }

    if (validRows.isEmpty() || !ok)
    {
        qDebug() << "Load empty!" << endl << m_Page->mainFrame()->url() << endl << page;
        m_Page->triggerAction(QWebPage::ReloadAndBypassCache);
        return;
    }

    foreach (QString row, validRows)
    {
        qDebug() << row;
    }

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
