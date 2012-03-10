#include "esmain.h"



ESmain::ESmain(QWidget *parent) :
    QMainWindow(parent)
{
    this->setWindowIcon(QIcon(":icons/icon.ico"));
    this->setWindowTitle("Оболочка продукционной ЭС");

    mdiArea=new QMdiArea(this);
    this->setCentralWidget(mdiArea);
    mdiArea->setDocumentMode(true);

    createMenu();
   // OpenEditSubwindows();
    //onMenuCreateNew();

    mdiArea->cascadeSubWindows();
    mdiArea->setTabShape(QTabWidget::Rounded);
    mdiArea->setViewMode(QMdiArea::TabbedView);

    domainWnd = NULL;
    varsWnd   = NULL;
    rulesWnd  = NULL;

    lastActiveSubw = NULL;
    connect(mdiArea,SIGNAL(subWindowActivated(QMdiSubWindow*)),SLOT(onSubWindowActivated(QMdiSubWindow*)));
    //mdiArea->installEventFilter(this);

    retranslate();

    this->statusBar()->showFullScreen();
    this->statusBar()->showMessage("Готов!");


    connect(DataModels::instance(),SIGNAL(sigErrorWhileValidating(QString)),statusBar(),SLOT(showMessage(QString)));

}

ESmain::~ESmain()
{

}

void
ESmain::retranslate()
{

}

void
ESmain::createMenu()
{
    this->menuBar = new QMenuBar();
    this->setMenuBar(menuBar);

    menuFile = new QMenu("&Файл");
    menuBar->addMenu(menuFile);
    {
        actCreate = menuFile->addAction("&Новая ЭС");
            actCreate->setShortcut(QKeySequence("Ctrl+N"));
        actOpen = menuFile->addAction("&Открыть ЭС...");
            actOpen->setShortcut(QKeySequence("Ctrl+O"));
        actSave = menuFile->addAction("&Сохранить ЭС");
            actSave->setShortcut(QKeySequence("Ctrl+S"));
        actSaveAs = menuFile->addAction("&Сохранить как...");
            actSaveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));

        menuFile->addSeparator();
        actExit = menuFile->addAction("&Выход");
            actExit->setShortcut(QKeySequence("Ctrl+Q"));
    }

    menuWindow = new QMenu("&Окна");
    menuBar->addMenu(menuWindow);
    {
        actCascade = menuWindow->addAction("&Каскадом");
            actCascade->setShortcut(QKeySequence("Ctrl+`"));
        actTile = menuWindow->addAction("&Заполнить");
            actTile->setShortcut(QKeySequence("Ctrl+`+t"));
    }

    menuES = new QMenu("&Экспертная система");
    menuBar->addMenu(menuES);
    {
        actDomains = menuES->addAction("&Домены...");
            actDomains->setShortcut(QKeySequence("Ctrl+1"));
        actVars = menuES->addAction("&Переменные...");
            actVars->setShortcut(QKeySequence("Ctrl+2"));
        actRules = menuES->addAction("П&равила...");
            actRules->setShortcut(QKeySequence("Ctrl+3"));
        //actResult = menuES->addAction("&Объяснение...");
        //    actResult->setShortcut(QKeySequence("Ctrl+4"));
    }

    menuConsultation = new QMenu("&Консультация");
    menuBar->addMenu(menuConsultation);
    {
        actStartCons = menuConsultation->addAction("&Начать консультацию...");
            actStartCons->setShortcut(QKeySequence("Ctrl+B"));
    }

    menuHelp = new QMenu("&Справка");
    menuBar->addMenu(menuHelp);
    {
        actAbout = menuHelp->addAction("О программе...");
            actAbout->setShortcut(QKeySequence("Ctrl+F1"));
    }


    connect(actCreate,SIGNAL(triggered()),SLOT(onMenuCreateNew()));
    connect(actOpen,SIGNAL(triggered()),SLOT(onMenuOpen()));
    connect(actSave,SIGNAL(triggered()),SLOT(onMenuSave()));
    connect(actSaveAs,SIGNAL(triggered()),SLOT(onMenuSaveAs()));
    QObject::connect(actExit,SIGNAL(triggered()),this,SLOT(close()));

    QObject::connect(actCascade,SIGNAL(triggered()),mdiArea,SLOT(cascadeSubWindows()));
    QObject::connect(actTile,SIGNAL(triggered()),mdiArea,SLOT(tileSubWindows()));

    connect(actDomains,SIGNAL(triggered()),SLOT(onOpenDomainWnd()));
    connect(actVars,SIGNAL(triggered()),SLOT(onOpenVarWnd()));
    connect(actRules,SIGNAL(triggered()),SLOT(onOpenRuleWnd()));

    connect(actStartCons,SIGNAL(triggered()),SLOT(onStartCons()));

    connect(actAbout,SIGNAL(triggered()),SLOT(onAbout()));

}
bool
ESmain::maybeSave()//возвращает true если OK или Отмена
{
    qDebug()<<"ESmain::maybeSave()";

    return Saver::mayBeSave();
    /////////////////////

//    if(!DataModels::instance()->saved())
//    {
//        qDebug()<<"ЭС не сохранена";
//        QMessageBox mb(this);
//        mb.setText(tr("Сохранить ЭС?"));
//        mb.setModal(true);
//        mb.addButton(QMessageBox::Save);
//        mb.addButton(QMessageBox::No);
//        mb.addButton(QMessageBox::Cancel);
//        switch( mb.exec() )
//        {
//        case QMessageBox::Save:
//            qDebug()<<"Сохранить";
//            if(DataModels::instance()->isValid())
//            {
//                if(DataModels::instance()->file!=NULL)
//                {
//                    qDebug()<<"Просто сохранить";
//                    DataModels::instance()->save();
//                }
//                else
//                {
//                    qDebug()<<"Сохранить как..";
//                    if(!saveAs())
//                        return false;
//                }
//            }
//            else
//            {
//                QMessageBox mbNotValid;
//                mbNotValid.setText("В данных есть ошибка. Сохранение отменено.");
//                mbNotValid.exec();
//                qDebug()<<"Отмена";
//                return false;
//            }
//            break;
//        case QMessageBox::No:
//            qDebug()<<"Не сохранять";
//            break;
//        case QMessageBox::Cancel:
//            qDebug()<<"Отмена";
//            return false;
//            break;
//        }
//    }
//    return true;
}

