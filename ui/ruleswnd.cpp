#include "ruleswnd.h"
#include <QDebug>

#include "saver.h"

RulesWnd::RulesWnd(NProduction *production,QWidget *parent) :
    QMainWindow(parent)
{
    this->ruleModel = production->getModel();

    splMain=new QSplitter(Qt::Vertical);
    splMain->setChildrenCollapsible(false);

    //верх
    grbListRules=new QGroupBox();
    //wgtListRules=new QWidget();
    layListRules=new QHBoxLayout();
    lbListRules=new MyListView();
    layBtnsListRules=new QVBoxLayout();
    btnAddRule=new QPushButton();
    btnEditRule=new QPushButton();
    btnDeleteRule=new QPushButton();

    btnUpRule=new QPushButton();
    btnUpRule->setVisible(false);//надо будет убрать,а пока что скроем с глаз моих
    btnDownRule=new QPushButton();
    btnDownRule->setVisible(false);

    wgtBtnsEditMode = new QWidget();
    wgtBtnsEditMode->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    layBtnsEditMode = new QHBoxLayout();//кнопки для режима редактирования
    wgtBtnsEditMode->setLayout(layBtnsEditMode);
    btnOkSave   =   new QPushButton();
    btnCancel   =   new QPushButton();
    btnEditMode =   new QPushButton();
    btnClose    =   new QPushButton();

    layBtnsEditMode->addWidget(btnOkSave);
    layBtnsEditMode->addWidget(btnCancel);
    layBtnsEditMode->addWidget(btnEditMode);
    layBtnsEditMode->addWidget(btnClose);

    layBtnsListRules->addWidget(btnAddRule);
    layBtnsListRules->addWidget(btnEditRule);
    layBtnsListRules->addWidget(btnDeleteRule);
    layBtnsListRules->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Maximum,QSizePolicy::MinimumExpanding));
    layBtnsListRules->addWidget(btnUpRule);
    layBtnsListRules->addWidget(btnDownRule);

    layListRules->addWidget(lbListRules);
    layListRules->addLayout(layBtnsListRules);


    //wgtListRules->setLayout(layListRules);

    grbListRules->setLayout(layListRules);

    //низ
    wgtRuleDetails=new QWidget();
    layRuleDetails=new QVBoxLayout();
    layNameAndReason=new QHBoxLayout();
    ltbNameRule=new LabeledTextBox();
    ltbReason=new LabeledTextBox();
    splIfThen=new QSplitter();
     splIfThen->setChildrenCollapsible(false);
    grbIf=new QGroupBox();
    layIf=new QHBoxLayout();
    lbIf=new MyListView();
    layBtnsIf=new QVBoxLayout();
    btnAddIf=new QPushButton();
    btnEditIf=new QPushButton();
    btnDeleteIf=new QPushButton();
    btnUpIf=new QPushButton();
    btnUpIf->setVisible(false);
    btnDownIf=new QPushButton();
    btnDownIf->setVisible(false);

    grbThen=new QGroupBox();
    layThen=new QHBoxLayout();
    lbThen=new MyListView();
    layBtnsThen=new QVBoxLayout();
    btnAddThen=new QPushButton();
    btnEditThen=new QPushButton();
    btnDeleteThen=new QPushButton();
    btnUpThen=new QPushButton();
    btnUpThen->setVisible(false);
    btnDownThen=new QPushButton();
    btnDownThen->setVisible(false);

    layBtnsIf->addWidget(btnAddIf);
    layBtnsIf->addWidget(btnEditIf);
    layBtnsIf->addWidget(btnDeleteIf);
    layBtnsIf->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Maximum,QSizePolicy::MinimumExpanding));
    layBtnsIf->addWidget(btnUpIf);
    layBtnsIf->addWidget(btnDownIf);

    layIf->addWidget(lbIf);
    layIf->addLayout(layBtnsIf);

    grbIf->setLayout(layIf);
    //
    layBtnsThen->addWidget(btnAddThen);
    layBtnsThen->addWidget(btnEditThen);
    layBtnsThen->addWidget(btnDeleteThen);
    layBtnsThen->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Maximum,QSizePolicy::MinimumExpanding));
    layBtnsThen->addWidget(btnUpThen);
    layBtnsThen->addWidget(btnDownThen);

    layThen->addWidget(lbThen);
    layThen->addLayout(layBtnsThen);

    grbThen->setLayout(layThen);
    //
    splIfThen->addWidget(grbIf);
    splIfThen->addWidget(grbThen);
    //
    layNameAndReason->addWidget(ltbNameRule);
    layNameAndReason->addWidget(ltbReason);

    layRuleDetails->addLayout(layNameAndReason);
    layRuleDetails->addWidget(splIfThen);
    layRuleDetails->addWidget(wgtBtnsEditMode);

    wgtRuleDetails->setLayout(layRuleDetails);
    //
    splMain->addWidget(grbListRules);
    splMain->addWidget(wgtRuleDetails);


    this->setCentralWidget(splMain);

    this->retranslate();
    //модели
    this->lbListRules->setModel(this->ruleModel);


   // QVector<Rule*> *rules = new QVector<Rule*>();
