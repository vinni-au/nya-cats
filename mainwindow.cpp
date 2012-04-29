#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "ui/kbeditorwindow.hpp"


#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    viz = new Visualizer(this);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(viz);
    ui->centralWidget->setLayout(layout);

    m_kbManager = new NKBManager();
    m_mlv = new MLV(m_kbManager, viz->GetGrid());
    m_mlvControl = new MLVControl(m_mlv);
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
    QMessageBox::about(this,"О программе...","<h1>NYA CATS (Not Yet Another Cognitive Bla-bla-bla)</h1>\n"
                       "<p>Авторы: Бобров А.А., Киселев А.Ю., Стороженко А.А.</p>  \n"
                       "<p>Копирайт &copy; 2012 ДаЁжСофт Software Inc.</p>");
}

void MainWindow::on_actFrameEditor_triggered()
{
    KBEditorWindow* wnd = new KBEditorWindow(m_kbManager,this);
    wnd->show();
}

void MainWindow::on_actCreate_triggered()
{
    if(m_kbManager->mayBeSave())
    {
        m_kbManager->Clear();
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

                m_kbManager->setDirty(false);//иначе все дело портит вопрос
                //statusBar()->showMessage(QString("Открыт файл: %1").arg(esFile),2000);
                QMessageBox::information(this,"Информация","База знаний загружена",QMessageBox::Ok);
            }
        }
    }
}

void MainWindow::on_actClose_triggered()
{
    on_actCreate_triggered();
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
    if (!m_mlv)
        return;

    m_mlvControl->ClearLog();
    m_mlvControl->show();
    m_mlv->Start();
}
