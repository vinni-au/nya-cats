#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "ui/kbeditorwindow.hpp"


#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_kbManager = new NKBManager();


    vlayout = new QVBoxLayout();
    {
        viz = new Visualizer(m_kbManager, this);    vlayout->addWidget(viz);


        hlayout = new QHBoxLayout;    vlayout->addLayout(hlayout);
        {
            btnStartGame = new QPushButton("Начать игру");  hlayout->addWidget(btnStartGame);
            QObject::connect(btnStartGame,SIGNAL(clicked()),SLOT(on_actStartGame_triggered()));

            btnStartRandomGame = new QPushButton("Начать случайную игру");  hlayout->addWidget(btnStartRandomGame);
            QObject::connect(btnStartRandomGame,SIGNAL(clicked()),SLOT(on_actStartRandGame_triggered()));

            btnStep = new QPushButton("Сделать шаг");  hlayout->addWidget(btnStep);
            QObject::connect(btnStep,SIGNAL(clicked()),SLOT(on_actDoStep_triggered()));

            btnStopGame = new QPushButton("Закончить игру");  hlayout->addWidget(btnStopGame);
            QObject::connect(btnStopGame,SIGNAL(clicked()),SLOT(on_actStopGame_triggered()));


        }

    }


    ui->centralWidget->setLayout(vlayout);

    setEnabledStartGame(true);
    setEnabledStartRandomGame(true);
    setEnabledStep(false);
    setEnabledStopGame(false);

    m_mlv = new MLV(m_kbManager, viz);
	//m_mlv->SetFullSearch(true);
    m_mlvControl = new MLVControl(m_mlv);


    QObject::connect(viz->GetGrid(), SIGNAL(sigFindOutSituation(int,int)),
        SLOT(runMLV(int,int)));
	QObject::connect(viz->GetGrid(), SIGNAL(sigDoVisualizerCell(Cell*)), m_mlv,
		SLOT(DoVisualizerCell(Cell*)));
	QObject::connect(viz->GetGrid(), SIGNAL(sigGetInfo(int,int)), m_mlv,
		SLOT(getInfo(int,int)));

    this->setWindowState(Qt::WindowMaximized);

    QObject::connect(m_kbManager,SIGNAL(sigDirtyChanged(bool)),this,SLOT(onKBDirtyChanged(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::runMLV(int x, int y)
{
    qDebug() << "RunMLV in (" + QString::number(x) + "," + QString::number(y) + ")";
    if (!m_mlv)
        return;

    ui->mainStatusBar->showMessage("Идет привязка...");

    m_mlvControl->ClearLog();
    m_mlvControl->show();
    m_mlv->Start(x, y);

    ui->mainStatusBar->clearMessage();
}

void MainWindow::on_actExit_triggered()
{
    this->close();
}

void MainWindow::on_actAbout_triggered()
{
    QMessageBox::about(this,"О программе...","<h1>NYA CATS (Not Yet Another Cognitive Actors Team Survival)</h1>\n"
                       "<p>Авторы: Бобров А.А., Киселев А.Ю., Сторожев А.А.</p>  \n"
                       "<p>Копирайт &copy; 2012-2013 ДаЁжСофт Software Inc.</p>");
}

void MainWindow::on_actFrameEditor_triggered()
{
    KBEditorWindow* wnd = new KBEditorWindow(m_kbManager,m_mlv,this);
    wnd->show();
}

void MainWindow::on_actCreate_triggered()
{
    if(m_kbManager->mayBeSave())
    {
        m_kbManager->Clear();
        this->setWindowTitle(QString(MAINWINDOWTITLE));
    }
}

void MainWindow::on_actOpen_triggered()
{
    qDebug()<<"MainWindow::on_actOpen_triggered";

    if(m_kbManager->mayBeSave())
    {
        QString kbFile = QFileDialog::getOpenFileName(this,"Открыть БЗ..",QDir::currentPath(),tr("Knowle Database (*.nya)"),0);
        if( !kbFile.isEmpty())
        {
            m_kbManager->Clear();
            if(!m_kbManager->Open(kbFile))
            {
                QMessageBox msgBox;
                msgBox.setText(tr("Не удалось открыть файл."));
                msgBox.exec();
            }
            else
            {
                this->setWindowTitle(QString(MAINWINDOWTITLE)+" "+ kbFile);
                m_kbManager->setDirty(false);//иначе все дело портит вопрос
                //statusBar()->showMessage(QString("Открыт файл: %1").arg(esFile),2000);
                //QMessageBox::information(this,"Информация","База знаний загружена",QMessageBox::Ok);
                this->setStatusTip("База знаний загружена");
                m_mlv->ClearAll();
            }
        }
    }
}

void MainWindow::on_actClose_triggered()
{
    on_actCreate_triggered();
    m_mlv->ClearAll();
}

void MainWindow::on_actSave_triggered()
{
    qDebug()<<"MainWindow::on_actSave_triggered)";
    if(m_kbManager->file()==NULL)
    {
        on_actSaveAs_triggered();
    }
    else
    {
        if(m_kbManager->isValid())
        {
            m_kbManager->save();
        }
        else
        {
            QMessageBox::warning(this,"Ошибка!","Данные невалидны. Не сохранено.");
        }
    }
}

void MainWindow::on_actSaveAs_triggered()
{
    qDebug()<<" MainWindow::";

    saveAs();
}
bool
MainWindow::saveAs()
{
    return m_kbManager->SaveAs();

    ////////////////////////////////////////////////////
//    if(!DataModels::instance()->isValid())
//    {
//        QMessageBox mb;
//        mb.setText("В данных ошибка. Не сохранено.");
//        mb.exec();
//        return false;
//    }
//    QString esFile=QFileDialog::getSaveFileName(this,tr("Сохранить ЭС как.."),QDir::currentPath(),tr("Expert system (*.esxml)"),0);
//    if(!esFile.isEmpty())
//    {
//        qDebug()<<esFile;
//        if(!DataModels::instance()->saveAs(esFile.append(".esxml")))
//        {
//            //если не сохранилось
//            return false;
//        }
//        return true;
//    }
//    else
//        return false;
}

void
MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<"MainWindow::closeEvent";
    if(m_kbManager->mayBeSave())
    {
        event->accept();
    }
    else
        event->ignore();
}

void MainWindow::on_actDomainEditor_triggered()
{
    DomainWnd *wnd = new DomainWnd(m_kbManager,this);
    //wnd->setWindowModality(Qt::WindowModal);
    wnd->show();
}

void MainWindow::on_actStartGame_triggered()
{

    setEnabledStartGame(false);
    setEnabledStartRandomGame(false);
    setEnabledStep(true);
    setEnabledStopGame(true);

    ui->mainStatusBar->showMessage("Идет привязка...");
    if (!m_mlv)
    {
        setEnabledStartGame(true);
        setEnabledStartRandomGame(true);
        setEnabledStep(false);
        setEnabledStopGame(false);
        return;
    }
    m_mlvControl->ClearLog();
    m_mlvControl->show();
    m_mlv->Start();
    ui->mainStatusBar->clearMessage();
}

void MainWindow::on_actStopGame_triggered()
{
    if (!m_mlv) return;
    m_mlv->Stop();
    m_mlvControl->show();

    setEnabledStartGame(true);
    setEnabledStartRandomGame(true);
    setEnabledStep(false);
    setEnabledStopGame(false);
}

void MainWindow::on_actDoStep_triggered()
{
    ui->mainStatusBar->showMessage("Идет привязка...");
    if (!m_mlv) return;
    m_mlv->Step();
    m_mlvControl->show();
    ui->mainStatusBar->clearMessage();
}

void MainWindow::on_actStartRandGame_triggered()
{

    setEnabledStartGame(false);
    setEnabledStartRandomGame(false);
    setEnabledStep(true);
    setEnabledStopGame(true);

    ui->mainStatusBar->showMessage("Идет привязка...");
    if (!m_mlv)
    {
        setEnabledStartGame(true);
        setEnabledStartRandomGame(true);
        setEnabledStep(false);
        setEnabledStopGame(false);
        return;
    }
    m_mlvControl->ClearLog();
    m_mlvControl->show();
    m_mlv->RandomStart();
    ui->mainStatusBar->clearMessage();
}

void MainWindow::onKBDirtyChanged(bool)
{
    viz->RedrawItems();
}

void MainWindow::setEnabledStartGame(bool enabled)
{
    ui->actStartGame->setEnabled(enabled);
    btnStartGame->setEnabled(enabled);
}

void MainWindow::setEnabledStartRandomGame(bool enabled)
{
    ui->actStartRandGame->setEnabled(enabled);
    btnStartRandomGame->setEnabled(enabled);
}

void MainWindow::setEnabledStep(bool enabled)
{
    ui->actDoStep->setEnabled(enabled);
    btnStep->setEnabled(enabled);
}

void MainWindow::setEnabledStopGame(bool enabled)
{
    ui->actStopGame->setEnabled(enabled);
    btnStopGame->setEnabled(enabled);
}

void MainWindow::on_actGlobalProcsEditor_triggered()
{
    NProc *proc = new NProc();
    proc->setProc( m_kbManager->globalProcsContext() );
    ProcEditor *procEditor = new ProcEditor(proc,false);
    procEditor->setWindowModality(Qt::ApplicationModal);
    QObject::connect(procEditor,SIGNAL(sigProcAdded(NProc*,bool)),this,SLOT(onGlobalProcAdded(NProc*,bool)));
    procEditor->show();
}

void MainWindow::onGlobalProcAdded(NProc *proc,bool newProc)
{
    if(m_kbManager->globalProcsContext()!=proc->proc())
        m_kbManager->setDirty(true);
    m_kbManager->setGlobalProcsContext( proc->proc() );
}
