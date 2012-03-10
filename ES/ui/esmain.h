#ifndef ESMAIN_H
#define ESMAIN_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QMdiSubWindow>
#include <QCloseEvent>
#include <QWindowStateChangeEvent>
#include <QStatusBar>


#include "domainwnd.h"
#include "varswnd.h"
#include "ruleswnd.h"
#include "resultswnd.h"

#include "questionwnd.h"//???
#include "startconswnd.h"
#include "saver.h"

namespace Ui {
    class ESmain;
}

class RulesWnd;

class ESmain : public QMainWindow
{
    Q_OBJECT

public:
    explicit ESmain(QWidget *parent = 0);
    ~ESmain();
    bool maybeSave();
    void retranslate();
    void open(QString str);
private:
    void createMenu();
    bool saveAs();
    void OpenEditSubwindows();
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *target, QEvent *event);

    QMdiSubWindow* subwindowByName(QString wndName);
signals:
public slots:
    //file menu
    void onMenuCreateNew();
    void onMenuOpen();
    void onMenuSave();
    void onMenuSaveAs();
    //
    void onOpenDomainWnd();
    void onOpenVarWnd();
    void onOpenRuleWnd();
    //
    void onStartCons();
    //
    void onAbout();

    void onSubWindowStateChanged(Qt::WindowStates,Qt::WindowStates);
    void onSubWindowActivated(QMdiSubWindow* sbw);

    void onRulesWndActivateByTimer();
    void onVarsWndActivateByTimer();
    void onDomainsWndActivateByTimer();
    void onRulesWndDestroyed();
    void onVarsWndDestroyed();
    void onDomainsWndDestroyed();
private:
    DomainWnd   *domainWnd;
    VarsWnd     *varsWnd;
    RulesWnd    *rulesWnd;

    bool dwps;//предыдущие состояния
    bool vwps;
    bool rwps;
    QMdiSubWindow   *lastActiveSubw;

    QMdiArea *mdiArea;

    QMenuBar    *menuBar;
     QMenu       *menuFile;
      QAction       *actCreate;
      QAction       *actOpen;
      QAction       *actSave;
      QAction       *actSaveAs;
      QAction       *actExit;
     QMenu       *menuWindow;
      QAction       *actCascade;
      QAction       *actTile;
     QMenu       *menuES;
      QAction       *actDomains;
      QAction       *actVars;
      QAction       *actRules;
      QAction       *actResult;
     QMenu       *menuConsultation;
      QAction       *actStartCons;
     QMenu       *menuHelp;
      QAction       *actAbout;

};

#endif // ESMAIN_H
