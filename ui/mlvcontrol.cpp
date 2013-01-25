#include "mlvcontrol.h"
#include "ui_mlvcontrol.h"

MLVControl::MLVControl(MLV* mlv, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MLVControl)
  ,m_Mlv(mlv)
  ,m_currentItem(NULL)
{
    ui->setupUi(this);

    //m_frameModel = new NFramenetModel;
    m_frameModel = new NFramenetModelHierarchical;

    ui->treeView->setModel(m_frameModel);

	// line of reasoning
	ui->lineOfReasoning->header()->hide();
	ui->lineOfReasoning->setColumnCount(1);

    connect(mlv, SIGNAL(AddMsgToLog(QString)), SLOT(AddMsg(QString)));
    connect(mlv, SIGNAL(ClearLog()), SLOT(ClearLog()));

	connect(mlv, SIGNAL(LOR_AddRoot(QString)),		SLOT(LOR_AddRoot(QString)));
	connect(mlv, SIGNAL(LOR_SetCurrState(bool)),	SLOT(LOR_SetCurrState(bool)));
	connect(mlv, SIGNAL(LOR_AddNextLevel(QString)),	SLOT(LOR_AddNextLevel(QString)));
	connect(mlv, SIGNAL(LOR_AddCurrLevel(QString)),	SLOT(LOR_AddCurrLevel(QString)));
	connect(mlv, SIGNAL(LOR_GotoPrevLevel()),		SLOT(LOR_GotoPrevLevel()));
	connect(mlv, SIGNAL(LOR_Clear()),				SLOT(LOR_Clear()));
	connect(mlv, SIGNAL(WM_Set(QList<NFrame *>*)),	SLOT(setWorkMemory(QList<NFrame*>*)));
}

MLVControl::~MLVControl()
{
    delete ui;
}

void MLVControl::ClearLog()
{
	ui->listLog->clear();
}

void MLVControl::AddMsg(QString msg)
{
	ui->listLog->addItem(msg);
}

void MLVControl::LOR_AddRoot(QString name)
{
	m_currentItem = new QTreeWidgetItem();
	m_currentItem->setText(0, name);
	ui->lineOfReasoning->addTopLevelItem(m_currentItem);
}

void MLVControl::LOR_AddNextLevel(QString name)
{
	if (m_currentItem == NULL)
	{
		LOR_AddRoot(name);
		return;
	}

	QTreeWidgetItem* childNew = new QTreeWidgetItem();
	childNew->setText(0, name);

	m_currentItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	m_currentItem->addChild(childNew);
	m_currentItem = childNew;

	ui->lineOfReasoning->setItemWidget(childNew, 1, 0);
	ui->lineOfReasoning->expandAll();
}

void MLVControl::LOR_AddCurrLevel(QString name)
{
	if (m_currentItem == NULL || m_currentItem->parent() == NULL)
	{
		LOR_AddRoot(name);
		return;
	}

	LOR_GotoPrevLevel();
	LOR_AddNextLevel(name);
}

void MLVControl::LOR_GotoPrevLevel()
{
	if (m_currentItem == NULL)
		return;

	m_currentItem = m_currentItem->parent();
}

void MLVControl::LOR_SetCurrState(bool bOk)
{
	if (m_currentItem == NULL)
		return;

	QIcon icon(bOk ? ":/mlvcontrol/pics/ok.jpg" : ":/mlvcontrol/pics/fail.jpg");
	m_currentItem->setIcon(0, icon);
}

void MLVControl::LOR_Clear()
{
	ui->lineOfReasoning->clear();
}

void MLVControl::setWorkMemory(QList<NFrame *> *list)
{
//	QList<NFrame *> *listNull = new QList<NFrame *>();
//    QList<NFrame*> toRemove;
//    for (int i = 0; i < list->count(); ++i) {
//        NFrame* frame = (*list)[i];
//        QString name = frame->name.defValue().toString();
//        if (name == "Ячейка игрового поля" ||
//            name == "Пусто" ||
//            name == "Игровое поле")
//        {
//            toRemove << frame;
//        }
//    }
//    for (int i = 0; i < toRemove.count(); ++i)
//        list->removeAll(toRemove.at(i));

//    for (int i = 0; i < list->count(); ++i)
//    {
//        NFrame* frame=list->at(i);
//        //отладочная инфа для имени
//        NSlot* slotName = frame->getSlotByName("name");
//        slotName->getFasetByName("default_value")->setValue( slotName->getFasetByName("default_value")->value().toString()+
//                                                             " (" +QString::number((long long)frame->id())+")" );

//        QStringList asdsd;
//        QStringList subframeNames = frame->getSubframesSlotNames(asdsd);
//        QString subFrameName;
//        foreach(subFrameName,subframeNames)
//        {
//            NSlot* slot = frame->getSlotByName(subFrameName);
//            if(!slot)
//                continue;
//            NFaset* faset = slot->getFasetByName("value");
//            if(!faset)
//                continue;
//            if(faset->value().toString().isEmpty())
//                continue;
//            NFrame* f = ((NFrame*)(faset->value().toLongLong()));
//                        if (f != NULL)
//                                faset->setValue( f->frameName()/* + " (" +QString::number((long long)f)+")"*/ );
//        }
//    }

    m_frameModel->setFrames(list);
    ui->treeView->update();
    ui->treeView->setColumnHidden(3,true);
    ui->treeView->setColumnHidden(4,true);
    ui->treeView->setColumnHidden(5,true);
}
