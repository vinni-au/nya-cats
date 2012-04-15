#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include "ui/domainwnd.h"
#include "visualize/gamefield.h"

#include "core/nkbmanager.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool saveAs();

    void closeEvent(QCloseEvent *event);
protected:
    GameField *viz;

private slots:
    void on_actExit_triggered();

    void on_actAbout_triggered();

    void on_actFrameEditor_triggered();

    void on_actCreate_triggered();

    void on_actOpen_triggered();

    void on_actClose_triggered();

    void on_actSave_triggered();

    void on_actSaveAs_triggered();

    void on_actDomainEditor_triggered();

private:
    Ui::MainWindow *ui;

    NKBManager m_kbManager;


};

#endif // MAINWINDOW_HPP