//    Rule *rule=new Rule();
//    rule->name="Rule1";
//    rule->reason="reason1";
//        Expr *pred=new Expr();
//        pred->var="var";
//        pred->sign="=";
//        pred->val="val";
//    rule->predicate.append(pred);

//        pred=new Expr();
//        pred->var="var2";
//        pred->sign="=";
//        pred->val="val2";
//    rule->predicate.append(pred);

//        pred=new Expr();
//        pred->var="var3";
//        pred->sign="!=";
//        pred->val="val3";
//    rule->predicate.append(pred);

//        Expr *conc=new Expr();
//        conc->var="var_concl";
//        conc->sign="=";
//        conc->val="true";
//    rule->conclusion.append(conc);

//    rules->append(rule);

//    rule=new Rule();
//    rule->name="Rule2";
//    rule->reason="reason2";
//    rules->append(rule);

//    rule=new Rule();
//    rule->name="Rule3";
//    rules->append(rule);

   // DataModels::instance()->ruleModel->setRules(rules);

    //Сигналы
    connect(lbListRules,SIGNAL(currentItemChanged(QModelIndex)),SLOT(onCurrentRuleChanged(QModelIndex)));
    connect(ltbNameRule,SIGNAL(textEdited(QString)),SLOT(onRuleNameChanged(QString)));
    connect(ltbReason,SIGNAL(textEdited(QString)),SLOT(onRuleReasonChanged(QString)));

    connect(btnAddRule,SIGNAL(clicked()),SLOT(onAddRuleClick()));//rule
    connect(btnEditRule,SIGNAL(clicked()),SLOT(onEditRuleClick()));
    connect(btnEditMode,SIGNAL(clicked()),SLOT(onEditModeClick()));
    connect(btnDeleteRule,SIGNAL(clicked()),SLOT(onDeleteRuleClick()));
    connect(btnClose,SIGNAL(clicked()),SLOT(onCloseClick()));
    connect(btnUpRule,SIGNAL(clicked()),SLOT(onMoveUpRuleClick()));

    connect(btnOkSave,SIGNAL(clicked()),SLOT(onOkSaveClick()));
    connect(btnCancel,SIGNAL(clicked()),SLOT(onCancelClick()));

    connect(btnAddIf,SIGNAL(clicked()),SLOT(onAddIfClick()));//if
    connect(btnEditIf,SIGNAL(clicked()),SLOT(onEditIfClick()));
    connect(btnDeleteIf,SIGNAL(clicked()),SLOT(onDelIfClick()));

    connect(btnAddThen,SIGNAL(clicked()),SLOT(onAddThenClick()));//then
    connect(btnEditThen,SIGNAL(clicked()),SLOT(onEditThenClick()));
    connect(btnDeleteThen,SIGNAL(clicked()),SLOT(onDelThenClick()));

    connect(lbIf,SIGNAL(activated(QModelIndex)),SLOT(onEditIfClick())); //двойной щелчек или ентер на условии
    connect(lbThen,SIGNAL(activated(QModelIndex)),SLOT(onEditThenClick()));

    //шотакаты
    QAction *actAdd = new QAction(this);
    actAdd->setShortcut(QKeySequence("Ctrl++"));
    connect(actAdd,SIGNAL(triggered()),SLOT(onShotcutAdd()));
    lbListRules->addAction(actAdd);

    QAction *actDelete = new QAction(this);
    actDelete->setShortcut(QKeySequence("Ctrl+Delete"));
    connect(actDelete,SIGNAL(triggered()),SLOT(onShotcutDelete()));
    lbListRules->addAction(actDelete);

    QShortcut *shortcutSwitch=new QShortcut(QKeySequence("Ctrl+*"),this);
    connect(shortcutSwitch,SIGNAL(activated()),SLOT(onShortcutSwitch()));


    onDataLoaded();//начальное выделение

    editStop();
