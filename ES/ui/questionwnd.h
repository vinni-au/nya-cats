#ifndef QUESTIONWND_H
#define QUESTIONWND_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include "core/datamodels.h"

class QuestionWnd : public QDialog
{
    Q_OBJECT
public:
    explicit QuestionWnd(QWidget *parent = 0);

signals:

public slots:

public:
    void SetQuestion(QModelIndex varIndex);
    QString GetAnswer();
public:
    QVBoxLayout *layMain;
        QLabel      *lbQuestion;
        QHBoxLayout *layVariants;
            QLabel      *lbVar;
            QComboBox   *cmbVariants;
        QHBoxLayout *layButtons;
            QPushButton *btnOk;
            QPushButton *btnCancel;




};

#endif // QUESTIONWND_H
