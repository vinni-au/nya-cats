#include "proceditor.h"
#include "ui_proceditor.h"

ProcEditor::ProcEditor(NProc *proc,bool newProc,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcEditor),
    m_proc(proc),
    m_newProc(newProc)
{
    ui->setupUi(this);

    if(!newProc)
        this->ui->plainTextEdit->setPlainText(proc->proc());
}

ProcEditor::~ProcEditor()
{
    delete ui;
}

void ProcEditor::on_buttonBox_accepted()
{
    m_proc->setProc( ui->plainTextEdit->toPlainText() );
    emit sigProcAdded(m_proc,m_newProc);
    this->close();
}

void ProcEditor::on_buttonBox_rejected()
{
    this->close();
}
