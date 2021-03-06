#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include "statsloader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void checkTimeRange();

private:
    void stealStats();
    void setTimeRangeContent();
    void loadStarted(QDate currDate);
    void loadProgress(int progress);
    void loadFinished();
    void browse();
    QString getDateString(QDate &date, QString format);

    Ui::MainWindow *ui;
    StatsLoader m_Loader;
    QDate m_CurrDate;
};

#endif // MAINWINDOW_H