void
ESmain::onMenuCreateNew()
{
    qDebug()<<"ESmain::onMenuCreateNew";
    if(maybeSave())
    {
        DataModels::instance()->Clear();
        mdiArea->closeAllSubWindows();
        OpenEditSubwindows();
    }
}

void
ESmain::onMenuOpen()
{
    try
    {

        qDebug()<<"ESmain::onMenuOpen()";

        if(maybeSave())
        {
            QString esFile = QFileDialog::getOpenFileName(this,"Открыть ЭС..",QDir::currentPath(),tr("Expert system (*.esxml)"),0);
            if( !esFile.isEmpty())
            {
                if(!DataModels::instance()->Open(esFile))
                {
                    QMessageBox msgBox;
                    msgBox.setText(tr("Не удалось открыть файл."));
                    msgBox.exec();
                }
                else
                {
                    mdiArea->closeAllSubWindows();
                    OpenEditSubwindows();
                    DataModels::instance()->dirty = false;//иначе все дело портит вопрос
                    statusBar()->showMessage(QString("Открыт файл: %1").arg(esFile),2000);

                }
            }
        }
    }
    catch(...)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Ошибка при чтении!"));
        msgBox.exec();
    }
}

void
ESmain::onMenuSave()
{
    qDebug()<<"ESmain::onMenuSave()";
    if(DataModels::instance()->file==NULL)
    {
        onMenuSaveAs();
    }
    else
    {
        if(DataModels::instance()->isValid())
        {
            DataModels::instance()->save();
        }
        else
        {
            QMessageBox::warning(this,"Ошибка!","Данные невалидны. Не сохранено.");
        }
    }
}

