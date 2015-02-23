#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebPage>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void loadStats();
    void loadFinished(bool ok);
    void setTimeRangeContent();

    Ui::MainWindow *ui;
    QWebPage *m_WP;
};

#endif // MAINWINDOW_H
