#include "expreditor.h"
#include <QDebug>

ExprEditor::ExprEditor(NKBManager* manager,QModelIndex slotIndex,QString domain,TypeExpr typeExpr,TypeAction typeAction,QWidget *parent) :
    QDialog(parent)
{
    m_kbManager = manager;
    m_slotIndex = slotIndex;
    m_domain = domain;
    this->typeExpr=typeExpr;
    this->typeAction=typeAction;
    /*QVBoxLayout **/layMain=new QVBoxLayout();                     this->setLayout(layMain);
    {
        /*QHBoxLayout    **/layUp=new QHBoxLayout();                layMain->addLayout(layUp);
        {
            /*QVBoxLayout    **/layVar=new QVBoxLayout();           layUp->addLayout(layVar);
            {
                /*QLabel          **/lbVar=new QLabel();            layVar->addWidget(lbVar);
                /*QComboBox       **/cmbVar=new MyComboBox();        layVar->addWidget(cmbVar);
                layVar->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding));
            }
            /*QVBoxLayout    **/laySign=new QVBoxLayout();          layUp->addLayout(laySign);
            {
                /*QLabel          **/lbSign=new QLabel();           laySign->addWidget(lbSign);
                /*QComboBox       **/cmbSign=new QComboBox();       laySign->addWidget(cmbSign);
                laySign->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding));
            }
            /*QVBoxLayout    **/layVal=new QVBoxLayout();           layUp->addLayout(layVal);
            {
                /*QLabel          **/lbVal=new QLabel();            layVal->addWidget(lbVal);
                /*QComboBox       **/cmbVal=new MyComboBox();        layVal->addWidget(cmbVal);
                layVal->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding));

            }
            //////
            /*QVBoxLayout    **/layArSign=new QVBoxLayout();           layUp->addLayout(layArSign);
            {
                /*QLabel          **/lbArSign=new QLabel();            layArSign->addWidget(lbArSign);
                /*QComboBox       **/cmbArSign=new MyComboBox();        layArSign->addWidget(cmbArSign);
                layArSign->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding));

            }
            /*QVBoxLayout    **/laySummer=new QVBoxLayout();           layUp->addLayout(laySummer);
            {
                /*QLabel          **/lbSummer=new QLabel();            laySummer->addWidget(lbSummer);
                /*QComboBox       **/spbSummer=new QSpinBox();        laySummer->addWidget(spbSummer);
                laySummer->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding));

            }

        }

        /*QHBoxLayout    **/layDown=new QHBoxLayout();              layMain->addLayout(layDown);
        {
            /*QPushButton   **/btnAddNewExpr=new QPushButton();     layDown->addWidget(btnAddNewExpr);
            layDown->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding));
            /*QPushButton   **/btnOK=new QPushButton();             layDown->addWidget(btnOK);
            /*QPushButton   **/btnCancel=new QPushButton();         layDown->addWidget(btnCancel);
        }
    }
    retranslate();

    cmbArSign->addItem("");
    cmbArSign->addItem("+");

