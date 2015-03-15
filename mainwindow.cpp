#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>

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
    QString fileName = QDate::currentDate().addDays(1).toString("nba-forecast-yyyy-MM-dd.txt");
    QString location = QStandardPaths::locate(QStandardPaths::DesktopLocation, NULL, QStandardPaths::LocateDirectory);
    ui->leOutputDB->setText(location + fileName);
    connect(ui->pbBrowse, &QPushButton::clicked, this, &MainWindow::browse);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::stealStats()
{
    ui->pbStartStealing->setEnabled(false);
    ui->sbAvLeagScore->setEnabled(false);
    ui->leOutputDB->setEnabled(false);
    ui->pbBrowse->setEnabled(false);
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

void MainWindow::loadFinished(QList<GameModel> games)
{
    setTable(games);
    writeForecast(games);
    ui->pbStartStealing->setEnabled(true);
    ui->sbAvLeagScore->setEnabled(true);
    ui->leOutputDB->setEnabled(true);
    ui->pbBrowse->setEnabled(true);
    ui->statusBar->showMessage("Stats load finished!");
}

void MainWindow::setTable(QList<GameModel> games)
{
    ui->twCalculation->setRowCount(games.count());

    int row = 0;
    foreach (GameModel game, games) {
        QTableWidgetItem *homeTeam = new QTableWidgetItem(game.getHomeTeam());
        homeTeam->setTextAlignment(Qt::AlignCenter);
        ui->twCalculation->setItem(row, 0, homeTeam);
        QTableWidgetItem *homeScore = new QTableWidgetItem(QString("%1").arg(game.getHomeScore()));
        homeScore->setTextAlignment(Qt::AlignLeft);
        ui->twCalculation->setItem(row, 1, homeScore);
        QTableWidgetItem *visitorScore = new QTableWidgetItem(QString("%1").arg(game.getVisitorScore()));
        visitorScore->setTextAlignment(Qt::AlignRight);
        ui->twCalculation->setItem(row, 2, visitorScore);
        QTableWidgetItem *visitorTeam = new QTableWidgetItem(game.getVisitorTeam());
        visitorTeam->setTextAlignment(Qt::AlignCenter);
        ui->twCalculation->setItem(row, 3, visitorTeam);
        row++;
    }
}

void MainWindow::noGames()
{
    ui->pbStartStealing->setEnabled(true);
    ui->sbAvLeagScore->setEnabled(true);
    ui->leOutputDB->setEnabled(true);
    ui->pbBrowse->setEnabled(true);
    ui->statusBar->showMessage("No games tomorrow!");
}

QString MainWindow::getDateString(QDate &date, QString format)
{
    QLocale locale(QLocale::English, QLocale::UnitedStates);
    return locale.toString(date, format);
}

void MainWindow::browse()
{
    QString fileLocation = QFileDialog::getSaveFileName(this, "Save Stats To File", ui->leOutputDB->text(), "Text files (*.txt)");
    ui->leOutputDB->setText(fileLocation);
}

void MainWindow::writeForecast(QList<GameModel> games)
{
    QFileInfo fileInfo(ui->leOutputDB->text());
    QDir dir(fileInfo.absoluteDir());

    if (!dir.exists())
    {
        browse();
    }

    QString fileName = fileInfo.absoluteFilePath();
    if(fileInfo.suffix() != "txt")
    {
        fileName += ".txt";
    }

    QFile file(fileName);
    if (file.open(QFile::ReadWrite | QFile::Truncate | QFile::Text))
    {
        foreach (GameModel game, games)
        {
            QString row = QString("%1\t%2\t%3\t%4\n").arg(game.getHomeTeam()).arg(game.getHomeScore())
                    .arg(game.getVisitorScore()).arg(game.getVisitorTeam());
            file.write(row.toLatin1());
        }
        file.close();
    }
}
