#include "questionwnd.h"

QuestionWnd::QuestionWnd(QWidget *parent) :
    QDialog(parent)
{
    /*QVBoxLayout **/layMain = new QVBoxLayout();   this->setLayout(layMain);
    {
        /*QLabel      **/lbQuestion = new QLabel("Вопрос");     layMain->addWidget(lbQuestion);
        /*QHBoxLayout **/layVariants=new QHBoxLayout(); layMain->addLayout(layVariants);
        {
            /*QLabel      **/lbVar = new QLabel("Переменная = ");          layVariants->addWidget(lbVar);
            /*QComboBox   **/cmbVariants = new QComboBox(); layVariants->addWidget(cmbVariants);

        }
        layMain->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding));
        /*QHBoxLayout **/layButtons=new QHBoxLayout(); layMain->addLayout(layButtons);
        {
            /*QPushButton **/btnOk = new QPushButton("OK"); layButtons->addWidget(btnOk);
            /*QPushButton **/btnCancel = new QPushButton("Отмена"); layButtons->addWidget(btnCancel);

        }
    }

    connect(btnOk,SIGNAL(clicked()),SLOT(accept()));
    connect(btnCancel,SIGNAL(clicked()),SLOT(reject()));
}
void
QuestionWnd::SetQuestion(QModelIndex varIndex)
{
    QModelIndex questIndex = DataModels::instance()->varModel->index(3,0,varIndex);
    this->lbQuestion->setText( DataModels::instance()->varModel->data(questIndex,Qt::DisplayRole).toString()  );
    if(lbQuestion->text().trimmed().isEmpty())//если пустой вопрос, то задаем вопрос по умолчанию
    {
        this->lbQuestion->setText( DataModels::instance()->varModel->data(varIndex,Qt::DisplayRole).toString()+" = ?"  );
    }

    this->lbVar->setText(DataModels::instance()->varModel->data(varIndex,Qt::DisplayRole).toString()+" = ");

    QModelIndex domainIndexInVar = DataModels::instance()->varModel->index(2,0,varIndex);
    QString sDomain = DataModels::instance()->varModel->data(domainIndexInVar,Qt::DisplayRole).toString();
    QModelIndex domainIndexInDomains = DataModels::instance()->domainModel->indexByName(sDomain);
    this->cmbVariants->setModel(DataModels::instance()->domainModel);
    this->cmbVariants->setRootModelIndex(domainIndexInDomains);
    this->cmbVariants->setCurrentIndex(0);
}
QString
QuestionWnd::GetAnswer()
{
    return this->cmbVariants->currentText();
}