//!    this->cmbVar->setModel(DataModels::instance()->varModel);
    QString frameName = m_kbManager->getFrameNetModel()->getFrameNameByIndex(m_slotIndex);
    QStringList slots1= m_kbManager->getVarsWithParents(frameName);
    cmbVar->addItems(slots1);


    if(typeExpr == ExprEditor::IfExpr)
    {

        this->cmbSign->addItem("=");
        this->cmbSign->addItem("!=");
    }
    else
    {
        this->cmbSign->addItem("=");
    }
    if(typeAction == ExprEditor::EditAction)
        btnAddNewExpr->setVisible(false);
    if(typeAction == ExprEditor::AddAction)//чтобы при добавлении новой переменной показывались значения
        onCurrentVarChanged(0);
    //сигналы
    connect(btnCancel,SIGNAL(clicked()),SLOT(onCancelClick()));//Cancel
    connect(btnOK,SIGNAL(clicked()),SLOT(onOkClick()));//OK
    connect(btnAddNewExpr,SIGNAL(clicked()),SLOT(onOkAndReturnClick()));//OkAndAgain

    connect(cmbVar,SIGNAL(currentIndexChanged(int)),SLOT(onCurrentVarChanged(int)));

    //контекстное пополнение

    connect(cmbVar,SIGNAL(sigNewValueEntered(QString)),SLOT(onVarContextAdded(QString)));
    connect(cmbVal,SIGNAL(sigNewValueEntered(QString)),SLOT(onValueContextAdded(QString)));
}
void
ExprEditor::retranslate()
{
    lbVar->setText(tr("Переменная"));
    lbSign->setText(tr("Знак"));
    lbVal->setText(tr("Значение"));

    btnAddNewExpr->setText(tr("Добавить несколько"));
    btnOK->setText(tr("ОК"));
    btnCancel->setText(tr("Отмена"));

    if(typeAction == ExprEditor::AddAction)
    {
        this->setWindowTitle(tr("Добавить "));
    }
    else
    {
        this->setWindowTitle(tr("Редактирвать "));
    }
    if(typeExpr == ExprEditor::IfExpr)
    {
        this->setWindowTitle(windowTitle()+tr("условие") );
    }
    else
    {
        this->setWindowTitle(windowTitle()+tr("заключение") );
    }
}
void
ExprEditor::onCurrentVarChanged(int inx)
{
    QString frameName = m_kbManager->getFrameNetModel()->getFrameNameByIndex( m_slotIndex );
    QString slotName = m_kbManager->getFrameNetModel()->getSlotFasetValue( m_slotIndex,"default_value"  ).toString();
    QString domain;
    if(cmbVar->currentText() == slotName)
    {//домен берем из конструктора
        domain = m_domain;
    }
    else
    {//домен берем из сети фреймов
        domain = m_kbManager->getDomainByString( frameName, cmbVar->currentText()  );
    }


    cmbArSign->setVisible(domain == "int");
    spbSummer->setVisible(domain == "int");


    if(domain.isEmpty() || domain == "string" || domain == "int" ||domain == "frame" )
    {
        cmbVal->setModel(new QStringListModel());
        return;
    }

    QModelIndex domainIndex = m_kbManager->getDomainModel()->indexByName(domain);
    cmbVal->setModel(m_kbManager->getDomainModel());
    cmbVal->setRootModelIndex(domainIndex);
    cmbVal->setCurrentIndex(0);
}

Expr*
ExprEditor::getExpr()
{
    Expr *expr = new Expr();
    expr->var = this->cmbVar->currentText();
    expr->sign = this->cmbSign->currentText();
    expr->val = this->cmbVal->currentText();

    expr->arSign = this->cmbArSign->currentText();
    expr->summer = spbSummer->value();
    return expr;
}

bool
ExprEditor::dataIsValid()//валидация
{
//    QModelIndex varIndex = DataModels::instance()->varModel->indexByName(cmbVar->currentText().trimmed());
//    if(!varIndex.isValid())
//        return false;

//    QModelIndex domainInVarIndex = DataModels::instance()->varModel->index(2,0,varIndex);
//    QString sDomain = DataModels::instance()->varModel->data(domainInVarIndex,Qt::DisplayRole).toString();


//    if(DataModels::instance()->domainModel->valueExists(sDomain,cmbVal->currentText().trimmed()))
//        return true;
//    else
//        return false;
    return true;
}

void
ExprEditor::onOkClick()
{
    qDebug()<<"ExprEditor::onOkClick";
    if(!dataIsValid())
    {
        QMessageBox::information(this,"Ошибка!","Ошибка во введенных данных. Возможно вручную была введена новая переменная или значение, но не подтверждено нажатием Enter.");
        return;
    }

    startAgain=false;
    accept();
}
void
ExprEditor::onCancelClick()
{
    qDebug()<<"ExprEditor::onCancelClick";
    startAgain=false;
    reject();
}
void
ExprEditor::onOkAndReturnClick()
{
    qDebug()<<"ExprEditor::onOkAndReturnClick";
    if(!dataIsValid())
    {
        QMessageBox::information(this,"Ошибка!","Ошибка во введенных данных. Возможно вручную была введена новая переменная или значение, но не подтверждено нажатием Enter.");
        return;
    }
    startAgain=true;
    accept();
}
void ExprEditor::reject()
{
    startAgain=false;
    QDialog::reject();
}
void
ExprEditor::selectVar(QModelIndex index)
{
    cmbVar->setCurrentIndex(index.row());
    if(index.row()==0)
        onCurrentVarChanged(index.row());
}

