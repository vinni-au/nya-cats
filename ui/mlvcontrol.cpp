#include "mlvcontrol.h"
#include "ui_mlvcontrol.h"

MLVControl::MLVControl(MLV* mlv, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MLVControl)
  ,m_Mlv(mlv)
{
    ui->setupUi(this);

    connect(mlv, SIGNAL(AddMsgToLog(QString)), SLOT(AddMsg(QString)));
    connect(mlv, SIGNAL(ClearLog()), SLOT(ClearLog()));
}

MLVControl::~MLVControl()
{
    delete ui;
}

void MLVControl::AddMsg(QString msg)
{
    ui->listLog->addItem(msg);
}

void MLVControl::ClearLog()
{
    ui->listLog->clear();
}
