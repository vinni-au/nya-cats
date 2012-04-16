#include "domainwnd.h"
#include <QDebug>
#include <QStringList>


DomainWnd::DomainWnd(NKBManager *kbManager,QWidget *parent) :
    QMainWindow(parent)
{

    setModel(kbManager->getDomainModel());
    wgtMain = new QWidget();
    this->setCentralWidget(wgtMain);
    layVertical = new QVBoxLayout();
    wgtMain->setLayout(layVertical);

    splitter=new QSplitter(this);
    splitter->setOrientation(Qt::Horizontal);
    splitter->setChildrenCollapsible(false);
    splitter->setMidLineWidth(4);
    layVertical->addWidget(splitter);

    //левая половина
    hlayDomainName=new QVBoxLayout();
    lbDomainName=new QLabel();
    lwDomainName=new MyListView();
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
    lwValues=new MyListView();
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
    //this->setCentralWidget(splitter);

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

    this->retranslate();

    QObject::connect(btnAddDomain,SIGNAL(clicked()),this,SLOT(onAddDomain()));
    QObject::connect(btnEditDomain,SIGNAL(clicked()),this,SLOT(onEditDomain()));
    QObject::connect(btnDeleteDomain,SIGNAL(clicked()),this,SLOT(onDeleteDomain()));
    QObject::connect(btnAddValue,SIGNAL(clicked()),this,SLOT(onAddValue()));
    QObject::connect(btnEditValue,SIGNAL(clicked()),this,SLOT(onEditValue()));
    QObject::connect(btnDeleteValue,SIGNAL(clicked()),this,SLOT(onDeleteValue()));

    connect(lwDomainName,SIGNAL(currentItemChanged(QModelIndex)),SLOT(onDomainSelected(QModelIndex)));

    this->lwDomainName->setModel(this->m_domainModel);
    this->lwValues->setModel(m_domainModel);

    connect(lwDomainName,SIGNAL(activated(QModelIndex)),SLOT(onEditDomain()));
    connect(lwValues,SIGNAL(activated(QModelIndex)),SLOT(onEditValue()));

    //shortcuts
    QAction *actAdd = new QAction(this);
    actAdd->setShortcut(QKeySequence("Ctrl++"));
    connect(actAdd,SIGNAL(triggered()),SLOT(onShortcutAdd()));
    lwValues->addAction(actAdd);

    QAction *actDelete = new QAction(this);
    actDelete->setShortcut(QKeySequence("Ctrl+Delete"));
    connect(actDelete,SIGNAL(triggered()),SLOT(onShortcutDelete()));
    lwValues->addAction(actDelete);

    QShortcut *shortcutSwitch = new QShortcut(QKeySequence("Ctrl+*"),this);
    connect(shortcutSwitch,SIGNAL(activated()),SLOT(onShortcutSwitch()));
            /////////////////////

//    QVector< Domain*> *domains=new  QVector< Domain*>();
//    Domain *domain=new Domain("Логический");
//    domain->values<<"Правда"<<"Ложь";
//    *domains << domain;


//    domain=new Domain("Числовой");
//    domain->values<<"Один"<<"Два"<<"Три";
//    *domains << domain;
///    DataModels::instance()->domainModel->setDomains(domains);
///////////////////////


    lwDomainName->selectionModel()->setCurrentIndex(m_domainModel->index(0,1,QModelIndex()),QItemSelectionModel::ClearAndSelect);

    onDataLoaded();
    editStop();
 //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   connect(DataModels::instance(),SIGNAL(sigDataLoaded()),SLOT(onDataLoaded()));
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

    QModelIndex index = m_domainModel->addDomain();
    lwDomainName->edit(index);
    lwDomainName->selectionModel()->setCurrentIndex(index,QItemSelectionModel::ClearAndSelect);
    lwDomainName->scrollTo(index);

}

void
DomainWnd::onEditDomain()
{
    qDebug()<<"DomainWnd::onEditDomain()";
    if(!isEditMode())
        return;

    QModelIndex index = lwDomainName->currentIndex();
    lwDomainName->edit(index);
}

void
DomainWnd::onDeleteDomain()
{
    qDebug()<<"DomainWnd::onEditDomain()";
    if(!isEditMode())
        return;
    if(!m_kbManager->areUsure("Вы действительно хотите удалить домен? \nБудут удалены все переменные этого домена. \nПродолжить?"))
        return;

    QModelIndex index = lwDomainName->currentIndex();
    if(index.isValid())
        m_domainModel->deleteDomain(index);
}

void
DomainWnd::onAddValue()
{
    qDebug()<<"DomainWnd::onAddValue()";
    if(!isEditMode())
        return;
    QModelIndex domaindIndex=lwDomainName->currentIndex();
    QModelIndex valueIndex = m_domainModel->addValue(domaindIndex);
    lwValues->edit(valueIndex);
    lwValues->selectionModel()->setCurrentIndex(valueIndex,QItemSelectionModel::ClearAndSelect);
    lwValues->scrollTo(valueIndex);
}

