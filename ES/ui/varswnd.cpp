#include "varswnd.h"
#include <QDebug>

VarsWnd::VarsWnd(QWidget *parent) :
    QMainWindow(parent)
{
    wgtMain = new QWidget();
    this->setCentralWidget(wgtMain);
    layVertical = new QVBoxLayout();
    wgtMain->setLayout(layVertical);

    splMain=new QSplitter();
    layVertical->addWidget(splMain);
    splMain->setChildrenCollapsible(false);

    //левая сторона
    wgtLeft=new QWidget();
    layLeft=new QVBoxLayout();

    grbVars=new QGroupBox();
    lswVars=new MyListView();
    layVars=new QVBoxLayout();
    layVars->addWidget(lswVars);
    grbVars->setLayout(layVars);

    layLeft->addWidget(grbVars);
    wgtLeft->setLayout(layLeft);

    splMain->addWidget(wgtLeft);

    //правая сторона
    layRidgt=new QVBoxLayout();
    //группа с кнопками действия
    grbActBtns=new QGroupBox();
    layBtns=new QVBoxLayout();
    btnAddVar=new QPushButton();
    btnEditVar=new QPushButton();
    btnDeleteVar=new QPushButton();
    btnChuprinaEditMode= new QPushButton("Изменить переменную");//ыы
    connect(btnChuprinaEditMode,SIGNAL(clicked()),SLOT(onChuprinaEditModeEnterClick()));
    layBtns->addWidget(btnEditVar);
    layBtns->addWidget(btnAddVar);

    layBtns->addWidget(btnChuprinaEditMode);
    layBtns->addWidget(btnDeleteVar);
    grbActBtns->setLayout(layBtns);
    layRidgt->addWidget(grbActBtns);

    //выводимая-запрашиваемая
    grbType=new QGroupBox();
    layType=new QVBoxLayout();
    chbDerivable=new QCheckBox();
    chbRequested=new QCheckBox();
    layType->addWidget(chbDerivable);
    layType->addWidget(chbRequested);
    grbType->setLayout(layType);

    layRidgt->addWidget(grbType);

    //домен
    grbDomain=new QGroupBox();
    layDomain=new QVBoxLayout();
    cmbDomain=new MyComboBox();


    layDomain->addWidget(cmbDomain);
    grbDomain->setLayout(layDomain);

    layRidgt->addWidget(grbDomain);

    //вопрос
    grbQuestion=new QGroupBox();
    layQuestion=new QVBoxLayout();
    tbQuestion=new QTextEdit();
    layQuestion->addWidget(tbQuestion);
    grbQuestion->setLayout(layQuestion);

    layRidgt->addWidget(grbQuestion);

    wgtRight=new QWidget();
    wgtRight->setLayout(layRidgt);

    splMain->addWidget(wgtRight);
    QList<int> sizes;
    sizes<<200<<100;
    splMain->setSizes(sizes);
    splMain->setStretchFactor(0,1);
    splMain->setStretchFactor(1,1);

    //для режима редактирования
    wgtBtnsEditMode = new QWidget();
    wgtBtnsEditMode->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    layBtnsEditMode = new QHBoxLayout();//кнопки для режима редактирования
    wgtBtnsEditMode->setLayout(layBtnsEditMode);
    btnOkSave   =   new QPushButton("ОК");
    btnCancel   =   new QPushButton("Отмена");
    btnEditMode =   new QPushButton("Редактировать");
    btnClose    =   new QPushButton("Закрыть");
    layBtnsEditMode->addWidget(btnOkSave);
    layBtnsEditMode->addWidget(btnCancel);
    layBtnsEditMode->addWidget(btnEditMode);
    layBtnsEditMode->addWidget(btnClose);
    connect(btnOkSave,SIGNAL(clicked()),SLOT(onOkSaveClick()));
    connect(btnCancel,SIGNAL(clicked()),SLOT(onCancelClick()));
    connect(btnEditMode,SIGNAL(clicked()),SLOT(onEditModeClick()));
    connect(btnClose,SIGNAL(clicked()),SLOT(onCloseClick()));
    layVertical->addWidget(wgtBtnsEditMode);

    //this->setCentralWidget(splMain);
    //this->setLayout(layVertical);
    //this->setLayout();

    this->retranslate();

    connect(btnAddVar,SIGNAL(clicked()),SLOT(onAddClick()));
    connect(btnEditVar,SIGNAL(clicked()),SLOT(onEditClick()));
    connect(btnDeleteVar,SIGNAL(clicked()),SLOT(onDeleteClick()));
    connect(chbDerivable,SIGNAL(toggled(bool)),SLOT(onDerivableChanged(bool)));
    connect(chbRequested,SIGNAL(toggled(bool)),SLOT(onAskingChanged(bool)));
    //connect(cmbDomain,SIGNAL(currentIndexChanged(QString)),SLOT(onDomainChanged(QString)));
    connect(cmbDomain,SIGNAL(activated(QString)),SLOT(onDomainActivated(QString)));
    connect(tbQuestion,SIGNAL(textChanged()),SLOT(onQuestionChanged()));

    ////
 //   QVector<Variable*> *varList = new QVector<Variable*>();

//    Variable *var = new Variable("1");
//    varList->append(var);
//    var = new Variable("2");
//    varList->append(var);
//    var = new Variable("3");
//    varList->append(var);
//    var = new Variable("4");
//    varList->append(var);

 //   DataModels::instance()->varModel->setVariables(varList);

    lswVars->setModel(DataModels::instance()->varModel);

    cmbDomain->setModel(DataModels::instance()->domainModel);

    connect(lswVars,SIGNAL(activated(QModelIndex)),SLOT(onEditClick()));

    //шотакаты
    QAction *actAdd = new QAction(this);
    actAdd->setShortcut(QKeySequence("Ctrl++"));
    connect(actAdd,SIGNAL(triggered()),SLOT(onAddClick()));
    lswVars->addAction(actAdd);

    QAction *actDelete = new QAction(this);
    actDelete->setShortcut(QKeySequence("Ctrl+Delete"));
    connect(actDelete,SIGNAL(triggered()),SLOT(onDeleteClick()));
    lswVars->addAction(actDelete);


    connect(cmbDomain,SIGNAL(currentIndexChanged(QString)),SLOT(onDomainChanged(QString)));
    connect(lswVars,SIGNAL(currentItemChanged(QModelIndex)),SLOT(onVarChanged(QModelIndex)));

    //lswVars->selectionModel()->setCurrentIndex(DataModels::instance()->varModel->index(0,1,QModelIndex()),QItemSelectionModel::ClearAndSelect);

    onDataLoaded();
    editStop();
    connect(DataModels::instance(),SIGNAL(sigDataLoaded()),SLOT(onDataLoaded()));


    //контекстное пополнение
    connect(cmbDomain,SIGNAL(sigNewValueEntered(QString)),SLOT(onContextDomainAdded(QString)));

    chuprinaEditMode(false);
}
void
VarsWnd::retranslate()
{
    this->setWindowTitle(tr("Переменные"));
    grbVars->setTitle(tr("Переменные"));
    btnAddVar->setText(tr("Добавить переменную"));
    btnEditVar->setText(tr("Изменить имя переменной"));
    btnDeleteVar->setText(tr("Удалить переменную"));
    grbActBtns->setTitle(tr("Действия"));
    grbType->setTitle(tr("Тип"));
    chbDerivable->setText(tr("Выводимая"));
    chbRequested->setText(tr("Запрашиваемая"));
    grbDomain->setTitle(tr("Домен"));
    grbQuestion->setTitle(tr("Вопрос"));


}

