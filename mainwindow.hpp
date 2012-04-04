#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include "ui/domainwnd.h"

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
    void on_actExit_triggered();

    void on_actAbout_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