void
DomainWnd::onEditValue()
{
    qDebug()<<"DomainWnd::onEditValue())";
    if(!isEditMode())
        return;
    if(!m_kbManager->areUsure("Вы действительно хотите изменить значение? \nБудут изменены значения во всех правилах. \nПродолжить?"))
        return;

    QModelIndex valueIndex = lwValues->currentIndex();
    lwValues->edit(valueIndex);
}

void
DomainWnd::onDeleteValue()
{
    qDebug()<<"DomainWnd::onDeleteValue()";
    if(!isEditMode())
        return;
    if(!m_kbManager->areUsure("Вы действительно удалить значение значение? \nБудут удалены все посылки правил, где встерчается это значение. \nПродолжить?"))
        return;

    QModelIndex valueIndex = lwValues->currentIndex();
    m_domainModel->deleteValue(valueIndex);
}

void
DomainWnd::onDomainSelected(QModelIndex index)
{
    qDebug()<<"DomainWnd::onDomainSelected(QModelIndex index)";
   // QModelIndex valIndex=DataModels::instance()->domainModel->index(0,0,index);
    if(index.isValid() && index.model()->data(index)!= "root")
    {
        lwValues->setRootIndex(index);
    }
}

void
DomainWnd::onShortcutAdd()
{
    qDebug()<<"DomainWnd::onShortcutAdd";
    if(!isEditMode())
        return;

    if(lwDomainName->hasFocus())
    {
        onAddDomain();
    }
    if(lwValues->hasFocus())
    {
        onAddValue();
    }
}

void
DomainWnd::onShortcutDelete()
{
    qDebug()<<"DomainWnd::onShortcutDelete";
    if(!isEditMode())
        return;

    if(lwDomainName->hasFocus())
    {
        onDeleteDomain();
    }
    if(lwValues->hasFocus())
    {
        onDeleteValue();
    }
}

void
DomainWnd::onShortcutSwitch()
{
    qDebug()<<"DomainWnd::onShortcutSwitch";
    if(lwDomainName->hasFocus())
    {
        lwValues->setFocus();
    }
    else if(lwValues->hasFocus())
    {
        lwDomainName->setFocus();
    }
    else
    {
        lwDomainName->setFocus();
    }
}

void
DomainWnd::editStart()
{
    if(editMode)
        return;
    editMode = true;

    lwDomainName->setDragEnabled(true);
    m_domainModel->setItemsIsEditable(true);
    lwValues->setDragEnabled(true);

    btnAddDomain->setVisible(true);
    btnEditDomain->setVisible(true);
    btnDeleteDomain->setVisible(true);
    btnAddValue->setVisible(true);
    btnEditValue->setVisible(true);
    btnDeleteValue->setVisible(true);
   //
    btnEditMode->setVisible(false);
    btnOkSave->setVisible(true);
    btnCancel->setVisible(true);
}

void
DomainWnd::editStop()
{

    editMode = false;

    lwDomainName->setDragEnabled(false);
    m_domainModel->setItemsIsEditable(false);
    lwValues->setDragEnabled(false);

    btnAddDomain->setVisible(false);
    btnEditDomain->setVisible(false);
    btnDeleteDomain->setVisible(false);
    btnAddValue->setVisible(false);
    btnEditValue->setVisible(false);
    btnDeleteValue->setVisible(false);

   //
    btnEditMode->setVisible(true);
    btnOkSave->setVisible(false);
    btnCancel->setVisible(false);
}

void
DomainWnd::closeEvent(QCloseEvent *event)
{
    qDebug()<<"DomainWnd::closeEvent";
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
DomainWnd::onDataLoaded()
{
    QModelIndex firstDomainIndex = m_domainModel->index(0,0,QModelIndex());
    lwDomainName->selectionModel()->setCurrentIndex(firstDomainIndex,QItemSelectionModel::ClearAndSelect);
    QModelIndex firstValIndex = m_domainModel->index(0,0,firstDomainIndex);
    lwValues->selectionModel()->setCurrentIndex(firstValIndex,QItemSelectionModel::ClearAndSelect);
}

void
DomainWnd::onOkSaveClick()
{
    if(m_kbManager->isValid())
    {
        m_kbManager->save();
        editStop();
    }
    else
    {
        QMessageBox::warning(this,"Ошибка при сохранении!","Неверные данные!");
    }
}

void
DomainWnd::onCancelClick()
{
    if(m_kbManager->reload())
    {
        editStop();
    }
    else
        QMessageBox::critical(this,"Ошибка!","Не удалось откатить!");
}

void
DomainWnd::onEditModeClick()
{
    editStart();
}

void
DomainWnd::onCloseClick()
{
    this->close();
}

void
DomainWnd::maybeClose()
{
    if(Saver::mayBeSave())//пытаемся сохранить
    {
        if(m_kbManager->saved())//нажал сохранить
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
DomainWnd::isEditMode()
{
    return editMode;
}


void
DomainWnd::setModel(DomainModel *model)
{
    this->m_domainModel = model;
}
