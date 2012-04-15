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

}

void KBEditorWindow::on_btnEditSlot_clicked()
{

}

void KBEditorWindow::on_btnDeleteSlot_clicked()
{

}

void KBEditorWindow::on_btnOk_clicked()
{

}

void KBEditorWindow::on_btnCancel_clicked()
{

}
