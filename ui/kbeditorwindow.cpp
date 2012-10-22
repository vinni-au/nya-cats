#include "kbeditorwindow.hpp"
#include "ui_kbeditorwindow.h"
#include <QInputDialog>

KBEditorWindow::KBEditorWindow(NKBManager *kbManager,QWidget *parent) :
    QMainWindow(parent), m_kbManager(kbManager),
    ui(new Ui::KBEditorWindow)
{
    ui->setupUi(this);

    ui->splitter->setSizes(QList<int>() << 1 << 4);

    this->ui->treeView->setModel(m_kbManager->getFrameNetModel());

    QMenu* contextMenu = new QMenu;
    contextMenu->addAction("Добавить фрейм", ui->btnAddFrame, SLOT(click()));

    ui->graphicsView->setContextMenu(contextMenu);

    QDomElement diagramElement = m_kbManager->diagramNode().toElement();
    this->ui->graphicsView->fromXML(diagramElement);

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

    QObject::connect(ui->graphicsView->scene(), SIGNAL(arrowAdded(Arrow*)),
                     SLOT(arrowAdded(Arrow*)));

    QObject::connect(ui->graphicsView, SIGNAL(frameSelected(uint)),
                     SLOT(frameSelectedOnDiagram(uint)));
    QObject::connect(ui->graphicsView, SIGNAL(nodeDeleted(uint)),
                     SLOT(frameDeletedOnDiagram(uint)));
    QObject::connect(ui->graphicsView, SIGNAL(isaDeleted(uint,uint)),
                     SLOT(isaDeletedOnDiagram(uint,uint)));
    QObject::connect(ui->graphicsView, SIGNAL(apoDeleted(uint,uint)),
                     SLOT(apoDeletedOnDiagram(uint,uint)));
    QObject::connect(ui->treeView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     SLOT(treeviewSelectionChanged(QItemSelection,QItemSelection)));

    //Соединяем сигналы/слоты менеджера БЗ и диаграмминга
    QObject::connect(m_kbManager, SIGNAL(frameAdded(uint,QString)),
                     ui->graphicsView, SLOT(addNode(uint,QString)));
    QObject::connect(m_kbManager, SIGNAL(sigFrameNameChanged(uint,QString)),
                     ui->graphicsView, SLOT(changeNodeTitle(uint,QString)));
    QObject::connect(m_kbManager, SIGNAL(frameDeleted(uint)),
                     ui->graphicsView, SLOT(deleteNode(uint)));

    QObject::connect(m_kbManager, SIGNAL(isaAdded(uint,uint)),
                     SLOT(addIsa(uint,uint)));
    QObject::connect(m_kbManager, SIGNAL(isaDeleted(uint,uint)),
                     ui->graphicsView, SLOT(deleteLink(uint,uint)));
    QObject::connect(m_kbManager, SIGNAL(apoAdded(uint,uint)),
                     SLOT(addApo(uint,uint)));
    QObject::connect(m_kbManager, SIGNAL(apoDeleted(uint,uint)),
                     ui->graphicsView, SLOT(deleteLink(uint,uint)));

}

KBEditorWindow::~KBEditorWindow()
{
    delete ui;
}

void KBEditorWindow::addIsa(unsigned sid, unsigned did)
{
    ui->graphicsView->addLink(sid, did, "is-a");
}

void KBEditorWindow::addApo(unsigned sid, unsigned did)
{
    ui->graphicsView->addLink(sid, did, "sub");
}

void KBEditorWindow::frameDeletedOnDiagram(unsigned id)
{
    if (m_kbManager->deleteFrame(id))
        ;//ui->graphicsView->deleteNode(id);
}

void KBEditorWindow::isaDeletedOnDiagram(unsigned sid, unsigned did)
{
    if (m_kbManager->deleteIsa(sid, did))
        ui->graphicsView->deleteLink(sid, did);
}

void KBEditorWindow::apoDeletedOnDiagram(unsigned sid, unsigned did)
{
    if (m_kbManager->deleteApo(sid, did))
        ;//ui->graphicsView->deleteLink(sid, did);
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
    bool ok;
    QString name = QInputDialog::getText(this, "Создать слот",
        "Введите имя слота", QLineEdit::Normal, QString(), &ok);
    if (ok && !name.isEmpty())
    {
        //нужно проверить, а есть ли во фрейме такой слот

        QModelIndex index = ui->treeView->selectionModel()->currentIndex();
        QString frameName = m_kbManager->getFrameNetModel()->getFrameNameByIndex(index);
        if(m_kbManager->slotExists(frameName,name))
        {
            QMessageBox::information(this,"","Не удалось добавить слот. Такой слот уже есть.",QMessageBox::Ok);
            return;
        }

        NFramenetModel *model = qobject_cast<NFramenetModel*>(ui->treeView->model());
        QModelIndex newSlotIndex = model->addSlot(index);
        if(! (newSlotIndex.isValid()))
        {
            QMessageBox::information(this,"","Не удалось добавить слот. Фрейм не выбран",QMessageBox::Ok);
            return;
        }
        model->setData(newSlotIndex, QVariant(name), Qt::EditRole);
        SlotEditorWnd *wnd = new SlotEditorWnd(newSlotIndex,m_kbManager,this);
        wnd->setModal(true);
        wnd->show();
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
    this->m_kbManager->setDiagramNode( ui->graphicsView->toXML( m_kbManager->doc  ) );
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
    ui->treeView->setColumnHidden(5,true);
    ui->treeView->setColumnHidden(6,true);//вариант наследования всегда скрыт
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
            ;
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

void KBEditorWindow::arrowAdded(Arrow *arrow)
{
    bool isIsa = ui->btn_addIsa->isChecked();
    if (isIsa) {
        if (!m_kbManager->addIsa(arrow->startItem()->id(), arrow->endItem()->id()))
            ui->graphicsView->scene()->removeItem(arrow);
        else {
            //ui->graphicsView->addArrow(arrow);
            arrow->setText("is-a");
            arrow->setColor(Qt::darkGreen);
        }
    } else {
        if (!m_kbManager->addApo(arrow->startItem()->id(), arrow->endItem()->id()))
            ui->graphicsView->scene()->removeItem(arrow);
        else {
            //ui->graphicsView->addArrow(arrow);
            arrow->setText("sub");
            arrow->setColor(Qt::darkYellow);
        }
    }
}

void KBEditorWindow::frameSelectedOnDiagram(unsigned id)
{
    QModelIndex index = m_kbManager->getFrameNetModel()->getFrameIndexById(id);
    QItemSelectionModel* smodel = ui->treeView->selectionModel();
    smodel->clear();
    smodel->setCurrentIndex(index, QItemSelectionModel::Select);
    ui->treeView->setSelectionModel(smodel);
    ui->treeView->collapseAll();
    ui->treeView->expand(index);
}

void KBEditorWindow::treeviewSelectionChanged(QItemSelection selected, QItemSelection deselected)
{
    if (selected.count() == 1) {
        QModelIndex sindex = selected.indexes().at(0);
        unsigned id = m_kbManager->getIdByIndex(sindex);
        ui->graphicsView->selectNode(id);
    }
}


void KBEditorWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    this->on_btnEditSlot_clicked();
}
