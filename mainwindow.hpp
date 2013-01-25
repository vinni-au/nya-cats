#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include "ui/domainwnd.h"
#include "visualize/visualizer.h"

#include "core/nkbmanager.h"
#include "mlv/mlv.h"
#include "ui/mlvcontrol.h"
#include "ui/proceditor.h"
#include "ui/kbeditorwindow.hpp"
#include <QScopedPointer>

#define MAINWINDOWTITLE "NYA CATS"



namespace Ui {
    class MainWindow;
}

/**
  @class MainWindow
  Класс главного окна с игровым полем
  */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool saveAs();

    void closeEvent(QCloseEvent *event);
protected:
    QVBoxLayout *vlayout;
     Visualizer *viz;
     QHBoxLayout *hlayout;
      QPushButton *btnStartGame;
      QPushButton *btnStartRandomGame;
      QPushButton *btnStep;
      QPushButton *btnStopGame;


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

    void on_actStartGame_triggered();

    void runMLV(int,int);

    void on_actStopGame_triggered();

    void on_actDoStep_triggered();

    void onKBDirtyChanged(bool);

    void on_actStartRandGame_triggered();

    void setEnabledStartGame(bool enabled);
    void setEnabledStartRandomGame(bool enabled);
    void setEnabledStep(bool enabled);
    void setEnabledStopGame(bool enabled);

    void on_actGlobalProcsEditor_triggered();
    void onGlobalProcAdded(NProc *proc,bool newProc);

private:
    Ui::MainWindow *ui;

    NKBManager* m_kbManager;
    MLV* m_mlv;
    MLVControl* m_mlvControl;

    QScopedPointer<KBEditorWindow> m_kbEditorWindow;
    QScopedPointer<DomainWnd>   m_domainWindow;
    QScopedPointer<ProcEditor>  m_procEditorWnd;

};

#endif // MAINWINDOW_HPP
