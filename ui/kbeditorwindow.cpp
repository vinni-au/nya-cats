#include "kbeditorwindow.hpp"
#include "ui_kbeditorwindow.h"
#include <QInputDialog>

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

void KBEditorWindow::on_act_addFrame_triggered()
{
    bool ok;
    QString name = QInputDialog::getText(this, "������� �����", "������� ��� ������:", QLineEdit::Normal, QString(), &ok);

    if (ok && !name.isEmpty()) {
        //��������� ������ � ��������� �� �� ���������� ������
    }
}