//!    connect(DataModels::instance(),SIGNAL(sigDataLoaded()),SLOT(onDataLoaded()));

}

RulesWnd::~RulesWnd()
{
}

void
RulesWnd::retranslate()
{
    this->setWindowTitle(tr("Правила"));
    grbListRules->setTitle(tr("Правила"));
    btnAddRule->setText(tr("Добавить правило"));
    btnEditRule->setText(tr("Изменить правило"));
    btnDeleteRule->setText(tr("Удалить правило"));

    btnUpRule->setText(tr("Вверх"));
    btnDownRule->setText(tr("Вниз"));

    btnEditMode->setText(tr("Редактировать"));
    btnClose->setText(tr("Закрыть"));
    btnOkSave->setText("OK");
    btnCancel->setText("Отмена");

    ltbNameRule->lbLabel->setText(tr("Имя правила"));
    ltbReason->lbLabel->setText(tr("Объяснение"));

    grbIf->setTitle(tr("Посылка"));
    btnAddIf->setText(tr("Добавить"));
    btnEditIf->setText(tr("Изменить"));
    btnDeleteIf->setText(tr("Удалить"));
    btnUpIf->setText(tr("Вверх"));
    btnDownIf->setText(tr("Вниз"));

    grbThen->setTitle(tr("Заключение"));
    btnAddThen->setText(tr("Добавить"));
    btnEditThen->setText(tr("Изменить"));
    btnDeleteThen->setText(tr("Удалить"));
    btnUpThen->setText(tr("Вверх"));
    btnDownThen->setText(tr("Вниз"));
}

void
RulesWnd::onCurrentRuleChanged(QModelIndex index)
{
    chuprinaEditMode(false);
    qDebug()<<"RulesWnd::onCurrentRuleChanged";
    RuleModel *rm=this->ruleModel;

    if(!index.isValid())
        return;

    this->ltbNameRule->setText(rm->getRuleName(index));

    QModelIndex iReason = rm->index(0,0,index);

    ltbReason->setText(rm->data(iReason,Qt::DisplayRole).toString());

    QModelIndex iIfHead = rm->index(1,0,index);
    lbIf->setModel(rm);
    lbIf->setRootIndex(iIfHead);

    QModelIndex iThenHead = rm->index(2,0,index);
    lbThen->setModel(rm);
    lbThen->setRootIndex(iThenHead);
}

void
RulesWnd::onRuleNameChanged(QString text)
{
    qDebug()<<"RulesWnd::onRuleNameChanged "<<text;

    QModelIndex index = this->lbListRules->currentIndex();
    RuleModel *rm=(RuleModel*)(index.model());
    if(rm==NULL)
        return;

    rm->setData(index,ltbNameRule->getText(),Qt::EditRole);
}

