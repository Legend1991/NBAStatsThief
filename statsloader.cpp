#include <QWebFrame>
#include "statsloader.h"

const int JULY = 7;
const int AUGUST = 8;
const int SEPTEMBER = 9;

StatsLoader::StatsLoader(QDate fromDate, QDate toDate, QObject *parent) :
    QObject(parent),
    m_FromDate(fromDate),
    m_ToDate(toDate),
    m_Page()
{
//    connect(&m_Page, &QWebPage::loadProgress, ui->progressBar, &QProgressBar::setValue);
    connect(&m_Page, &QWebPage::loadFinished, this, &StatsLoader::loadFinished);
}

StatsLoader::~StatsLoader()
{
}

void StatsLoader::load()
{
    for (QDate date = m_FromDate; date <= m_ToDate; date = date.addMonths(1))
    {
        if (date.month() == JULY || date.month() == AUGUST || date.month() == SEPTEMBER) continue;

        QString month = QString("%1").arg(date.month(), 2, 10, QChar('0'));
        QUrl url = QUrl(QString("http://mi.nba.com/schedule/#!/%1/%2").arg(month).arg(date.year()));
        m_Page.mainFrame()->load(url);
    }
}

void StatsLoader::loadFinished(bool ok)
{
    QString page = m_Page.mainFrame()->toPlainText();

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

    if (validRows.isEmpty()) {
        m_Page.triggerAction(QWebPage::ReloadAndBypassCache);
        return;
    }

    foreach (QString row, validRows)
    {
        qDebug() << row;
    }

    if (!ok)
    {
        qDebug() << "Something wrong!";
    }
}
