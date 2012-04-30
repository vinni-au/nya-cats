#include "nkbmanager.h"

NKBManager::NKBManager(QObject *parent) :
    QObject(parent)
{
    m_dirty = false;
    m_file = NULL;
    m_framenetModel = NULL;

    Clear();

    connect(&m_domainModel,SIGNAL(sigDataChanged()),SLOT(onDataChanged()));


    //контроль целостности модели
    //QObject::connect(domainModel,SIGNAL(sigDomainDeleted(QString)),varModel,SLOT(onDomainDeleted(QString)));
    //QObject::connect(domainModel,SIGNAL(sigDomainNameChanged(QString,QString)),varModel,SLOT(onDomainNameChanged(QString,QString)));

    //!QObject::connect(domainModel,SIGNAL(sigDomainValueChanged(QString,QString,QString)),ruleModel,SLOT(onDomainValueChanged(QString,QString,QString)));
    //!QObject::connect(domainModel,SIGNAL(sigDomainValueDeleted(QString,QString)),ruleModel,SLOT(onDomainValueDeleted(QString,QString)));
    //!QObject::connect(varModel,SIGNAL(sigVarNameChanged(QString,QString)),ruleModel,SLOT(onVarNameChanged(QString,QString)));

    //!QObject::connect(varModel,SIGNAL(sigVarDeleted(QString)),ruleModel,SLOT(onVarDeleted(QString)));
    //!QObject::connect(varModel,SIGNAL(sigVarDomainChanged(QString,QString,QString)),ruleModel,SLOT(onVarDomainChanged(QString,QString,QString)));

    //сигналы об ошибках валидации
    connect(&m_domainModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
    //connect(varModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
    //connect(ruleModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
}

//интерфейсные методы
///От диаграмм
QMap<unsigned, QString> NKBManager::frameNames()
{
    NFrame *frame;
    QMap<unsigned, QString> map;

    foreach(frame,m_frames)
    {
        map.insert(frame->id(),frame->name.defValue().toString());
    }
    return map;
}

void NKBManager::selectFrame(unsigned id)
{

}

bool NKBManager::addFrame(QString name)
{
    if(frameExists(name))
        return false;

    NFrame *frame = new NFrame(getFreeId());
    frame->name.setDefValue(name);
    m_frames.append(frame);

    m_framenetModel->setFrames(&m_frames);

    emit frameAdded(frame->id(), frame->name.defValue().toString());

    setDirty(true);

    return true;
}

bool NKBManager::deleteFrame(unsigned id)
{
    NFrame *frame = getFrameById(id);
    if(!frame)
        return false;

    //удаление связей
    deleteLinks(id);

    m_frames.removeOne(frame);

    m_framenetModel->setFrames(&m_frames);

    emit frameDeleted(id);

    setDirty(true);

    return true;
}
void NKBManager::deleteLinks(int id)
{
    NFrame* frame = getFrameById(id);
    if(!frame)
        return;

    QString frameName = frame->frameName();
    //удаление связей is_a, указывающих на фрейм
    foreach(frame,m_frames)
    {
        if(frame->parentFrame() == frameName)
        {
            frame->setParentName("");
        }
    }
    //удаление слотов фреймов, в которые фрейм входит как субфрейм
    foreach(frame,m_frames)
    {
        NSlot *slot = frame->getSlotByName(frameName);
        if(slot)
        {
            frame->removeSlot(slot);
        }
    }

}

//source id, destination id
bool NKBManager::addIsa(unsigned sid, unsigned did)
{
    NFrame *sFrame = getFrameById(sid);
    NFrame *dFrame = getFrameById(did);

    if((!sFrame) || (!dFrame) )
    {
        qDebug()<<"void NKBManager::addIsa  Связь не создана. Нет фреймов.";
        return false;
    }

    QString destName = dFrame->name.defValue().toString();

    //магический код изменения значения в тривью
    //sFrame->getSlotByName("is_a")->setValue( destName );
    QModelIndex sFrameIndex = m_framenetModel->getFrameIndexById(sFrame->id());
    QModelIndex slotIndex = m_framenetModel->getSlotFasetIndex(sFrameIndex,"is_a","default_value");

    //можно добавить связь?
    QString existingValue = m_framenetModel->data(slotIndex,Qt::DisplayRole).toString();//связь is_a уже есть
    if(!existingValue.isEmpty())
        return false;

    //проверка на обратную связь is_a
    QModelIndex dFrameIndex = m_framenetModel->getFrameIndexById(dFrame->id());
    QModelIndex destFrameSlotIndex = m_framenetModel->getSlotFasetIndex(dFrameIndex,"is_a","default_value");
    QString destFrameSlotVal = m_framenetModel->data(destFrameSlotIndex,Qt::DisplayRole).toString();
    if(destFrameSlotVal == sFrame->name.defValue())
        return false;

    //проверка связи a_part_of. если связь есть, is-a нельзя. причем в обе стороны
    NSlot* subframeSlot =  dFrame->getSlotByName(  sFrame->getSlotByName("name")->defValue().toString()   );
    if(subframeSlot)
        return false;

    subframeSlot =  sFrame->getSlotByName(  dFrame->getSlotByName("name")->defValue().toString()   );
    if(subframeSlot)
        return false;
    //

    m_framenetModel->setData(slotIndex,destName,Qt::EditRole);
    //
    qDebug()<<"void NKBManager::addIsa Создана связь is-a";

    //m_framenetModel->update();

    return true;
}

bool NKBManager::addApo(unsigned sid, unsigned did)
{
    NFrame *sFrame = getFrameById(sid);
    NFrame *dFrame = getFrameById(did);

    if((!sFrame) || (!dFrame) )
    {
        qDebug()<<"NKBManager::addApo  Связь не создана. Нет фреймов.";
        return false;
    }

    //проверка связи a_part_of.
    NSlot* subframeSlot =  dFrame->getSlotByName(  sFrame->getSlotByName("name")->defValue().toString()   );
    if(subframeSlot)
        return false;

    subframeSlot =  sFrame->getSlotByName(  dFrame->getSlotByName("name")->defValue().toString()   );
    if(subframeSlot)
        return false;
    //
    //проверка связи is_a между источником и стоком
    if(sFrame->parentFrame() == dFrame->frameName())
        return false;
    if(dFrame->parentFrame() == sFrame->frameName())
        return false;
    //

    QString sFrameName = sFrame->frameName();
    //
//    NSlot *slot = new NSlot();
//    slot->setName(sFrameName);
//    slot->getFasetByName("type")->setValue("frame");
//    slot->setValue(sFrameName);

//    dFrame->addSlot(slot);
//    m_framenetModel->update();
    QModelIndex frameIndex = m_framenetModel->getFrameIndexById(dFrame->id());
    QModelIndex slotIndex = m_framenetModel->addSlot(frameIndex);

    QModelIndex fasetNameIndex = m_framenetModel->getSlotFasetIndex(slotIndex,"name");
    m_framenetModel->setData(fasetNameIndex,sFrameName,Qt::EditRole);//имя слота
    QModelIndex fasetTypeIndex =m_framenetModel->getSlotFasetIndex(slotIndex,"slot_type");
    m_framenetModel->setData(fasetTypeIndex,"frame",Qt::EditRole);//тип слота
    QModelIndex fasetValueIndex =m_framenetModel->getSlotFasetIndex(slotIndex,"default_value");
    m_framenetModel->setData(fasetValueIndex,sFrameName,Qt::EditRole);//значение слота

    //

    return true;
}


bool NKBManager::deleteIsa(unsigned sid, unsigned did)
{
    NFrame *sFrame = getFrameById(sid);
    NFrame *dFrame = getFrameById(did);

    if((!sFrame) || (!dFrame) )
    {
        qDebug()<<"void NKBManager::addIsa  Связь не удалена. Нет фреймов.";
        return false;
    }

    NSlot *slot = sFrame->getSlotByName("is_a");
    QString isaFrameName = slot->defValue().toString();

    if(isaFrameName.isEmpty())
    {
        qDebug()<<"void NKBManager::addIsa  Связь не удалена. Ее нет).";
            return false;
    }

    slot->setDefValue("");
    return true;
}

bool NKBManager::deleteApo(unsigned sid, unsigned did)
{
    NFrame *sFrame = getFrameById(sid);
    NFrame *dFrame = getFrameById(did);

    if((!sFrame) || (!dFrame) )
    {
        qDebug()<<"NKBManager::deleteApo  Связь не удалена. Нет фреймов.";
        return false;
    }

    QString sFrameName = sFrame->name.defValue().toString();

    NSlot *slot = dFrame->getSlotByName(sFrameName);
    if(!slot)
    {
        qDebug()<<"NKBManager::deleteApo  Связь не удалена. Нет связи.";
        return false;
    }

//    dFrame->removeSlot(slot);
//    return true;

    //
    QModelIndex frameIndex = m_framenetModel->getFrameIndexById(did);
    QModelIndex slotIndex = m_framenetModel->getSlotFasetIndex(frameIndex,sFrameName,"name");

    return m_framenetModel->deleteSlot(slotIndex);


}

//конец интерфейсные методы




bool
NKBManager::SaveAs()
{
    if(!this->isValid())
    {
        QMessageBox mb;
        mb.setText("В данных ошибка. Не сохранено.");
        mb.exec();
        return false;
    }
    QString esFile=QFileDialog::getSaveFileName(NULL,tr("Сохранить БЗ как.."),QDir::currentPath(),tr("NYA-CATS (*.nya)"),0);
    if(!esFile.isEmpty())
    {
        qDebug()<<esFile;
        if(!this->saveAs(esFile/*.append(".esxml")*/))
        {
            //если не сохранилось
            return false;
        }
        return true;
    }
    else
        return false;
}

bool
NKBManager::mayBeSave()
{
    qDebug()<<"NKBManager::mayBeSave";

    if(!this->saved())
    {
        qDebug()<<"БЗ не сохранена";
        QMessageBox mb;
        mb.setWindowTitle("Сохранить?");
        mb.setText(tr("Сохранить предыдущие действия?"));
        mb.setModal(true);
        mb.addButton(QMessageBox::Yes);
        mb.addButton(QMessageBox::No);
        mb.addButton(QMessageBox::Cancel);
        switch( mb.exec() )
        {
        case QMessageBox::Yes:
            qDebug()<<"Сохранить";
            if(this->isValid())
            {
                if(this->file()!=NULL)
                {
                    qDebug()<<"Просто сохранить";
                    this->save();
                }
                else
                {
                    qDebug()<<"Сохранить как..";
                    if(!(SaveAs()))
                        return false;
                }
            }
            else
            {
                QMessageBox mbNotValid;
                mbNotValid.setText("В данных есть ошибка. Сохранение отменено.");
                mbNotValid.exec();
                qDebug()<<"Отмена";
                return false;
            }
            break;
        case QMessageBox::No:
            qDebug()<<"Не сохранять";
            break;
        case QMessageBox::Cancel:
            qDebug()<<"Отмена";
            return false;
            break;
        }
    }
    return true;
}

bool
NKBManager::areUsure(QString quest)
{
    QMessageBox mb;
    mb.setWindowTitle("А вы уверены?");
    mb.setText(quest);
    mb.addButton(QMessageBox::Ok);
    mb.addButton(QMessageBox::Cancel);
    mb.setDefaultButton(QMessageBox::Cancel);
    switch(mb.exec())
    {
    case QMessageBox::Ok:
        return true;
        break;
    case QMessageBox::Cancel:
        return false;
        break;
    }
    return false;
}
/////////////////////////из Datamodels





//DataModels::DataModels(QObject *parent):
//    QObject(parent)
//{
//    domainModel=new DomainModel();
//    varModel=new VarModel();
//    ruleModel=new RuleModel();
//    dirty = false;
//    file = NULL;

//    Clear();

//    connect(domainModel,SIGNAL(sigDataChanged()),SLOT(onDataChanged()));
//    connect(varModel,SIGNAL(sigDataChanged()),SLOT(onDataChanged()));
//    connect(ruleModel,SIGNAL(sigDataChanged()),SLOT(onDataChanged()));

//    //контроль целостности модели
//    QObject::connect(domainModel,SIGNAL(sigDomainDeleted(QString)),varModel,SLOT(onDomainDeleted(QString)));
//    QObject::connect(domainModel,SIGNAL(sigDomainNameChanged(QString,QString)),varModel,SLOT(onDomainNameChanged(QString,QString)));

//    QObject::connect(domainModel,SIGNAL(sigDomainValueChanged(QString,QString,QString)),ruleModel,SLOT(onDomainValueChanged(QString,QString,QString)));
//    QObject::connect(domainModel,SIGNAL(sigDomainValueDeleted(QString,QString)),ruleModel,SLOT(onDomainValueDeleted(QString,QString)));
//    QObject::connect(varModel,SIGNAL(sigVarNameChanged(QString,QString)),ruleModel,SLOT(onVarNameChanged(QString,QString)));

//    QObject::connect(varModel,SIGNAL(sigVarDeleted(QString)),ruleModel,SLOT(onVarDeleted(QString)));
//    QObject::connect(varModel,SIGNAL(sigVarDomainChanged(QString,QString,QString)),ruleModel,SLOT(onVarDomainChanged(QString,QString,QString)));

//    //сигналы об ошибках валидации
//    connect(domainModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
//    connect(varModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
//    connect(ruleModel,SIGNAL(sigErrorWhileValidating(QString)),SIGNAL(sigErrorWhileValidating(QString)));
//}


void
NKBManager::saveToXml(QTextStream &stream)
{
    doc = QDomDocument();
    QDomElement kbEl = doc.createElement("kb");   doc.appendChild(kbEl);
    QDomElement domainsEl = m_domainModel.toXml(doc);   kbEl.appendChild(domainsEl);
    QDomElement framesEl = doc.createElement("frames"); kbEl.appendChild(framesEl);

    //Фреймы в xml
    NFrame *frame;
    foreach(frame,m_frames)
    {
        QDomElement frameEl = frame->toXml(doc);
        framesEl.appendChild(frameEl);
    }
    //процедуры
    QDomElement procsEl = doc.createElement("procs"); kbEl.appendChild(procsEl);
    NProc *proc;
    foreach(proc,m_procs)
    {
        QDomElement procEl = proc->toXml(doc);
        procsEl.appendChild(procEl);
    }
    //продукции
    QDomElement productionsEl = doc.createElement("productions"); kbEl.appendChild(productionsEl);
    NProduction *production;
    foreach(production,m_productions)
    {
        QDomElement productionEl = production->toXml(doc);
        productionsEl.appendChild(productionEl);
    }

    //диаграммы
    kbEl.appendChild( m_diagramElement );

    doc.save(stream,4);

}
void
NKBManager::readFromXml(QFile &file)
{
    doc.clear();
    if(!doc.setContent(&file))
    {
        qDebug()<<"NKBManager::readFromXml: Не удалось считать файл: "<<file.fileName();
        return;
    }

    QDomElement kbEl = doc.documentElement();
    //домены
    QDomElement domainsEl = kbEl.firstChildElement("domains");
    m_domainModel.fromXml(domainsEl);
    //фреймы
    QDomElement framesEl = kbEl.firstChildElement("frames");
    QDomElement frameEl = framesEl.firstChild().toElement();
    while(!frameEl.isNull())
    {
        NFrame *frame = new NFrame(0);
        frame->fromXml(frameEl);
        m_frames.append(frame);
        frameEl = frameEl.nextSibling().toElement();
    }
    getFrameNetModel()->setFrames(&m_frames);
    //процедуры
    QDomElement procsEl = kbEl.firstChildElement("procs");
    QDomElement procEl = procsEl.firstChild().toElement();
    while(!procEl.isNull())
    {
        NProc *proc = new NProc();
        proc->fromXml(procEl);
        m_procs.append(proc);
        procEl = procEl.nextSibling().toElement();
    }
    //продукции
    QDomElement productionsEl = kbEl.firstChildElement("productions");
    QDomElement prodictionEl = productionsEl.firstChild().toElement();
    while(!prodictionEl.isNull())
    {
        NProduction *production = new NProduction();
        production->fromXml(prodictionEl);
        m_productions.append(production);
        prodictionEl = prodictionEl.nextSibling().toElement();
    }
    //диаграммы
    m_diagramElement=kbEl.firstChildElement("diagram");
    file.close();
    m_dirty=false;
    emit sigDataLoaded();
}

bool
NKBManager::saved()
{
    return !m_dirty;
}

void
NKBManager::save()
{
    if(!m_file)
    {
        qDebug("NKBManager::save() нет файла");
        return;
    }

    m_file->open(QIODevice::WriteOnly);
    QTextStream stream(m_file);
    saveToXml(stream);
    m_file->close();
    m_dirty = false;
}
bool
NKBManager::saveAs(QString filePath)
{
    qDebug()<<"DataModels::saveAs ::"<<filePath;
    QFile *newFile = new QFile(filePath);
//    if(newFile->exists())//такой файл уже существует
//    {
//        qDebug()<<"Такой файл уже существует";
//        return false;
//    }
    m_file = newFile;
    save();
    return true;
}

bool
NKBManager::Open(QString filePath)
{
    qDebug()<<"DataModels::Open ::"<<filePath;
    QFile *newFile = new QFile(filePath);
    if(!newFile->exists())//если нет такого файла
    {
        qDebug()<<"Нет такого файла";
        return false;
    }
    this->m_file = newFile;
    m_file->open(QIODevice::ReadOnly);
    readFromXml(*newFile);
    m_file->close();
    m_dirty=false;
    return true;
}
void//todo
NKBManager::Clear()
{
    m_domainModel.setDomains(new QList<Domain*>());
    m_frames.clear();
    m_productions.clear();
    m_procs.clear();

    if(m_framenetModel)
    {
        m_framenetModel->setFrames(&m_frames);
    }
    m_diagramElement = QDomElement();

    m_file = NULL;
    m_dirty = false;
}
void
NKBManager::onDataChanged()
{
    qDebug()<<"NKBManager::onDataChanged()";
    m_dirty = true;
}

bool//todo
NKBManager::isValid()
{
    //return domainModel->isValid()&&varModel->isValid()&&ruleModel->isValid();
    return true;
}

bool NKBManager::reload()
{
    QFile *tmpF = m_file;
    this->Clear();
    m_file = tmpF;

    if(this->file()!=NULL)
    {
        m_file->open(QIODevice::ReadOnly);
        this->readFromXml(*(this->file()));
        m_file->close();
    }

    m_dirty = false;
    return true;
}

NFrame* NKBManager::getFrameById(int id)
{
    NFrame *frame=NULL;
    foreach(frame,m_frames)
    {
        if(frame->id()==id)
        {
            return frame;
        }
    }
    return frame;
}

unsigned NKBManager::getIdByIndex(QModelIndex index)
{
    return getFrameNetModel()->getIdByIndex(index);
}

int NKBManager::getFreeId()
{
    NFrame *frame;
    int maxId=-1;
    foreach(frame,m_frames)
    {
        if(frame->id()>maxId)
        {
            maxId=frame->id();
        }
    }

    return (maxId+1);
}

bool NKBManager::frameExists(QString name)
{
    NFrame *frame;
    foreach(frame,m_frames)
    {
        if(frame->name.defValue()==name)
        {
            return true;
        }
    }
    return false;
}

NFramenetModel *NKBManager::getFrameNetModel()
{
    if(!m_framenetModel)
    {
        m_framenetModel = new NFramenetModel();
        m_framenetModel->setFrames(&m_frames);
        connect(m_framenetModel,SIGNAL(sigDataChanged()),SLOT(onDataChanged()));
    }

    return m_framenetModel;
}
void NKBManager::setDirty(bool dirty)
{
    this->m_dirty = dirty;
}

QFile* NKBManager::file()
{
    return this->m_file;
}

void NKBManager::setFile(QFile *file)
{
    m_file = file;
}

DomainModel* NKBManager::getDomainModel()
{
    return &m_domainModel;
}

QDomElement NKBManager::diagramNode()
{
    return m_diagramElement;
}

void NKBManager::setDiagramNode(QDomElement node)
{
    m_diagramElement = node;
}

bool NKBManager::productionExists(QString name)
{
    NProduction* p;
    foreach(p,this->m_productions)
    {
        if(p->name() == name)
            return true;
    }
    return false;
}

NProduction *NKBManager::getProduction(QString name)
{
    NProduction* p;
    foreach(p,this->m_productions)
    {
        if(p->name() == name)
            return p;
    }
    return NULL;
}

void NKBManager::reloadProduction(NProduction *production)
{
    if(!m_productions.contains(production))
        return;

    QDomDocument doc;
    if(!doc.setContent(file()))
    {
        qDebug()<<"NKBManager::reloadProduction: Не удалось считать файл: "<<file()->fileName();
        return;
    }

    QDomElement kbEl = doc.documentElement();
    //продукции
    QDomElement productionsEl = kbEl.firstChildElement("productions");
    QDomElement prodictionEl = productionsEl.firstChild().toElement();
    while(!prodictionEl.isNull())
    {
        if(prodictionEl.attribute("name") == production->name())
        {
            production->clear();
            production->fromXml(prodictionEl);
            break;
        }
        prodictionEl = prodictionEl.nextSibling().toElement();
    }

    file()->close();
    m_dirty=false;
    emit sigDataLoaded();
}

void NKBManager::addProduction(NProduction *prod)
{
    m_productions.append(prod);
}

QStringList NKBManager::getProductionNames()
{
    QStringList names;
    NProduction *p;
    foreach(p,m_productions)
    {
        names<<p->name();
    }
    return names;
}

QStringList NKBManager::getProceduresNames()
{
    QStringList names;
    NProc *p;
    foreach(p,m_procs)
    {
        names<<p->name();
    }
    return names;
}

NFrame *NKBManager::getFrameByName(QString name)
{
    NFrame *frame=NULL;
    foreach(frame,m_frames)
    {
        if(frame->frameName()==name)
        {
            return frame;
        }
    }
    return NULL;
}

QStringList NKBManager::getVars(QString frameName)
{
    NFrame *frame = getFrameByName(frameName);
    if(!frame)
        return QStringList();

    QStringList ownSlots = frame->getSimpleSlotNames();//собственные слоты

    QStringList subFsTypes;
    QStringList subFs = frame->getSubframesSlotNames(subFsTypes);
    QStringList subframeSlots;
    for(int i=0;i<subFs.count();i++)
    {
        QString prefix=subFs[i]+QString(".");
        subframeSlots = getVars(subFsTypes[i]);

        for(int j=0;j<subframeSlots.count();++j)
        {
            subframeSlots.replace(j,prefix+subframeSlots.at(j) );
        }
        ownSlots.append(subframeSlots);
    }


    return ownSlots;
}


QStringList NKBManager::getVarsWithParents(QString frameName)
{
    QStringList allSlots = getVars(frameName);

    NFrame *frame = getFrameByName(frameName);

    while(frame)
    {
        QString parent = frame->parentFrame();
        if(!parent.isEmpty())
        {
            QStringList parentSlots = getVars(parent);
            allSlots.append(parentSlots);
        }
        frame = getFrameByName(parent);
    }
    allSlots.removeDuplicates();
    return allSlots;
}


QString NKBManager::getDomainByString( QString frameName, QString str  )
{
    NSlot *slot = getSlotByString(frameName,str);
    if(!slot)
        return "";
    NFaset* faset = slot->getFasetByName( "slot_type" );
    return faset->value().toString();
}

NSlot * NKBManager::getSlotByString( QString frameName, QString str  )
{
    NFrame* frame = getFrameByName(frameName);
    if(!frame)
        return NULL;

    //тут надо еще наследование учесть
    if(!str.contains("."))
    {//просто слот
        NSlot* slot = frame->getSlotByName(str);
        if(!slot)
        {//ищем в родительском
            return getSlotByString(frame->parentFrame(),str);
        }
        else
        {
            return slot;
        }
    }
    else
    {//субфреймы
        int pointInx = str.indexOf(".");
        QString newStr = str.right(str.length()-pointInx-1);
        QString subfName = str.left(pointInx);

        //проверяем наличие слота субфрейма
        NSlot* slot = frame->getSlotByName(subfName);
        if(!slot)
        {//ищем в родительском
            return getSlotByString(frame->parentFrame(),str);
        }
        else
        {
            //return getSlotByString(subfName,newStr);
            return getSlotByString(slot->defValue().toString(),newStr);
        }

    }
    return NULL;
}

// Для МЛВ
NFrame* NKBManager::GetFrameByName(QString name)
{
    NFrame *frame;
    foreach(frame,m_frames)
    {
        if(frame->frameName()==name)
        {
            return frame;
        }
    }
    return NULL;
}

NFrame* NKBManager::GetFrameParent(NFrame* frame)
{
    QString parFrName = frame->parentFrame();
    return GetFrameByName(parFrName);
}

QList<NFrame*> NKBManager::GetFrameChildren(NFrame* frame)
{
    QList<NFrame*> list;
    NFrame *f;
    foreach(f,m_frames)
    {
        QString fParName = f->getSlotByName("is_a")->defValue().toString();
        if(!fParName.isEmpty())
        {
            if(fParName == frame->frameName())
                list<<f;
        }
    }
    return list;
}

NFrame* NKBManager::GetFrameInstance(NFrame* frame)
{
    return frame->createInstance();
}

NFrame* NKBManager::GetFrameInstance(QString name)
{
    NFrame* frame = GetFrameByName(name);
    if(!frame)
        return NULL;
    return frame->createInstance();
}

QList<NSlot*> NKBManager::GetFrameSlots(NFrame* frame)
{
    QList<NSlot*> list;
    for(int i=0;i<frame->slotCount();i++)
    {
        NSlot *slot=frame->getSlotByIndex(i);
        list<<slot;
    }
    return list;
}

NSlot* NKBManager::GetFrameSlot(NFrame* frame, QString slotName)
{
    if (!frame)
        return NULL;

    return frame->getSlotByName(slotName);
}

Domain* NKBManager::GetSlotDomain(NSlot* slot)
{
    QString domainName = slot->getFasetByName("slot_type")->value().toString();
    Domain *domain = m_domainModel.getDomainByName(domainName);
    return domain;
}

QList<NFaset*> NKBManager::GetSlotFasets(NSlot* slot)
{
    QList<NFaset*> list;
    for(int i=0;i<slot->fasetCount();i++)
    {
        NFaset *faset = slot->getFasetByIndex(i);
        list<<faset;
    }
    return list;
}

NProc* NKBManager::getProc(QString name)
{
    NProc* p;
    foreach(p,m_procs)
    {
        if(p->name()==name)
            return p;
    }
    return NULL;
}

NFrame* NKBManager::GetFrameInstanceWithParents(NFrame* frame)
{
    NFrame *frameInst = GetFrameInstance(frame);

    NFrame *parFrame=NULL;
    parFrame=this->GetFrameParent(frame);
    while(parFrame)
    {
        NFrame* parframeInst = GetFrameInstance(parFrame);
        frameInst->merge(parframeInst);
        parFrame=this->GetFrameParent(parFrame);
    }
    return frameInst;
}

NFrame* NKBManager::GetFrameInstanceWithParents(QString frameName)
{
    NFrame* frame = GetFrameByName(frameName);
    if (!frame)
        return NULL;

    return GetFrameInstanceWithParents(frame);
}

QList<NFrame*> NKBManager::getAllChildren(NFrame* frame)//получает потомков и потомков потомков ...
{
    QList<NFrame*> children;
    QList<NFrame*> myChildren = GetFrameChildren(frame);
    children.append(myChildren);
    NFrame* child;
    foreach(child,myChildren)
    {
        children.append(getAllChildren(child));
    }
    return children;
}

QStringList NKBManager::getFilteredFrameList(QString frameName,QString slotName)
{
    NFrame* frame = GetFrameByName(frameName);
    if(!frame)
        return QStringList();
    if(slotName=="is_a")
    {//выдавать только фреймы, которые не являются текуцим фреймом и детьми его (в перспективе - дети детей детей)
        QStringList childrenNames;

        QList<NFrame*> children = getAllChildren(frame);

        QList<NFrame*> allFrames;
        foreach(NFrame* f,m_frames)//добавляем все фреймы
        {
            allFrames<<f;
        }
        foreach(NFrame* f,children)
        {
            allFrames.removeAll(f);
        }
        foreach(NFrame* f,allFrames)
        {
            childrenNames<<f->frameName();
        }
        childrenNames.removeDuplicates();
        return childrenNames;
    }
    else
    {//субфреймы
        QStringList childrenNames;
        QList<NFrame*> allFrames;
        foreach(NFrame* f,m_frames)//добавляем все фреймы
        {
            allFrames<<f;
        }
        allFrames.removeAll(frame);
        foreach(NFrame* f,allFrames)
        {
            childrenNames<<f->frameName();
        }
        childrenNames.removeDuplicates();
        return childrenNames;
    }
    return QStringList();
}