void
RulesWnd::onRuleReasonChanged(QString text)
{
    qDebug()<<"RulesWnd::onRuleReasonChanged "<<text;
    QModelIndex index = this->lbListRules->currentIndex();
    RuleModel *rm=(RuleModel *)(index.model());
    if(rm==NULL)
        return;
    QModelIndex reasonIndex = rm->index(0,0,index);
    rm->setData(reasonIndex,ltbReason->getText(),Qt::EditRole);
}

void
RulesWnd::onAddRuleClick()
{
    qDebug()<<"RulesWnd::onAddRuleClick()";
    //QModelIndex selRuleIndex = DataModels::instance()->ruleModel->addRule();
    QModelIndex selRuleIndex = this->ruleModel->insertRule(lbListRules->currentIndex());
    lbListRules->selectionModel()->setCurrentIndex(selRuleIndex,QItemSelectionModel::ClearAndSelect);
    lbListRules->scrollTo(selRuleIndex);
    ltbNameRule->tbTextBox->setFocus();
    ltbNameRule->tbTextBox->selectAll();
    chuprinaEditMode(true);
}

void
RulesWnd::onDeleteRuleClick()
{
    qDebug()<<"RulesWnd::onDeleteRuleClick()";
    QModelIndex index = lbListRules->currentIndex();
    this->ruleModel->deleteRule(index);
}

void
RulesWnd::onAddIfClick()
{
    qDebug()<<"RulesWnd::onAddIfClick()";
    ExprEditor exprEditor(ExprEditor::IfExpr,ExprEditor::AddAction);
    do
    {
        if(exprEditor.exec())
        {
            QModelIndex ruleIndex = lbListRules->currentIndex();
            QModelIndex index = this->ruleModel->addIf(ruleIndex);
            Expr *expr = exprEditor.getExpr();
            this->ruleModel->setData(index,reinterpret_cast<unsigned long long>(expr),Qt::EditRole);
        }
    }
    while(exprEditor.startAgain);
}

void
RulesWnd::onEditIfClick()
{
    qDebug()<<"RulesWnd::onEditIfClick";
    QModelIndex ifIndex = lbIf->currentIndex();
    if(!ifIndex.isValid())
        return;
    ExprEditor exprEditor(ExprEditor::IfExpr,ExprEditor::EditAction);
    //получаем выражение по частям
    QString sVar = this->ruleModel->getExprPart(ifIndex,RuleModel::ExprVar);
    QString sSign = this->ruleModel->getExprPart(ifIndex,RuleModel::ExprSign);
    QString sVal = this->ruleModel->getExprPart(ifIndex,RuleModel::ExprVal);

//!    QModelIndex varIndexInVarModel = DataModels::instance()->varModel->indexByName(sVar);

//!    exprEditor.selectVar(varIndexInVarModel);       //выбираем переменную
    exprEditor.selectSign(sSign);                   //выбираем знак
    exprEditor.selectVal(sVal);                     //выбираем значение

    if(exprEditor.exec())
    {
        Expr *expr = exprEditor.getExpr();
        this->ruleModel->setData(ifIndex,reinterpret_cast<unsigned long long>(expr),Qt::EditRole);
    }
}

void
RulesWnd::onDelIfClick()
{
    qDebug()<<"RulesWnd::onDelIfClick";
    QModelIndex ifIndex = lbIf->currentIndex();
    this->ruleModel->deleteIf(ifIndex);
}

void
RulesWnd::onAddThenClick()
{
    qDebug()<<"RulesWnd::onAddThenClick()";
    ExprEditor exprEditor(ExprEditor::ThenExpr,ExprEditor::AddAction);
    do
    {
        if(exprEditor.exec())
        {
            QModelIndex ruleIndex = lbListRules->currentIndex();
            QModelIndex index = this->ruleModel->addThen(ruleIndex);
            Expr *expr = exprEditor.getExpr();
            this->ruleModel->setData(index,reinterpret_cast<unsigned long long>(expr),Qt::EditRole);
        }
    }
    while(exprEditor.startAgain);
}

