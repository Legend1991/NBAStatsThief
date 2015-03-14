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
    connect(&m_Loader, &StatsLoader::loadStarted, this, &MainWindow::loadStarted);
    connect(&m_Loader, &StatsLoader::loadProgress, this, &MainWindow::loadProgress);
    connect(&m_Loader, &StatsLoader::loaded, this, &MainWindow::loadFinished);
    connect(&m_Loader, &StatsLoader::noGames, this, &MainWindow::noGames);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::stealStats()
{
    ui->pbStartStealing->setEnabled(false);
    ui->sbAvLeagScore->setEnabled(false);
    QDate fromDate = QDate::currentDate().addMonths(-1);
    QDate toDate = QDate::currentDate();
    m_Loader.setTimeRange(fromDate, toDate);
    m_Loader.load(ui->sbAvLeagScore->value());
}

void MainWindow::loadStarted(QDate currDate)
{
    m_CurrDate = currDate;
    QString status = QString("Load stats for %1 (0%)").arg(getDateString(m_CurrDate, "MMMM yyyy"));
    ui->statusBar->showMessage(status);
}

void MainWindow::loadProgress(int progress)
{
    QString status = QString("Load stats for %1 (%2%)").arg(getDateString(m_CurrDate, "MMMM yyyy")).arg(progress);
    ui->statusBar->showMessage(status);
}

void MainWindow::loadFinished()
{
    ui->pbStartStealing->setEnabled(true);
    ui->sbAvLeagScore->setEnabled(true);
    ui->statusBar->showMessage("Stats load finished!");
}

void MainWindow::noGames()
{
    ui->pbStartStealing->setEnabled(true);
    ui->sbAvLeagScore->setEnabled(true);
    ui->statusBar->showMessage("No games tomorrow!");
}

QString MainWindow::getDateString(QDate &date, QString format)
{
    QLocale locale(QLocale::English, QLocale::UnitedStates);
    return locale.toString(date, format);
}