void
VarsWnd::onAddClick()
{
    qDebug()<<"VarsWnd::onAddClick";
    if(!isEditMode())
        return;

    QModelIndex index = DataModels::instance()->varModel->addVariable();
    lswVars->edit(index);
    lswVars->selectionModel()->setCurrentIndex(index,QItemSelectionModel::ClearAndSelect);
    lswVars->scrollTo(index);

    chbDerivable->setChecked(true);//по-умолчанию - выводимая
    chbRequested->setChecked(false);
}
void
VarsWnd::onEditClick()
{
    qDebug()<<"VarsWnd::onEditClick";
    if(!isEditMode())
        return;
    if(!Saver::areUsure("Вы действительно хотите изменить имя переменной? \nИмя переменной будет изменено во всех правилах, где она встречается. \nПродолжить?"))
        return;
    QModelIndex index = lswVars->currentIndex();
    lswVars->edit(index);
}
void
VarsWnd::onDeleteClick()
{
    qDebug()<<"VarsWnd::onDeleteClick";
    if(!isEditMode())
        return;
    if(!Saver::areUsure("Вы действительно хотите удалить переменную? \nБудут удалены посылки и заключени правил, в которых присутствует это переменная. \nПродолжить?"))
        return;
    QModelIndex index = lswVars->currentIndex();
    DataModels::instance()->varModel->deleteVariable(index);
}

