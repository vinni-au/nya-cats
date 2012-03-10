#include "domainwnd.h"
#include <QDebug>

DomainWnd::DomainWnd(QWidget *parent) :
    QMainWindow(parent)
{
    splitter=new QSplitter(this);
    splitter->setOrientation(Qt::Horizontal);
    splitter->setChildrenCollapsible(false);
    splitter->setMidLineWidth(4);

    //левая половина
    hlayDomainName=new QVBoxLayout();
    lbDomainName=new QLabel();
    lwDomainName=new QListWidget();
    btnAddDomain=new QPushButton();
    btnEditDomain=new QPushButton();
    btnDeleteDomain=new QPushButton();
    hlayDomainName->addWidget(lbDomainName);
    hlayDomainName->addWidget(lwDomainName);
    hlayDomainName->addWidget(btnAddDomain);
    hlayDomainName->addWidget(btnEditDomain);
    hlayDomainName->addWidget(btnDeleteDomain);

    wDomainPanel=new QWidget();
    wDomainPanel->setLayout(hlayDomainName);
    splitter->addWidget(wDomainPanel);

    //правая половина
    hlayValues=new QVBoxLayout();
    lbValue=new QLabel();
    lwValues=new QListWidget();
    btnAddValue=new QPushButton();
    btnEditValue=new QPushButton();
    btnDeleteValue=new QPushButton();
    hlayValues->addWidget(lbValue);
    hlayValues->addWidget(lwValues);
    hlayValues->addWidget(btnAddValue);
    hlayValues->addWidget(btnEditValue);
    hlayValues->addWidget(btnDeleteValue);

    wValuePanel=new QWidget();
    wValuePanel->setLayout(hlayValues);
    splitter->addWidget(wValuePanel);
    ///
    this->setCentralWidget(splitter);
    this->retranslate();

    QObject::connect(btnAddDomain,SIGNAL(clicked()),this,SLOT(onAddDomain()));
    QObject::connect(btnEditDomain,SIGNAL(clicked()),this,SLOT(onEditDomain()));
    QObject::connect(btnDeleteDomain,SIGNAL(clicked()),this,SLOT(onDeleteDomain()));
    QObject::connect(btnAddValue,SIGNAL(clicked()),this,SLOT(onAddValue()));
    QObject::connect(btnEditValue,SIGNAL(clicked()),this,SLOT(onEditValue()));
    QObject::connect(btnDeleteValue,SIGNAL(clicked()),this,SLOT(onDeleteValue()));
}

void
DomainWnd::retranslate()
{
    this->setWindowTitle(tr("Домены"));
    //домены
    lbDomainName->setText(tr("Домены"));
    btnAddDomain->setText(tr("Добавить домен"));
    btnEditDomain->setText(tr("Изменить домен"));
    btnDeleteDomain->setText(tr("Удалить домен"));

    //значения
    lbValue->setText(tr("Значения доменов"));
    btnAddValue->setText(tr("Добавить значение"));
    btnEditValue->setText(tr("Изменить значение"));
    btnDeleteValue->setText(tr("Удалить значение"));
}

void
DomainWnd::onAddDomain()
{
    qDebug()<<"onAddDomain()";
}
void
DomainWnd::onEditDomain()
{
}
void
DomainWnd::onDeleteDomain()
{
}

void
DomainWnd::onAddValue()
{
}
void
DomainWnd::onEditValue()
{
}
void
DomainWnd::onDeleteValue()
{
}
