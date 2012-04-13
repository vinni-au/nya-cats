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
