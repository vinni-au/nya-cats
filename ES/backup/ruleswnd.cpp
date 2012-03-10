#include "ruleswnd.h"

RulesWnd::RulesWnd(QWidget *parent) :
    QMainWindow(parent)
{
    splMain=new QSplitter(Qt::Vertical);
    splMain->setChildrenCollapsible(false);

    //верх
    grbListRules=new QGroupBox();
    //wgtListRules=new QWidget();
    layListRules=new QHBoxLayout();
    lbListRules=new QListWidget();
    layBtnsListRules=new QVBoxLayout();
    btnAddRule=new QPushButton();
    btnDeleteRule=new QPushButton();
    btnUpRule=new QPushButton();
    btnDownRule=new QPushButton();

    layBtnsListRules->addWidget(btnAddRule);
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
    lbIf=new QListWidget();
    layBtnsIf=new QVBoxLayout();
    btnAddIf=new QPushButton();
    btnEditIf=new QPushButton();
    btnDeleteIf=new QPushButton();
    btnUpIf=new QPushButton();
    btnDownIf=new QPushButton();

    grbThen=new QGroupBox();
    layThen=new QHBoxLayout();
    lbThen=new QListWidget();
    layBtnsThen=new QVBoxLayout();
    btnAddThen=new QPushButton();
    btnEditThen=new QPushButton();
    btnDeleteThen=new QPushButton();
    btnUpThen=new QPushButton();
    btnDownThen=new QPushButton();

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

    wgtRuleDetails->setLayout(layRuleDetails);
    //
    splMain->addWidget(grbListRules);
    splMain->addWidget(wgtRuleDetails);

    this->setCentralWidget(splMain);

    this->retranslate();
}
void
RulesWnd::retranslate()
{
    this->setWindowTitle(tr("Правила"));
    grbListRules->setTitle(tr("Правила"));
    btnAddRule->setText(tr("Добавить"));
    btnDeleteRule->setText(tr("Удалить"));
    btnUpRule->setText(tr("Вверх"));
    btnDownRule->setText(tr("Вниз"));

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
