#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbmanager.h"

const int JANUARY = 1;
const int DECEMBER = 12;
const int START_YEAR = 2012;

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
    connect(&m_Loader, &StatsLoader::loaded, this, &MainWindow::loadFinished);

    connect(ui->cbToMonth, SIGNAL(currentIndexChanged(int)), this, SLOT(checkTimeRange()));
    connect(ui->cbToYear, SIGNAL(currentIndexChanged(int)), this, SLOT(checkTimeRange()));
    connect(ui->cbFromMonth, SIGNAL(currentIndexChanged(int)), this, SLOT(checkTimeRange()));
    connect(ui->cbFromYear, SIGNAL(currentIndexChanged(int)), this, SLOT(checkTimeRange()));

    QString fileName = QDate::currentDate().addDays(1).toString("nbastats.sqlite");
    QString location = QStandardPaths::locate(QStandardPaths::DesktopLocation, NULL, QStandardPaths::LocateDirectory);
//    ui->leOutputDB->setText(location + fileName);
    ui->leOutputDB->setText("D:/" + fileName);
    connect(ui->pbBrowse, &QPushButton::clicked, this, &MainWindow::browse);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTimeRangeContent()
{
    QDate currentDate = QDate::currentDate();

    for (int i = START_YEAR; i <= currentDate.year(); ++i)
    {
        ui->cbFromYear->addItem(QString::number(i), i);
        ui->cbToYear->addItem(QString::number(i), i);
    }
    ui->cbFromYear->setCurrentIndex(currentDate.year() - START_YEAR);
    ui->cbToYear->setCurrentIndex(currentDate.year() - START_YEAR);

    for (int i = JANUARY; i <= DECEMBER; ++i)
    {
        QDate date(START_YEAR, i, 1);
        QString month = getDateString(date, "MMMM");
        ui->cbFromMonth->addItem(month, i);
        ui->cbToMonth->addItem(month, i);
    }
    ui->cbFromMonth->setCurrentIndex(9);
    ui->cbToMonth->setCurrentIndex(currentDate.month() - 1);
}

void MainWindow::checkTimeRange()
{
    QDate currentDate = QDate::currentDate();
    QDate fromDate = QDate(ui->cbFromYear->currentData().toInt(), ui->cbFromMonth->currentData().toInt(), 1);
    QDate toDate = QDate(ui->cbToYear->currentData().toInt(), ui->cbToMonth->currentData().toInt(), 1);

    if (toDate > currentDate)
    {
        toDate = currentDate;
        ui->cbToYear->setCurrentIndex(toDate.year() - START_YEAR);
        ui->cbToMonth->setCurrentIndex(toDate.month() - 1);
    }

    if (fromDate > toDate)
    {
        fromDate = toDate;
        ui->cbFromYear->setCurrentIndex(fromDate.year() - START_YEAR);
        ui->cbFromMonth->setCurrentIndex(fromDate.month() - 1);
    }
}

void MainWindow::stealStats()
{
    ui->pbStartStealing->setEnabled(false);
    DBManager::inst()->createDB(ui->leOutputDB->text());
    QDate fromDate = QDate(ui->cbFromYear->currentData().toInt(), ui->cbFromMonth->currentData().toInt(), 2);
    QDate toDate = QDate(ui->cbToYear->currentData().toInt(), ui->cbToMonth->currentData().toInt(), 12);
    m_Loader.setTimeRange(fromDate, toDate);
    m_Loader.load();
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
    ui->statusBar->showMessage("Stats load finished!");
}

void MainWindow::browse()
{
    QString fileLocation = QFileDialog::getSaveFileName(this, "Save Stats To File", ui->leOutputDB->text(), "SQLite DB files (*.sqlite)");

    QFileInfo fileInfo(fileLocation);
    QDir dir(fileInfo.absoluteDir());

    if (!dir.exists())
    {
        browse();
    }

    QString fileName = fileInfo.absoluteFilePath();
    if(fileInfo.suffix() != "sqlite")
    {
        fileName += ".sqlite";
    }

    ui->leOutputDB->setText(fileName);
}

QString MainWindow::getDateString(QDate &date, QString format)
{
    QLocale locale(QLocale::English, QLocale::UnitedStates);
    return locale.toString(date, format);
}