void
RulesWnd::onEditThenClick()
{
    qDebug()<<"RulesWnd::onEditThenClick";
    QModelIndex thenIndex = lbThen->currentIndex();
    if(!thenIndex.isValid())
        return;
    ExprEditor exprEditor(ExprEditor::ThenExpr,ExprEditor::EditAction);
    //получаем выражение по частям
    QString sVar = this->ruleModel->getExprPart(thenIndex,RuleModel::ExprVar);
    QString sSign = this->ruleModel->getExprPart(thenIndex,RuleModel::ExprSign);
    QString sVal = this->ruleModel->getExprPart(thenIndex,RuleModel::ExprVal);

//!    QModelIndex varIndexInVarModel = DataModels::instance()->varModel->indexByName(sVar);

 //!   exprEditor.selectVar(varIndexInVarModel);       //выбираем переменную
    exprEditor.selectSign(sSign);                   //выбираем знак
    exprEditor.selectVal(sVal);                     //выбираем значение

    if(exprEditor.exec())
    {
        Expr *expr = exprEditor.getExpr();
        this->ruleModel->setData(thenIndex,reinterpret_cast<unsigned long long>(expr),Qt::EditRole);
    }
}

void
RulesWnd::onDelThenClick()
{
    qDebug()<<"RulesWnd::onDelThenClick";
    QModelIndex thenIndex = lbThen->currentIndex();
    this->ruleModel->deleteThen(thenIndex);
}

#include <QFile>
#include <QTextStream>
void
RulesWnd::onMoveUpRuleClick()
{
    qDebug()<<"RulesWnd::onMoveUpRuleClick";
//    QFile file("D:\\expertSystem.txt");
//    file.open(QIODevice::WriteOnly);

//    QTextStream stream(&file);

//    DataModels::instance()->saveToXml(stream);
//    file.close();
////////////////
//    QFile file1("D:\\expertSystem.txt");
//    file1.open(QIODevice::ReadOnly);

//    DataModels::instance()->readFromXml(file1);
//    file1.close();
}

void
RulesWnd::onShotcutAdd()
{
    qDebug()<<"RulesWnd::onShotcutAdd";
    if(!isEditMode())
        return;
    if(lbListRules->hasFocus() || ltbNameRule->tbTextBox->hasFocus())
    {
        onAddRuleClick();
    }
    else if(lbIf->hasFocus())
    {
        onAddIfClick();
    }
    else if(lbThen->hasFocus())
    {
        onAddThenClick();
    }
}

void
RulesWnd::onShotcutDelete()
{
    qDebug()<<"RulesWnd::onShotcutDelete";
    if(!isEditMode())
        return;
    if(lbListRules->hasFocus() || ltbNameRule->tbTextBox->hasFocus())
    {
        onDeleteRuleClick();
    }
    else if(lbIf->hasFocus())
    {
        onDelIfClick();
    }
    else if(lbThen->hasFocus())
    {
        onDelThenClick();
    }
}

void
RulesWnd::onShortcutSwitch()
{
    qDebug()<<"RulesWnd::onShortcutSwitch";
    if(lbListRules->hasFocus() || ltbNameRule->tbTextBox->hasFocus())
    {
        lbIf->setFocus();
    }
    else if(lbIf->hasFocus())
    {
        lbThen->setFocus();
    }
    else if(lbThen->hasFocus())
    {
        lbListRules->setFocus();
    }
    else
    {
        lbListRules->setFocus();
    }
}

void
RulesWnd::onEditModeClick()
{
    qDebug()<<"RulesWnd::onEditRuleClick";
    editStart();
    chuprinaEditMode(false);
}

void
RulesWnd::onCloseClick()
{
    qDebug()<<"RulesWnd::onCloseClick";
    this->close();
}

void
RulesWnd::maybeClose()
{
    if(Saver::mayBeSave())//пытаемся сохранить
    {
        if(/*DataModels::instance()->saved()*/1)//нажал сохранить
        {
            editStop();
        }
        else
        {
            onCancelClick();
        }
    }
}

