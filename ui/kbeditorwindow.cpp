#include "kbeditorwindow.hpp"
#include "ui_kbeditorwindow.h"

KBEditorWindow::KBEditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KBEditorWindow)
{
    ui->setupUi(this);
}

KBEditorWindow::~KBEditorWindow()
{
    delete ui;
}

void KBEditorWindow::on_btnAddFrame_clicked()
{

}

void KBEditorWindow::on_btnDeleteFrame_clicked()
{

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
