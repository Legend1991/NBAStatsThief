#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebFrame>
#include <QWebElement>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_WP = new QWebPage(this);
//    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::loadStats);
//    connect(m_WP, &QWebPage::loadProgress, ui->progressBar, &QProgressBar::setValue);
//    connect(m_WP, &QWebPage::loadFinished, this, &MainWindow::loadFinished);
    setTimeRangeContent();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTimeRangeContent()
{
    for (int i = 2010; i <= QDate::currentDate().year(); ++i)
    {
        ui->cbFromYear->addItem(QString::number(i), i);
        ui->cbToYear->addItem(QString::number(i), i);
    }
    ui->cbToYear->setCurrentIndex(ui->cbToYear->count() - 1);

    for (int i = 1; i <= 12; ++i)
    {
        ui->cbFromMonth->addItem(QDate::longMonthName(i), i);
        ui->cbToMonth->addItem(QDate::longMonthName(i), i);
    }
    ui->cbFromMonth->setCurrentIndex(9);
    ui->cbToMonth->setCurrentIndex(5);
}

void MainWindow::loadStats()
{
//    QUrl url("http://mi.nba.com/schedule/#!/02/2015");
//    QWebFrame *wf = m_WP->mainFrame();
//    QtConcurrent::run(wf, &QWebFrame::load, url);
    m_WP->mainFrame()->load(QUrl("http://mi.nba.com/schedule/#!/02/2015"));
}

void MainWindow::loadFinished(bool ok)
{
    QString page = m_WP->mainFrame()->toPlainText();

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
        m_WP->triggerAction(QWebPage::ReloadAndBypassCache);
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