//0-asking
//1-deriv
//2-domain
//3-quest
void
VarsWnd::onDerivableChanged(bool checked)
{
    qDebug()<<"VarsWnd::onDerivableChanged";
    if(checked == false)
    {
        if(chbRequested->isChecked() == false)
            chbRequested->setChecked(true);
    }


    QModelIndex index = lswVars->currentIndex();
    if(index.isValid())
        DataModels::instance()->varModel->setData(DataModels::instance()->varModel->index(1,0,  index)  ,checked,Qt::EditRole);
}
void
VarsWnd::onAskingChanged(bool checked)
{
    qDebug()<<"VarsWnd::onAskingChanged";

    if(checked == false)
    {
        if(chbDerivable->isChecked() == false)
            chbDerivable->setChecked(true);
    }
    QModelIndex index = lswVars->currentIndex();
    if(index.isValid())
        DataModels::instance()->varModel->setData(DataModels::instance()->varModel->index(0,0,  index)  ,checked,Qt::EditRole);
}
void
VarsWnd::onDomainChanged(QString str)
{
    qDebug()<<"VarsWnd::onDomainChanged:"<<str<<" "<<cmbDomain->currentText();

    //if(Saver::areUsure("Вы действительно хотите сменить домен?"))
    {
        QModelIndex index = lswVars->currentIndex();
        if(index.isValid())
            DataModels::instance()->varModel->setData(DataModels::instance()->varModel->index(2,0,  index)  ,cmbDomain->currentText(),Qt::EditRole);
    }
//    else
//    {
//        cmbDomain->blockSignals(true);
//        cmbDomain->selectPrevious();
//        cmbDomain->blockSignals(false);
//    }
}
void
VarsWnd::onQuestionChanged()
{
    qDebug()<<"VarsWnd::onQuestionChanged";
    QModelIndex index = lswVars->currentIndex();
    if(index.isValid())
        DataModels::instance()->varModel->setData(DataModels::instance()->varModel->index(3,0,  index)  ,tbQuestion->toPlainText(),Qt::EditRole);
}
void
VarsWnd::onDomainActivated(QString str)
{
    qDebug()<<"VarsWnd::onDomainActivated "<<str;
}
void
VarsWnd::onVarChanged(QModelIndex index)
{
    qDebug()<<"VarsWnd::onVarChanged";
    VarModel *vm = DataModels::instance()->varModel;
    QModelIndex iDerivable = vm->index(1,0, index);
    QModelIndex iAsking = vm->index(0,0, index);
    QModelIndex iDomain = vm->index(2,0, index);
    QModelIndex iQuestion = vm->index(3,0, index);
    this->chbDerivable->setChecked(     vm->data(iDerivable,Qt::DisplayRole).toBool()    );
    this->chbRequested->setChecked(     vm->data(iAsking,Qt::DisplayRole).toBool()      );
    QString domainStr = vm->data(iDomain,Qt::DisplayRole).toString();
    QModelIndex domInx = DataModels::instance()->domainModel->indexByName(domainStr);
    this->cmbDomain->setCurrentIndex(domInx.row());
    this->tbQuestion->setPlainText(     vm->data(iQuestion,Qt::DisplayRole).toString()   );

    chuprinaEditMode(false);
}