void
ESmain::onMenuSaveAs()
{
    qDebug()<<"ESmain::onMenuSaveAs";

    saveAs();

//    QString esFile=QFileDialog::getSaveFileName(this,tr("Сохранить ЭС как.."),QDir::currentPath(),tr("Expert system (*.esxml)"),0);
//    if(!esFile.isEmpty())
//    {
//        qDebug()<<esFile;
//        if(!DataModels::instance()->saveAs(esFile))
//        {
//            //если не сохранилось
//            QMessageBox msgBox;
//            msgBox.setText(tr("Ошибка при сохранении. Выберите другой путь."));
//            msgBox.exec();
//        }
//    }
}


bool
ESmain::saveAs()
{
    return Saver::SaveAs();
    ////////////////////////////////////////////////////
    if(!DataModels::instance()->isValid())
    {
        QMessageBox mb;
        mb.setText("В данных ошибка. Не сохранено.");
        mb.exec();
        return false;
    }
    QString esFile=QFileDialog::getSaveFileName(this,tr("Сохранить ЭС как.."),QDir::currentPath(),tr("Expert system (*.esxml)"),0);
    if(!esFile.isEmpty())
    {
        qDebug()<<esFile;
        if(!DataModels::instance()->saveAs(esFile.append(".esxml")))
        {
            //если не сохранилось
            return false;
        }
        return true;
    }
    else
        return false;
}


void
ESmain::OpenEditSubwindows()
{
   onOpenDomainWnd();
   onOpenVarWnd();
   onOpenRuleWnd();
}

void
ESmain::onOpenDomainWnd()
{
    QList<QMdiSubWindow*> subWnds = mdiArea->subWindowList();

    bool found = false;
    QMdiSubWindow* subWnd;
    foreach(subWnd,subWnds)
        if(subWnd->windowTitle() == tr("Домены"))
        {
            found = true;
            break;
        }
    if(found)
    {
        mdiArea->setActiveSubWindow(subWnd);
    }
    else
    {
        domainWnd=new DomainWnd();
        domainWnd->setWindowIcon(QIcon(":icons/icon.ico"));
        QMdiSubWindow* subW = mdiArea->addSubWindow(domainWnd);
        subW->setVisible(true);
        mdiArea->setActiveSubWindow(subW);
        connect(domainWnd,SIGNAL(sigClosed()),SLOT(onDomainsWndDestroyed()));
    }
}
void
ESmain::onOpenVarWnd()
{   
    QList<QMdiSubWindow*> subWnds = mdiArea->subWindowList();
    bool found = false;
    QMdiSubWindow* subWnd;
    foreach(subWnd,subWnds)
        if(subWnd->windowTitle() == tr("Переменные"))
        {
            found = true;
            break;
        }

    if(found)
    {
        mdiArea->setActiveSubWindow(subWnd);
    }
    else
    {
        varsWnd=new VarsWnd();
        varsWnd->setWindowIcon(QIcon(":icons/icon.ico"));
        QMdiSubWindow* subW = mdiArea->addSubWindow(varsWnd);
        subW->setVisible(true);
        mdiArea->setActiveSubWindow(subW);
        connect(varsWnd,SIGNAL(sigClosed()),SLOT(onVarsWndDestroyed()));
    }
}
void
ESmain::onOpenRuleWnd()
{
    QList<QMdiSubWindow*> subWnds = mdiArea->subWindowList();
    bool found = false;
    QMdiSubWindow* subWnd;

    foreach(subWnd,subWnds)
        if(subWnd->windowTitle() == tr("Правила"))
        {
            found = true;
            break;
        }
    if(found)
    {
        mdiArea->setActiveSubWindow(subWnd);
    }
    else
    {
        rulesWnd=new RulesWnd(this);
        rulesWnd->setWindowIcon(QIcon(":icons/icon.ico"));
        QMdiSubWindow* subW = mdiArea->addSubWindow(rulesWnd);
        subW->setVisible(true);
        mdiArea->setActiveSubWindow(subW);
        //connect(subW,SIGNAL(windowStateChanged(Qt::WindowStates,Qt::WindowStates)),SLOT(onSubWindowStateChanged(Qt::WindowStates,Qt::WindowStates)));
        //rulesWnd->installEventFilter(this);
        connect(rulesWnd,SIGNAL(sigClosed()),SLOT(onRulesWndDestroyed()));
    }

}

