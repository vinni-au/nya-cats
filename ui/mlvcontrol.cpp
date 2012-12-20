#include "mlvcontrol.h"
#include "ui_mlvcontrol.h"

MLVControl::MLVControl(MLV* mlv, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MLVControl)
  ,m_Mlv(mlv)
{
    ui->setupUi(this);

    m_frameModel = new NFramenetModel;

    ui->treeView->setModel(m_frameModel);

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

void MLVControl::setWorkMemory(QList<NFrame *> *list)
{
    QList<NFrame*> toRemove;
    for (int i = 0; i < list->count(); ++i) {
        NFrame* frame = (*list)[i];
        QString name = frame->name.defValue().toString();
        if (name == "Ячейка игрового поля" ||
            name == "Пусто" ||
            name == "Игровое поле")
        {
            toRemove << frame;
        }
    }
    for (int i = 0; i < toRemove.count(); ++i)
        list->removeAll(toRemove.at(i));

    for (int i = 0; i < list->count(); ++i)
    {
        NFrame* frame=list->at(i);
        //отладочная инфа для имени
        NSlot* slotName = frame->getSlotByName("name");
        slotName->getFasetByName("default_value")->setValue( slotName->getFasetByName("default_value")->value().toString()+
                                                             " (" +QString::number((long long)frame->id())+")" );

        QStringList asdsd;
        QStringList subframeNames = frame->getSubframesSlotNames(asdsd);
        QString subFrameName;
        foreach(subFrameName,subframeNames)
        {
            NSlot* slot = frame->getSlotByName(subFrameName);
            if(!slot)
                continue;
            NFaset* faset = slot->getFasetByName("value");
            if(!faset)
                continue;
            if(faset->value().toString().isEmpty())
                continue;
            NFrame* f = ((NFrame*)(faset->value().toLongLong()));
			if (f != NULL)
				faset->setValue( f->frameName()/* + " (" +QString::number((long long)f)+")"*/ );
        }
    }

    m_frameModel->setFrames(list);
    ui->treeView->update();
    ui->treeView->setColumnHidden(3,true);
    ui->treeView->setColumnHidden(4,true);
    ui->treeView->setColumnHidden(5,true);
}
