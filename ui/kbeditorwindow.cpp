#include "kbeditorwindow.hpp"
#include "ui_kbeditorwindow.h"
#include <QInputDialog>

KBEditorWindow::KBEditorWindow(NKBManager *kbManager,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KBEditorWindow)
{
    ui->setupUi(this);

    this->m_kbManager = kbManager;

    this->ui->treeView->setModel(m_kbManager->getFrameNetModel());

    m_simpleView = false;
    setSimpleView(m_simpleView);
    //Контекстное меню для инспектора объектов
    QAction *actSwitch = new QAction("Переключить вид простой/подробный",ui->treeView);
    ui->treeView->addAction(actSwitch);
    QObject::connect(actSwitch,SIGNAL(triggered()),SLOT(onSimpleViewSwitched()));

    ui->treeView->header()->setResizeMode(QHeaderView::ResizeToContents);

    QObject::connect(ui->btn_zoomIn, SIGNAL(clicked()),
                     ui->graphicsView, SLOT(zoomIn()));
    QObject::connect(ui->btn_zoomOut, SIGNAL(clicked()),
                     ui->graphicsView, SLOT(zoonOut()));

    //Соединяем сигналы/слоты менеджера БЗ и диаграмминга
    QObject::connect(m_kbManager, SIGNAL(frameAdded(uint,QString)),
                     ui->graphicsView, SLOT(addNode(uint,QString)));
    QObject::connect(m_kbManager, SIGNAL(frameDeleted(uint)),
                     ui->graphicsView, SLOT(deleteNode(uint)));
}

KBEditorWindow::~KBEditorWindow()
{
    delete ui;
}

void KBEditorWindow::on_btnAddFrame_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Создать фрейм", "Введите имя фрейма:", QLineEdit::Normal, QString(), &ok);

    if (ok && !name.isEmpty()) {
        //Выполнить запрос к менеджеру БЗ на добавление фрейма
        if(m_kbManager->addFrame(name))
        {
            //ok
        }
        else
        {
            QMessageBox::information(this,"","Не удалось добавить фрейм",QMessageBox::Ok);
        }
    }
}

void KBEditorWindow::on_btnDeleteFrame_clicked()
{
    int frameId=-1;

    //TODO Получить ид выделенного фрейма
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::information(this,"","Ни один фрейм не выбран",QMessageBox::Ok);
        return;
    }
    NFramenetModel *model = qobject_cast<NFramenetModel*>(ui->treeView->model());

    frameId = model->getIdByIndex(index);

    if(frameId<0)
    {
        QMessageBox::information(this,"","Не удалось удалить фрейм",QMessageBox::Ok);
        return;
    }

    m_kbManager->deleteFrame(frameId);
}

void KBEditorWindow::on_btnAddSlot_clicked()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    NFramenetModel *model = qobject_cast<NFramenetModel*>(ui->treeView->model());
    if(!model->addSlot(index))
    {
        QMessageBox::information(this,"","Не удалось добавить слот. Фрейм не выбран",QMessageBox::Ok);
    }
}

void KBEditorWindow::on_btnEditSlot_clicked()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    NFramenetModel *model = qobject_cast<NFramenetModel*>(ui->treeView->model());
    if(model->isSlot(index))
    {
        SlotEditorWnd *wnd = new SlotEditorWnd(index,m_kbManager,this);
        wnd->setModal(true);
        wnd->show();
    }
    else
    {
        QMessageBox::information(this,"","Выделите слот, пожалуйста.",QMessageBox::Ok);
    }
}

void KBEditorWindow::on_btnDeleteSlot_clicked()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    NFramenetModel *model = qobject_cast<NFramenetModel*>(ui->treeView->model());
    if(!model->deleteSlot(index))
    {
        QMessageBox::information(this,"","Не выбран слот",QMessageBox::Ok);
    }
}

void KBEditorWindow::on_btnOk_clicked()
{
    this->m_kbManager->save();
    close();
}

void KBEditorWindow::on_btnCancel_clicked()
{
    m_kbManager->reload();
    close();
}

void KBEditorWindow::onSimpleViewSwitched()
{
    m_simpleView = !m_simpleView;
    setSimpleView(m_simpleView);
}

void KBEditorWindow::setSimpleView(bool isSimple)
{
    ui->treeView->setColumnHidden(1,isSimple);
    ui->treeView->setColumnHidden(3,isSimple);
    ui->treeView->setColumnHidden(4,isSimple);
    ui->treeView->setColumnHidden(5,isSimple);
    ui->treeView->setColumnHidden(6,isSimple);
}

void KBEditorWindow::on_btn_addIsa_toggled(bool checked)
{
    if (checked) {
        ui->btn_addSub->setChecked(false);
        ui->graphicsView->scene()->setMode(DiagramScene::InsertLine);
    } else ui->graphicsView->scene()->setMode(DiagramScene::MoveItem);
}

void KBEditorWindow::on_btn_addSub_toggled(bool checked)
{
    if (checked) {
        ui->btn_addIsa->setChecked(false);
        ui->graphicsView->scene()->setMode(DiagramScene::InsertLine);
    } else ui->graphicsView->scene()->setMode(DiagramScene::MoveItem);
}


void
KBEditorWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<"KBEditorWindow::closeEvent";

    if(m_kbManager->mayBeSave())//пытаемся сохранить
    {
        if(m_kbManager->saved())//нажал сохранить
        {

        }
        else
        {
            on_btnCancel_clicked();
        }
    }
    else
    {
        event->ignore();
        return;
    }

    event->accept();
    //emit sigClosed();
}