void
ESmain::onAbout()
{
//    QMdiSubWindow   *rulesWndSbw = NULL;
//    QList<QMdiSubWindow*> sbwList = mdiArea->subWindowList();
//    QMdiSubWindow *subWindow;
//    foreach(subWindow,sbwList)
//    {
//        if(subWindow->windowTitle() == tr("Правила"))
//        {
//            rulesWndSbw = subWindow;
//            //mdiArea->setActiveSubWindow(rulesWndSbw);
//            //connect(rulesWnd,SIGNAL(sigClosed()),SLOT(onRulesWndDestroyed()));
//        }
//    }<img src=':icons/icon.ico'></img>

    QMessageBox::about(this,"О программе...","<h1>Оболочка продукционной ЭС</h1>\n"
                       "<p>Автор: Бобров А.А.</p>  \n"
                       "<p>Копирайт &copy; 2011 ДаЁжСофт Software Inc.</p>");

}

void
ESmain::closeEvent(QCloseEvent *event)
{
    qDebug()<<"ESmain::closeEvent";
    if(maybeSave())
    {
        event->accept();
    }
    else
        event->ignore();
}
void
ESmain::onStartCons()
{
    qDebug()<<"ESmain::onStartCons";
    if(maybeSave())
    {
        if(!DataModels::instance()->saved())
            DataModels::instance()->reload();

        if(!DataModels::instance()->isValid())
        {
            QMessageBox::warning(this,"Ошибка","Ошибки в ЭС.");
            return;
        }

        ResultsWnd *rwnd = new ResultsWnd();
        StartConsWnd scw;
        connect(&scw,SIGNAL(sigVarUse(QString,QString)),rwnd,SLOT(slotAddVar(QString,QString)));
        connect(&scw,SIGNAL(sigRuleUse(QString,int)),rwnd,SLOT(slotAddRule(QString,int)));

        mdiArea->addSubWindow(rwnd)->setVisible(true);
        if(scw.exec())
        {
            if(!scw.consResult.isEmpty())
            {//есть результат
                QMessageBox msgBox;
                msgBox.setText("Результат: "+scw.consResult);
                msgBox.exec();

                //TODO открыть вкладки с объяснением вывода
            }
            else
            {//нет результата
                QMessageBox msgBox;
                msgBox.setText(tr("Данная ЭС ничем не может вам помочь."));
                msgBox.exec();
            }
        }
    }
}

void
ESmain::onSubWindowStateChanged(Qt::WindowStates s1, Qt::WindowStates s2)
{
//    //qDebug()<<"---"<<s1<<"--->"<<s2;
//    //qDebug()<<rulesWnd->windowState();

//    if(!rulesWnd->windowState().testFlag(Qt::WindowActive))//если неактивно
//    {
//        //qDebug()<<"Деактивация вкладки правил";
//    }
//    if(!(s2.testFlag(Qt::WindowActive)))
//    {

//        //if(maybeSave())
//        {

//        }
//    }
}

