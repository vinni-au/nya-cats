#ifndef PROCEDITOR_H
#define PROCEDITOR_H

#include <QWidget>
#include "core/nproc.h"

namespace Ui {
    class ProcEditor;
}

class ProcEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ProcEditor(NProc *proc,bool newProc,QWidget *parent = 0);
    ~ProcEditor();

signals:
    void sigProcAdded(NProc *proc,bool newProc);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ProcEditor *ui;

    NProc   *m_proc;
    bool    m_newProc;  //новая или на редактирование
};

#endif // PROCEDITOR_H