void ExprEditor::selectVar(QString var)
{
    int i=cmbVar->findText(var);
    cmbVar->setCurrentIndex(i);
}

void
ExprEditor::selectSign(QString sign)
{
    for(int i=0;i<cmbSign->count();++i)                  //выбираем знак
    {
        if(cmbSign->itemText(i)==sign)
        {
            cmbSign->setCurrentIndex(i);
            break;
        }
    }
}
void
ExprEditor::selectVal(QString val)
{
    for(int i=0;i<cmbVal->count();++i)                  //выбираем значение
    {
        if(cmbVal->itemText(i)==val)
        {
            cmbVal->setCurrentIndex(i);
            break;
        }
    }
}

//контекстное пополнение

void
ExprEditor::onVarContextAdded(QString newVar)
{
//    qDebug()<<"ExprEditor::onVarContextAdded: "<<newVar;
//    if(Saver::areUsure(QString("Вы хотите добавить новую переменную  %1 ?").arg("'"+newVar+"'")))
//    {
//        QModelIndex varIndex = DataModels::instance()->varModel->addVariable();
//        DataModels::instance()->varModel->setData(varIndex,newVar,Qt::EditRole);
//        //сделать домен по-умолчанию №0
//        QModelIndex firstDomainIndex = DataModels::instance()->domainModel->index(0,0,QModelIndex());
//        QString domainName = DataModels::instance()->domainModel->data(firstDomainIndex,Qt::DisplayRole).toString();

//        QModelIndex domainInVar = DataModels::instance()->varModel->index(2,0,varIndex);
//        DataModels::instance()->varModel->setData(domainInVar,domainName,Qt::EditRole);

//        QModelIndex askingIndex = DataModels::instance()->varModel->index(0,0,varIndex);
//        QModelIndex derivableIndex = DataModels::instance()->varModel->index(1,0,varIndex);
//        if(this->typeExpr == ExprEditor::IfExpr)
//        {//добавляем запрашиваемую переменную (в модели по-умолчанию)
//            DataModels::instance()->varModel->setData(askingIndex,true,Qt::EditRole);
//            DataModels::instance()->varModel->setData(derivableIndex,false,Qt::EditRole);
//        }
//        else//ThenExpr
//        {//делаем переменную выводимой
//            DataModels::instance()->varModel->setData(askingIndex,false,Qt::EditRole);
//            DataModels::instance()->varModel->setData(derivableIndex,true,Qt::EditRole);
//        }

//        //обновить домен
//        onCurrentVarChanged(varIndex.row());
//    }
}

void
ExprEditor::onValueContextAdded(QString newVal)
{
//    qDebug()<<"ExprEditor::onValueContextAdded: "<<newVal;

//    QModelIndex varIndex = DataModels::instance()->varModel->indexByName(cmbVar->currentText().trimmed());
//    QModelIndex domainInVarIndex = DataModels::instance()->varModel->index(2,0,varIndex);
//    QString sDomain = DataModels::instance()->varModel->data(domainInVarIndex,Qt::DisplayRole).toString();

//    if(sDomain.isEmpty())
//    {
//        QMessageBox::warning(this,"Ошибка","Выберите переменную!");
//        return;
//    }

//    if(Saver::areUsure(QString("Вы хотите добавить новое значение  %1 в домен %2 ?").arg("'"+newVal+"'").arg("'"+sDomain+"'")))
//    {
//        QModelIndex domainIndex = DataModels::instance()->domainModel->indexByName(sDomain);
//        QModelIndex newValIndex = DataModels::instance()->domainModel->addValue(domainIndex);

//        DataModels::instance()->domainModel->setData(newValIndex,cmbVal->currentText().trimmed(),Qt::EditRole);
//    }
}