void
ESmain::onSubWindowActivated(QMdiSubWindow *sbw)
{
    qDebug()<<"ESmain::onSubWindowActivated";
    //находим окна, которые находятся в режиме редактирования и закрываем этот режим
//    QPointer<RulesWnd*> p;
//    p = rulesWnd;
//    bool boo = p.isNull();


    bool rulesWndIsOpened = false;
    bool varsWndIsOpened = false;
    bool domainWndIsopened = false;

    QMdiSubWindow   *rulesWndSbw = NULL;
    QMdiSubWindow   *varsWndSbw = NULL;
    QMdiSubWindow   *domainsWndSbw = NULL;

    QList<QMdiSubWindow*> sbwList = mdiArea->subWindowList();
    QMdiSubWindow *subWindow;
    foreach(subWindow,sbwList)//ищем нужные окна в mdiArea
    {
        if(subWindow->windowTitle() == tr("Правила"))
        {
            rulesWndIsOpened = true;
            rulesWndSbw = subWindow;
        }
        if(subWindow->windowTitle() == tr("Переменные"))
        {
            varsWndIsOpened = true;
            varsWndSbw = subWindow;
        }
        if(subWindow->windowTitle() == tr("Домены"))
        {
            domainWndIsopened = true;
            domainsWndSbw = subWindow;
        }
    }


    qDebug()<<"Окно правил открыто: "<<rulesWndIsOpened;
    if(rulesWndIsOpened)
    {
        if(rulesWnd->isEditMode()  )
        {
            qDebug()<<"Окно правил в режиме редактирования";
            if(!(mdiArea->activeSubWindow() == rulesWndSbw) && !(mdiArea->activeSubWindow()==NULL))//деактивация формы была вызвана не всплывающей формой, а переключением
            {
                qDebug()<<"Это не был мессаджбокс"<<(int)mdiArea->activeSubWindow() <<" "<< (int)rulesWndSbw;
                mdiArea->blockSignals(true);
                 rulesWnd->maybeClose();
                mdiArea->blockSignals(false);
                if(rulesWnd->isEditMode())//все еще режим редактирования
                {
                    QTimer::singleShot(0, this, SLOT(onRulesWndActivateByTimer()));//шаманство
                }
            }
            else
            {
                qDebug()<<"После деактивации вкладка осталась активной в mdi|"<<(int)mdiArea->activeSubWindow() <<" "<< (int)rulesWndSbw;
            }
        }
    }
    else
    {
        //значит закрыли окно
        qDebug()<<"RulesWindow has been closed";
    }
    //------------------------------------------------------------
    qDebug()<<"Окно переменных открыто: "<<varsWndIsOpened;
    if(varsWndIsOpened)
    {
        if(varsWnd->isEditMode()  )
        {
            qDebug()<<"Окно переменных в режиме редактирования";
            if(!(mdiArea->activeSubWindow() == varsWndSbw) && !(mdiArea->activeSubWindow()==NULL))//деактивация формы была вызвана не всплывающей формой, а переключением
            {
                qDebug()<<"Это не был мессаджбокс"<<(int)mdiArea->activeSubWindow() <<" "<< (int)varsWnd;
                mdiArea->blockSignals(true);
                 varsWnd->maybeClose();
                mdiArea->blockSignals(false);
                if(varsWnd->isEditMode())//все еще режим редактирования
                {
                    QTimer::singleShot(0, this, SLOT(onVarsWndActivateByTimer()));//шаманство
                }
            }
            else
            {
                qDebug()<<"После деактивации вкладка осталась активной в mdi|"<<(int)mdiArea->activeSubWindow() <<" "<< (int)varsWnd;
            }
        }
    }
    else
    {
        //значит закрыли окно
        qDebug()<<"varsWnd has been closed";
    }
    //------------------------------------------------------------
    qDebug()<<"Окно доменов открыто: "<<domainWndIsopened;
    if(domainWndIsopened)
    {
        if(domainWnd->isEditMode()  )
        {
            qDebug()<<"Окно доменов в режиме редактирования";
            if(!(mdiArea->activeSubWindow() == domainsWndSbw) && !(mdiArea->activeSubWindow()==NULL))//деактивация формы была вызвана не всплывающей формой, а переключением
            {
                qDebug()<<"Это не был мессаджбокс"<<(int)mdiArea->activeSubWindow() <<" "<< (int)domainWnd;
                mdiArea->blockSignals(true);
                 domainWnd->maybeClose();
                mdiArea->blockSignals(false);
                if(domainWnd->isEditMode())//все еще режим редактирования
                {
                    QTimer::singleShot(0, this, SLOT(onDomainsWndActivateByTimer()));//шаманство
                }
            }
            else
            {
                qDebug()<<"После деактивации вкладка осталась активной в mdi|"<<(int)mdiArea->activeSubWindow() <<" "<< (int)domainWnd;
            }
        }
    }
    else
    {
        //значит закрыли окно
        qDebug()<<"domainsWnd has been closed";
    }


}

