#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_Loader(),
    m_CurrDate()
{
    ui->setupUi(this);
    connect(ui->pbStartStealing, &QPushButton::clicked, this, &MainWindow::stealStats);
    setTimeRangeContent();
    connect(&m_Loader, &StatsLoader::loadStarted, this, &MainWindow::loadStarted);
    connect(&m_Loader, &StatsLoader::loadProgress, this, &MainWindow::loadProgress);
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

void MainWindow::stealStats()
{
    QDate fromDate = QDate(ui->cbFromYear->currentData().toInt(), ui->cbFromMonth->currentData().toInt(), 1);
    QDate toDate = QDate(ui->cbToYear->currentData().toInt(), ui->cbToMonth->currentData().toInt(), 1);
    m_Loader.setTimeRange(fromDate, toDate);
    m_Loader.load();
}

void MainWindow::loadStarted(QDate currDate)
{
    m_CurrDate = currDate;
    QString status = QString("Load stats for %1 0%").arg(m_CurrDate.toString("MMMM"));
    ui->statusBar->showMessage(status);
}

void MainWindow::loadProgress(int progress)
{
    QString status = QString("Load stats for %1 %2%").arg(m_CurrDate.toString("MMMM")).arg(progress);
    ui->statusBar->showMessage(status);
}
