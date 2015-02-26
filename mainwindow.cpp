#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "statsloader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pbStartStealing, &QPushButton::clicked, this, &MainWindow::stealStats);
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

void MainWindow::stealStats()
{
    QDate fromDate = QDate(ui->cbFromYear->currentData().toInt(), ui->cbFromMonth->currentData().toInt(), 1);
    QDate toDate = QDate(ui->cbToYear->currentData().toInt(), ui->cbToMonth->currentData().toInt(), 1);
    StatsLoader loader(fromDate, toDate);
    loader.load();
}