bool
ESmain::eventFilter(QObject *target, QEvent *event)
{
////    if(target == mdiArea)
////    {
////        qDebug()<<"Mdiarea Event "<<event->type();
////    }
//    if(target == rulesWnd)
//    {
////        if(event->type()==QEvent::WindowActivate || event->type()==QEvent::WindowDeactivate ||
////                event->type()==QEvent::WindowStateChange)
////        qDebug()<<"RulesWnd Event "<<event->type();
//        if(event->type() == QEvent::WindowStateChange)
//        {
//            QWindowStateChangeEvent* wsceEvent = static_cast<QWindowStateChangeEvent*>(event);
//            qDebug()<<"RulesWnd: QEvent::WindowStateChange OldState:"<<wsceEvent->oldState();
//        }
//    }
    return QMainWindow::eventFilter(target,event);
}


void
ESmain::onRulesWndActivateByTimer()
{
    qDebug()<<"ESmain::onRulesWndActivateByTimer()";
    QList<QMdiSubWindow*> sbwList = mdiArea->subWindowList();
    QMdiSubWindow *subWindow;
    foreach(subWindow,sbwList)
    {
        if(subWindow->windowTitle() == tr("Правила"))
        {
            mdiArea->setActiveSubWindow(subWindow);
            break;
        }
    }
}

void
ESmain::onVarsWndActivateByTimer()
{
    qDebug()<<"ESmain::onVarsWndActivateByTimer";
    QMdiSubWindow *subWindow = subwindowByName("Переменные");
    if(subWindow)
        mdiArea->setActiveSubWindow(subWindow);
}

void
ESmain::onDomainsWndActivateByTimer()
{
    qDebug()<<"ESmain::onDomainsWndActivateByTimer";
    QMdiSubWindow *subWindow = subwindowByName("Домены");
    if(subWindow)
        mdiArea->setActiveSubWindow(subWindow);
}

QMdiSubWindow*
ESmain::subwindowByName(QString wndName)
{
    QMdiSubWindow *sbw;
    QList<QMdiSubWindow*> swnds = mdiArea->subWindowList();
    foreach(sbw,swnds)
    {
        if(sbw->windowTitle() == wndName)
            return sbw;
    }
    return NULL;
}

void
ESmain::onRulesWndDestroyed()
{
    qDebug()<<"---------------ESmain::onRulesWndDestroyed()";
    Qt::WindowStates prevState;
    if(mdiArea->activeSubWindow()!=NULL)
    {
        prevState = mdiArea->activeSubWindow()->windowState();
    }
    mdiArea->removeSubWindow(subwindowByName("Правила"));//удаляем
    if(mdiArea->activeSubWindow()!=NULL)
    {
        mdiArea->activeSubWindow()->setWindowState(prevState);
    }
}

void
ESmain::onVarsWndDestroyed()
{
    qDebug()<<"---------------ESmain::onVarsWndDestroyed";
    Qt::WindowStates prevState;
    if(mdiArea->activeSubWindow()!=NULL)
    {
        prevState = mdiArea->activeSubWindow()->windowState();
    }
    mdiArea->removeSubWindow(subwindowByName("Переменные"));//удаляем
    if(mdiArea->activeSubWindow()!=NULL)
    {
        mdiArea->activeSubWindow()->setWindowState(prevState);
    }
}

void
ESmain::onDomainsWndDestroyed()
{
    qDebug()<<"---------------ESmain::onDomainsWndDestroyed";
    Qt::WindowStates prevState;
    if(mdiArea->activeSubWindow()!=NULL)
    {
        prevState = mdiArea->activeSubWindow()->windowState();
    }
    mdiArea->removeSubWindow(subwindowByName("Домены"));//удаляем
    if(mdiArea->activeSubWindow()!=NULL)
    {
        mdiArea->activeSubWindow()->setWindowState(prevState);
    }
}


void ESmain::open(QString str)
{
    DataModels::instance()->Open(str);
    OpenEditSubwindows();
    DataModels::instance()->dirty = false;
}