void
VarsWnd::editStart()
{
    if(editMode)
        return;
    editMode = true;

    lswVars->setDragEnabled(true);
    DataModels::instance()->varModel->setItemsIsEditable(true);

    grbActBtns->setVisible(true);

    grbType->setEnabled(true);
    grbDomain->setEnabled(true);
    grbQuestion->setEnabled(true);

   //
    btnEditMode->setVisible(false);
    btnOkSave->setVisible(true);
    btnCancel->setVisible(true);
}

void
VarsWnd::editStop()
{

    editMode = false;

    lswVars->setDragEnabled(false);
    DataModels::instance()->varModel->setItemsIsEditable(false);

    grbActBtns->setVisible(false);

    grbType->setEnabled(false);
    grbDomain->setEnabled(false);
    grbQuestion->setEnabled(false);

   //
    btnEditMode->setVisible(true);
    btnOkSave->setVisible(false);
    btnCancel->setVisible(false);
}

void
VarsWnd::closeEvent(QCloseEvent *event)
{
    qDebug()<<"VarsWnd::closeEvent";
    if(isEditMode())
    {
        maybeClose();
        if(isEditMode())
        {
            event->ignore();
            return;
        }
    }
    event->accept();
    emit sigClosed();
}

void
VarsWnd::onDataLoaded()
{
    QModelIndex firstVarIndex = DataModels::instance()->varModel->index(0,0,QModelIndex());
    lswVars->selectionModel()->setCurrentIndex(firstVarIndex,QItemSelectionModel::ClearAndSelect);
}

void
VarsWnd::onOkSaveClick()
{
    if(DataModels::instance()->isValid())
    {
        DataModels::instance()->save();
        editStop();
    }
    else
    {
        QMessageBox::warning(this,"Ошибка при сохранении!","Неверные данные!");
    }
}

void
VarsWnd::onCancelClick()
{
    if(DataModels::instance()->reload())
    {
        editStop();
    }
    else
        QMessageBox::critical(this,"Ошибка!","Не удалось откатить!");
}

void
VarsWnd::onEditModeClick()
{
    editStart();
    chuprinaEditMode(false);
}

void
VarsWnd::onCloseClick()
{
    this->close();
}

void
VarsWnd::maybeClose()
{
    if(Saver::mayBeSave())//пытаемся сохранить
    {
        if(DataModels::instance()->saved())//нажал сохранить
        {
            editStop();
        }
        else
        {
            onCancelClick();
        }
    }
}

bool
VarsWnd::isEditMode()
{
    return editMode;
}

//контесктное пополнение

void
VarsWnd::onContextDomainAdded(QString newDomain)
{
    qDebug()<<"VarsWnd::onContextDomainAdded: "<<newDomain;
    //QMessageBox::warning(this,"Добавление домена",newDomain);

    if(Saver::areUsure(QString("Вы действительно хотите добавить новый домен %1 ?").arg("'"+newDomain.trimmed()+"'")))
    {
        QModelIndex domainIndex = DataModels::instance()->domainModel->addDomain();
        DataModels::instance()->domainModel->setData(domainIndex,newDomain.trimmed(),Qt::EditRole);
    }
}

void VarsWnd::chuprinaEditMode(bool enter)
{
    grbType->setEnabled(enter);
    grbDomain->setEnabled(enter);
    grbQuestion->setEnabled(enter);
}

void VarsWnd::onChuprinaEditModeEnterClick()
{
    chuprinaEditMode(true);
}