void
RulesWnd::editStart()
{
    if(editMode)
        return;
    editMode = true;

    lbListRules->setDragEnabled(true);

    btnAddRule->setVisible(true);
    btnEditRule->setVisible(true);
    btnDeleteRule->setVisible(true);

    //btnUpRule->setVisible(true);
    //btnDownRule->setVisible(true);

    ltbNameRule->setEnabled(true);
    ltbReason->setEnabled(true);

    grbIf->setEnabled(true);
    grbThen->setEnabled(true);

   //
    btnEditMode->setVisible(false);
    btnOkSave->setVisible(true);
    btnCancel->setVisible(true);
    //btnClose->setVisible(true);
}

void
RulesWnd::editStop()
{
    editMode = false;

    lbListRules->setDragEnabled(false);

    btnAddRule->setVisible(false);
    btnEditRule->setVisible(false);
    btnEditMode->setVisible(false);
    btnDeleteRule->setVisible(false);

    //btnUpRule->setVisible(false);
    //btnDownRule->setVisible(false);

    ltbNameRule->setEnabled(false);
    ltbReason->setEnabled(false);

    grbIf->setEnabled(false);
    grbThen->setEnabled(false);

   //
    btnEditMode->setVisible(true);
    btnOkSave->setVisible(false);
    btnCancel->setVisible(false);
    //btnClose->setVisible(true);
}

void
RulesWnd::onOkSaveClick()
{
    if(/*DataModels::instance()->isValid()*/1)
    {
  //!      DataModels::instance()->save();
        editStop();
    }
    else
    {
        QMessageBox::warning(this,"Ошибка при сохранении!","Неверные данные!");
    }
}

void
RulesWnd::onCancelClick()
{
    if(/*DataModels::instance()->reload()*/1)
    {
        editStop();
    }
    else
        QMessageBox::critical(this,"Ошибка!","Не удалось откатить!");
}


void
RulesWnd::onDataLoaded()
{
    QModelIndex firstRuleIndex = this->ruleModel->index(0,0,QModelIndex());
    lbListRules->selectionModel()->setCurrentIndex(firstRuleIndex,QItemSelectionModel::ClearAndSelect);

    QModelIndex iIfHead = this->ruleModel->index(1,0,firstRuleIndex);
    QModelIndex firstIfInx = this->ruleModel->index(0,0,iIfHead);
    if(firstIfInx.isValid())
        lbIf->selectionModel()->setCurrentIndex(firstIfInx,QItemSelectionModel::ClearAndSelect);

    QModelIndex iThenHead = this->ruleModel->index(2,0,firstRuleIndex);
    QModelIndex firstThenInx = this->ruleModel->index(0,0,iThenHead);
    if(firstThenInx.isValid())
        lbThen->selectionModel()->setCurrentIndex(firstThenInx,QItemSelectionModel::ClearAndSelect);
}

bool RulesWnd::event(QEvent *event)
{

    if(event->type() == QEvent::WindowStateChange)
    {
        //event->ignore();
        //qDebug()<<"bool RulesWnd::event(QEvent *event)";
        //QString s;
        //QTextStream stream(&s);
        //stream<<"Rules Window State:"<<this->windowState();

       // QMessageBox::information(this,"",*stream.string());
    }
    return QWidget::event(event);
}

bool
RulesWnd::isEditMode()
{
    return editMode;
}

void
RulesWnd::closeEvent(QCloseEvent *event)
{
    qDebug()<<"RulesWnd::closeEvent";
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

void RulesWnd::chuprinaEditMode(bool enter)
{
    ltbNameRule->setEnabled(enter);
    ltbReason->setEnabled(enter);

    grbIf->setEnabled(enter);
    grbThen->setEnabled(enter);
}

void RulesWnd::onEditRuleClick()
{
    chuprinaEditMode(true);
}
