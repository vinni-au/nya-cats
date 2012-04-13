/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sat Apr 14 00:05:25 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actExit;
    QAction *actAbout;
    QAction *actCreate;
    QAction *actOpen;
    QAction *actClose;
    QAction *actSave;
    QAction *actSaveAs;
    QAction *actFrameEditor;
    QAction *actDomainEditor;
    QAction *actStartGame;
    QAction *actShowOutput;
    QWidget *centralWidget;
    QMenuBar *mainMenuBar;
    QMenu *menuFile;
    QMenu *menuInfo;
    QMenu *menuKB;
    QMenu *menuGame;
    QMenu *menuOptions;
    QToolBar *mainToolBar;
    QStatusBar *mainStatusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(475, 369);
        actExit = new QAction(MainWindow);
        actExit->setObjectName(QString::fromUtf8("actExit"));
        actAbout = new QAction(MainWindow);
        actAbout->setObjectName(QString::fromUtf8("actAbout"));
        actCreate = new QAction(MainWindow);
        actCreate->setObjectName(QString::fromUtf8("actCreate"));
        actOpen = new QAction(MainWindow);
        actOpen->setObjectName(QString::fromUtf8("actOpen"));
        actClose = new QAction(MainWindow);
        actClose->setObjectName(QString::fromUtf8("actClose"));
        actSave = new QAction(MainWindow);
        actSave->setObjectName(QString::fromUtf8("actSave"));
        actSaveAs = new QAction(MainWindow);
        actSaveAs->setObjectName(QString::fromUtf8("actSaveAs"));
        actFrameEditor = new QAction(MainWindow);
        actFrameEditor->setObjectName(QString::fromUtf8("actFrameEditor"));
        actDomainEditor = new QAction(MainWindow);
        actDomainEditor->setObjectName(QString::fromUtf8("actDomainEditor"));
        actStartGame = new QAction(MainWindow);
        actStartGame->setObjectName(QString::fromUtf8("actStartGame"));
        actShowOutput = new QAction(MainWindow);
        actShowOutput->setObjectName(QString::fromUtf8("actShowOutput"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        mainMenuBar = new QMenuBar(MainWindow);
        mainMenuBar->setObjectName(QString::fromUtf8("mainMenuBar"));
        mainMenuBar->setGeometry(QRect(0, 0, 475, 22));
        menuFile = new QMenu(mainMenuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuInfo = new QMenu(mainMenuBar);
        menuInfo->setObjectName(QString::fromUtf8("menuInfo"));
        menuKB = new QMenu(mainMenuBar);
        menuKB->setObjectName(QString::fromUtf8("menuKB"));
        menuKB->setEnabled(true);
        menuGame = new QMenu(mainMenuBar);
        menuGame->setObjectName(QString::fromUtf8("menuGame"));
        menuOptions = new QMenu(mainMenuBar);
        menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
        MainWindow->setMenuBar(mainMenuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        mainStatusBar = new QStatusBar(MainWindow);
        mainStatusBar->setObjectName(QString::fromUtf8("mainStatusBar"));
        MainWindow->setStatusBar(mainStatusBar);

        mainMenuBar->addAction(menuFile->menuAction());
        mainMenuBar->addAction(menuKB->menuAction());
        mainMenuBar->addAction(menuGame->menuAction());
        mainMenuBar->addAction(menuOptions->menuAction());
        mainMenuBar->addAction(menuInfo->menuAction());
        menuFile->addAction(actCreate);
        menuFile->addAction(actOpen);
        menuFile->addSeparator();
        menuFile->addAction(actClose);
        menuFile->addSeparator();
        menuFile->addAction(actSave);
        menuFile->addAction(actSaveAs);
        menuFile->addSeparator();
        menuFile->addAction(actExit);
        menuInfo->addAction(actAbout);
        menuKB->addAction(actFrameEditor);
        menuKB->addAction(actDomainEditor);
        menuGame->addAction(actStartGame);
        menuGame->addAction(actShowOutput);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "NYA CATS", 0, QApplication::UnicodeUTF8));
        actExit->setText(QApplication::translate("MainWindow", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
        actAbout->setText(QApplication::translate("MainWindow", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", 0, QApplication::UnicodeUTF8));
        actCreate->setText(QApplication::translate("MainWindow", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
        actCreate->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actOpen->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214...", 0, QApplication::UnicodeUTF8));
        actOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actClose->setText(QApplication::translate("MainWindow", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", 0, QApplication::UnicodeUTF8));
        actSave->setText(QApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        actSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actSaveAs->setText(QApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\272\320\260\320\272...", 0, QApplication::UnicodeUTF8));
        actSaveAs->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", 0, QApplication::UnicodeUTF8));
        actFrameEditor->setText(QApplication::translate("MainWindow", "\320\240\320\265\320\264\320\260\320\272\321\202\320\276\321\200 \321\204\321\200\320\265\320\271\320\274\320\276\320\262", 0, QApplication::UnicodeUTF8));
        actDomainEditor->setText(QApplication::translate("MainWindow", "\320\240\320\265\320\264\320\260\320\272\321\202\320\276\321\200 \320\264\320\276\320\274\320\265\320\275\320\276\320\262", 0, QApplication::UnicodeUTF8));
        actStartGame->setText(QApplication::translate("MainWindow", "\320\235\320\260\321\207\320\260\321\202\321\214 \320\270\320\263\321\200\321\203", 0, QApplication::UnicodeUTF8));
        actShowOutput->setText(QApplication::translate("MainWindow", "\320\237\320\276\320\272\320\260\320\267\320\260\321\202\321\214 \320\262\321\213\320\262\320\276\320\264", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
        menuInfo->setTitle(QApplication::translate("MainWindow", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260", 0, QApplication::UnicodeUTF8));
        menuKB->setTitle(QApplication::translate("MainWindow", "\320\221\320\260\320\267\320\260 \320\267\320\275\320\260\320\275\320\270\320\271", 0, QApplication::UnicodeUTF8));
        menuGame->setTitle(QApplication::translate("MainWindow", "\320\230\320\263\321\200\320\260", 0, QApplication::UnicodeUTF8));
        menuOptions->setTitle(QApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
