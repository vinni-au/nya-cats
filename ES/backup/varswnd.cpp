#include "varswnd.h"

VarsWnd::VarsWnd(QWidget *parent) :
    QMainWindow(parent)
{
    splMain=new QSplitter();
    splMain->setChildrenCollapsible(false);

    //левая сторона
    wgtLeft=new QWidget();
    layLeft=new QVBoxLayout();

    grbVars=new QGroupBox();
    lswVars=new QListWidget();
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
    layBtns->addWidget(btnAddVar);
    layBtns->addWidget(btnEditVar);
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
    cmbDomain=new QComboBox();
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

    this->setCentralWidget(splMain);

    this->retranslate();
}
void
VarsWnd::retranslate()
{
    this->setWindowTitle(tr("Переменные"));
    grbVars->setTitle(tr("Переменные"));
    btnAddVar->setText(tr("Добавить"));
    btnEditVar->setText(tr("Изменить"));
    btnDeleteVar->setText(tr("Удалить"));
    grbActBtns->setTitle(tr("Действия"));
    grbType->setTitle(tr("Тип"));
    chbDerivable->setText(tr("Выводимая"));
    chbRequested->setText(tr("Запрашиваемая"));
    grbDomain->setTitle(tr("Домен"));
    grbQuestion->setTitle(tr("Вопрос"));


}
