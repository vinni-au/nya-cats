#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    viz = new GameField(this);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(viz);
    ui->centralWidget->setLayout(layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actExit_triggered()
{
    this->close();
}

void MainWindow::on_actAbout_triggered()
{
    DomainWnd wnd(new DomainModel());
    wnd.show();
}
