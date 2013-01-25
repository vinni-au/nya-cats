#include "nframenetmodel.h"
#include <QDebug>

NFramenetModel::NFramenetModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    rootNode=new NFrameNode(NFrameNode::Root,NULL,NULL);
    itemsIsEditable = true;
    frames = new QList<NFrame*>();

    m_freeId = -1;
    frames=NULL;
    m_fasetCount = 6;
}

NFramenetModel::~NFramenetModel()
{
    delete rootNode;
}

void
NFramenetModel::setRootNode(NFrameNode *node)
{
    if(rootNode!=node)
        delete rootNode;
    rootNode=node;
    reset();
}

QModelIndex
NFramenetModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!rootNode || row<0 || column<0)
        return QModelIndex();
    NFrameNode *parentNode=nodeFromIndex(parent);
    NFrameNode *childNode=parentNode->children.value(row);
    if(!childNode)
        return QModelIndex();
    return createIndex(row,column,childNode);
}

NFrameNode*
NFramenetModel::nodeFromIndex(const QModelIndex &index) const
{
    if(index.isValid())
    {
        return static_cast<NFrameNode*>(index.internalPointer());
    }
    else
    {
        return rootNode;
    }
}

int
NFramenetModel::rowCount(const QModelIndex &parent) const
{
    NFrameNode *parentNode=nodeFromIndex(parent);
    if(!parentNode)
        return 0;
    return parentNode->children.count();
}

int
NFramenetModel::columnCount(const QModelIndex &parent) const
{
    NFrameNode *parentNode=nodeFromIndex(parent);
    if(!parentNode)
        return 0;

    return getFasetCount();
}

QModelIndex
NFramenetModel::parent(const QModelIndex &child) const
{
    NFrameNode *node=nodeFromIndex(child);
    if(!node)
        return QModelIndex();
    NFrameNode *parentNode=node->parent;
    if(!parentNode)
        return QModelIndex();
    NFrameNode *grandParentNode=parentNode->parent;
    if(!grandParentNode)
        return QModelIndex();
    int row=grandParentNode->children.indexOf(parentNode);
    return createIndex(row,0,parentNode);
}
QVariant
NFramenetModel::data(const QModelIndex &index, int role) const
{
    if(role!= Qt::DisplayRole /*&& role!=Qt::EditRole*/ )
        return QVariant();

    NFrameNode *node=nodeFromIndex(index);
    if(!node)
        return QVariant();

    NFrame *frame;
    NSlot *slotName;
    NFaset *faset;
    NSlot *slot;
    int column;
    int row;

    NSlot* x_slot;
    QString x_val;
    NSlot* y_slot;
    QString y_val;
    QString frameName;

    switch(node->type)
    {
    case NFrameNode::Root:
        return "root";
        break;
    case NFrameNode::FrameName:

        if(index.column()!=0)
            break;

        frame = node->frame;
        if(!frame)
            return "errNoFrame";
        slotName = frame->getSlotByName("name");
        if(!slotName)
            return "errSlotName";

        frameName = slotName->defValue().toString();

        if( frame->frameType()==FrameType::exemplar)
        {
            frameName.append(QString(" %1").arg(frame->id()));
            if(frameName.contains("Ячейка игрового поля"))
            {
                x_slot = frame->getSlotByName("x");
                x_val = x_slot->getFasetByName("value")->value().toString();
                y_slot = frame->getSlotByName("y");
                y_val = y_slot->getFasetByName("value")->value().toString();

                frameName.append( (QString(" (%1, %2)")).arg(x_val).arg(y_val));
            }
        }
        return frameName;
        break;
    case NFrameNode::Faset:
        column = index.column();
        row = index.row();

        frame = node->frame;
        slot = frame->getSlotByIndex(index.row());
        if(!slot)
            return QVariant();
        faset = slot->getFasetByIndex(column);
        if(!faset)
            return QVariant();
//!!!???
        if(faset->name()=="name" && slot->isSystem())
            return QString("#").append(faset->value().toString());
        return faset->value();
        break;
    default:
        qDebug()<<"NFramenetModel::data  неизвестный тип node";
        break;
    }

    return QVariant();
}
bool
NFramenetModel::setData(const QModelIndex &index, const QVariant &value, int role)//изменение данных пока не планируется, хотя возможно Саньку это понадобится
{
    if(index.isValid() && role==Qt::EditRole)
    {
//        if(value.toString().trimmed() == "")
//        {
//            emit sigErrorWhileValidating(QString("Пустое значение запрещено!"));
//            return false;    //можно испустить какой-нибудь сигнал
//        }
        NFrameNode *node=nodeFromIndex(index);
        NFrame *frame=node->frame;
        if(!frame)
            qt_assert("нету фрейма. NFramenetModel::setData ","",109);


        NSlot *slotName;
        NFaset *faset;
        NSlot *slot;
        int column;
        QVariant oldValue;

        switch(node->type)
        {
        case NFrameNode::Root:
            break;

        case NFrameNode::FrameName:
            frame = node->frame;
            if(!frame)
                return "errNoFrame";
            slotName = frame->getSlotByName("name");
            if(!slotName)
                return "errSlotName";

            slotName->setDefValue(value);

            emit dataChanged(index,index);
            emit sigDataChanged();
            return true;
            break;

        case NFrameNode::Faset:
            column = index.column();

            frame = node->frame;
            slot = frame->getSlotByIndex(index.row());

            faset = slot->getFasetByIndex(column);
            oldValue = faset->value();

            if(slot->isSystem())
            {
                if(faset->name()=="name" || faset->name()=="slot_type")//имена системных слотов менять нельзя
                {
                    return false;
                }
                if(slot->name()=="name")
                {
                    if(faset->name()=="default_value")
                    {
                        if(value.toString()!=faset->value())//значение изменилось
                        {
                                if(frameExists(value.toString()))//нельзя
                                {
                                    return false;
                                }
                                else
                                {
                                    emit sigFrameNameChanged(frame->frameName(),value.toString());
                                }
                        }

                    }
                }
            }

            faset->setValue(value);

            emit dataChanged(index,index);
            emit sigDataChanged();


            if(slot->getSlotType()=="frame")
            {
                //isa
                if(faset->name()=="default_value")
                {
                    if(slot->name()=="is_a")
                    {
                        if(oldValue.toString().isEmpty())
                        {//добавлена связь
                            emit sigIsaAdded(frame->frameName(),value.toString());
                        }
                        else
                        {//изменена
                            if(value.toString().isEmpty())
                            {//очищена
                                emit sigIsaDeleted(frame->frameName(),oldValue.toString());
                            }
                            else
                            {//просто изменена на другой фрейм
                                emit sigIsaChanged(frame->frameName(),oldValue.toString(),value.toString());
                            }
                        }
                    }
                    else
                    {//субфрейм
                        if(oldValue.toString().isEmpty())
                        {//добавлена связь
                            //нужно добавлять только если такой связи уже нет
                            if(frame->getSlotsByDefValue(value.toString()).count() == 1)
                            {
                                emit sigApoAdded(value.toString(),frame->frameName());
                            }
                        }
                        else
                        {//изменена
                            if(value.toString().isEmpty())
                            {//очищена
                                //очищать только если не осталось больше связей с этим субфреймом
                                if(frame->getSlotsByDefValue(oldValue.toString()).count() == 0)
                                {
                                    emit sigApoDeleted(oldValue.toString(),frame->frameName());
                                }
                            }
                            else
                            {//просто изменена на другой фрейм
                                //тут тоже все хитро
                                //если много связей, то на удаление не кидать сигнал

                                if(frame->getSlotsByDefValue(oldValue.toString()).count() >0)//входит много стрелок
                                {
                                     if(frame->getSlotsByDefValue(value.toString()).count() >1)//целевой
                                     {
                                         //ничего не делаем и так все стрелки есть ыыы
                                         int sd=2+4;
                                         sd+=1;
                                     }
                                     else
                                     {
                                         emit sigApoAdded(value.toString(),frame->frameName());
                                     }

                                }
                                else
                                {//входит одна стрелка из старых значений
                                    if(frame->getSlotsByDefValue(value.toString()).count() !=0 )//целевой
                                    {//у новых значений есть стрелки уже
                                        //удаление источника
                                        emit sigApoDeleted(oldValue.toString(),frame->frameName());
                                    }
                                    else
                                    {
                                        emit sigApoChanged(oldValue.toString(),value.toString(),frame->frameName());
                                    }
                                }

                            }
                        }
                    }
                }
            }
            return true;
            break;


//        case DomainNode::DomainName:
//            if(!domainExists(value.toString().trimmed()))
//            {
//                emit sigDomainNameChanged(domain->name.trimmed(),value.toString().trimmed());       //сигнал об изменении имени домена
//                domain->name=value.toString().trimmed();
//                emit dataChanged(index,index);
//                emit sigDataChanged();
//                return true;
//            }
//            else
//            {
//                emit sigValueExists(index,value);
//                emit sigErrorWhileValidating(QString("Домен с именем %1 уже есть!").arg("'"+value.toString()+"'"));
//            }
//            break;

        default:
            qDebug()<<"DomainModel::setData// case default:";
            break;
        }
    }
    return false;
}

Qt::ItemFlags
NFramenetModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    return flags;
//    Qt::ItemFlags flag = Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsEnabled | Qt::ItemIsSelectable ;
//    if(itemsIsEditable)
//        flag|= Qt::ItemIsEditable;
//    return flag;
}

bool
NFramenetModel::removeRow(int row, const QModelIndex &parent)
{
    if (row >= 0 && row < rowCount(parent))
    {
        beginRemoveRows(parent, row, row);

        NFrameNode *parentNode = nodeFromIndex(parent);
        NFrameNode *node = parentNode->children.at(row);
        NFrame *frame=NULL;
        NSlot *slot=NULL;
        int inx;
        switch(parentNode->type)//родитель
        {
        case NFrameNode::Root:          //удаляем фреймы
            frame=node->frame;
            //emit sigDomainDeleted(domain->name);                    //оповещение об удалении (пока не удален)
            inx = frames->indexOf(frame);
           // domains->remove(inx);
            frames->removeAt(inx);
            parentNode->children.removeAt(row);//delete?

            delete frame;
            delete node;

            emit sigDataChanged();
            break;
        case NFrameNode::FrameName:    //удаляем слоты

            frame=parentNode->frame;

            slot = frame->getSlotByIndex(row);
            if(!slot)
                qDebug()<<"NFramenetModel::removeRow slot is null";

            if(slot->isSystem())
                break;

            if(slot->getSlotType()=="frame")
            {
                if(frame->getSlotsByDefValue( slot->defValue().toString() ).count() == 1)
                {
                    emit sigApoDeleted(slot->defValue().toString(),frame->frameName());
                }
            }

            parentNode->children.removeAt(row);
            delete node;
            //emit sigDomainValueDeleted(domain->name, domain->values.at(row));    //сигнал об удалении значения


            frame->removeSlot(row);
            emit sigDataChanged();
            break;
        case NFrameNode::Faset:
            break;
        }

        endRemoveRows();
        return true;
    }
    return false;
}

bool
NFramenetModel::insertRow(int row, const QModelIndex &parent)
{
    if (row >= 0)
    {
        beginInsertRows(parent,row,row);

        NFrameNode *parentNode = nodeFromIndex(parent);
        NFrameNode *node;
        NFrame *frame=NULL;
        NFrame *newFrame=NULL;
        NSlot *slot=NULL;

        switch(parentNode->type)
        {
        case NFrameNode::Root://добавляем новый фрейм
            newFrame = new NFrame(getFreeId());//сгенерировать уникальлное имя???
            //m_fasetCount = newFrame->getSlotByName("name")->fasetCount();
            node=new NFrameNode(NFrameNode::FrameName,newFrame,true,rootNode);
            //this->domains->append(newDomain);
            frames->insert(row,newFrame);
            emit sigDataChanged();
            break;


        case NFrameNode::FrameName://добавляем новый слот
            frame = parentNode->frame;
            node=new NFrameNode(NFrameNode::Faset,frame,true,parentNode);

            slot = new NSlot();
            frame->insertSlot(row,slot);///имя слота????

            emit sigDataChanged();
            break;
        case NFrameNode::Faset://DO NOTHING
            break;
        }
        endInsertRows();
        return true;
    }
    return false;
}

void
NFramenetModel::setFrames( QList<NFrame *> *frames )
{
    qDebug()<<"NFramenetModel::setFrames";
    //здесь делаем разбор списка фреймов
    //строим на основе списка дерево DomainNode

    //this->frames->clear();



    if(frames->count()>0)
    {
        m_fasetCount = frames->at(0)->getSlotByName("name")->fasetCount();
    }

    if(this->frames!=frames)
    {
        if(frames->count()>0)
        {
            qDeleteAll(*(this->frames));
            this->frames->clear();
        }
        delete this->frames;//? Статичное поле?
    }

    this->frames = frames;

    qDeleteAll(rootNode->children);
    rootNode->children.clear();

    NFrame *frame;

    foreach(frame,*frames)
    {
        NFrameNode *frameNode = new NFrameNode(NFrameNode::FrameName,frame,true,rootNode);
        //слоты
        NSlot *slot;
        NFrameNode *slotNode;
        for(int i=0;i<frame->slotCount();i++)
        {
            slot = frame->getSlotByIndex(i);
            slotNode = new NFrameNode(NFrameNode::Faset,frame,true,frameNode);
        }
    }
    reset();
    emit sigDataChanged();

}

//Qt::DropActions
//NFramenetModel::supportedDropActions() const
//{
//    return Qt::MoveAction;
//}

//QModelIndex
//DomainModel::addDomain()
//{
//    int rowCount=domains->count();

//    if(!insertRow(rowCount,QModelIndex()))
//        return QModelIndex();
//    return createIndex(rowCount,0,rootNode->children.at(rowCount));
//}
//void
//DomainModel::editDomain(QModelIndex domain, QString newName)
//{
//    DomainNode *node=nodeFromIndex(domain);
//    node->domain->name=newName;
//    emit sigDataChanged();
//}
//void
//DomainModel::deleteDomain(QModelIndex index)
//{
//    if(!index.isValid())
//        return;

//    removeRow(index.row(),parent(index));
//}
//bool
//DomainModel::domainExists(QString domainName)
//{
//    bool exists=false;
//    Domain *domain=NULL;

//    foreach(domain,*(this->domains))
//    {
//        if(domain->name.trimmed().compare(domainName.trimmed(),Qt::CaseInsensitive)==0)//(domain->name == domainName)
//        {
//            exists = true;
//            break;
//        }
//    }
//    return exists;
//}
//QModelIndex
//DomainModel::addValue(QModelIndex domainIndex)
//{
//    if(!domainIndex.isValid())
//        return QModelIndex();

//    DomainNode *node = nodeFromIndex(domainIndex);
//    if(!node)
//        return QModelIndex();
//    int rowCount = node->children.count();
//    if(!insertRow(rowCount,domainIndex))
//        return QModelIndex();
//    return createIndex(rowCount,0,node->children.at(rowCount));
//}
//void
//DomainModel::deleteValue(QModelIndex valueIndex)
//{
//    if(!valueIndex.isValid())
//        return;
//    removeRow(valueIndex.row(),parent(valueIndex));
//}
//bool
//DomainModel::valueExists(QString domainName,QString value)
//{
//    bool exists=false;
//    Domain *domain=NULL;

//    foreach(domain,*(this->domains))
//    {
//        if(domain->name == domainName)
//        {
//            for(int i =0;i<domain->values.count();++i)
//            {
//                if(domain->values[i].trimmed().compare(value.trimmed(),Qt::CaseInsensitive)==0)
//                {
//                    exists = true;
//                    break;
//                }
//            }
////            if(domain->values.contains(value))
////            {
////                exists = true;
////                break;
////            }
//        }
//    }
//    return exists;
//}
//bool
//DomainModel::valueExists(Domain *domain,QString value)
//{
//    bool exists=false;

//    for(int i =0;i<domain->values.count();++i)
//    {
//        if(domain->values[i].trimmed().compare(value.trimmed(),Qt::CaseInsensitive)==0)
//        {
//            exists = true;
//            break;
//        }
//    }

////    if(domain->values.contains(value))
////    {
////        exists = true;
////    }
//    return exists;
//}
//QModelIndex
//DomainModel::indexByName(QString domain)
//{
//    for(int i=0;i<domains->count();++i)
//    {
//        if(domains->at(i)->name == domain)
//        {
//            return createIndex(i,0,rootNode->children.at(i));
//        }
//    }
//    return QModelIndex();
//}


//bool
//DomainModel::isValid()
//{
//    bool isValid = true;
//    Domain *d;
//    QStringList domainsStrList;
//    foreach(d,*domains)
//    {
//          domainsStrList<<d->name;

//          if(!Validator::NoCopies( d->values.toList() ))
//          {
//              isValid = false;
//              emit sigErrorWhileValidating(QString("Повторяющиеся значения в домене %1").arg(d->name));
//              break;
//          }
//    }
//    if(isValid)
//    {
//        if(!Validator::NoCopies( domainsStrList))
//        {
//            isValid = false;
//            emit sigErrorWhileValidating(QString("Повторяющиеся имена доменов"));
//        }
//    }

//    return isValid;
//}

//void
//NFramenetModel::setItemsIsEditable(bool isEditable)
//{
//    itemsIsEditable = isEditable;
//}


//Drag'&'Drop

//void NFramenetModel::moveBefore(QModelIndex sourceInx,QModelIndex destInx)
//{
//    if(!sourceInx.isValid())
//        return;
//    if(!destInx.isValid())
//        return;
//    QModelIndex parentInx = parent(sourceInx);
//    DomainNode* parentNode = nodeFromIndex(parentInx);
//    if(!parentNode)
//        return;
//    qDebug()<<"sourceInx:"<<sourceInx.row()<<" destInx:"<<destInx.row();
//    qDebug()<<"DomainModel::: "<<data(sourceInx,Qt::DisplayRole).toString()<<" "<<data(destInx,Qt::DisplayRole).toString();
//    if(parentNode->type == DomainNode::Root)//перетаскиваем домены
//    {
//        if(sourceInx.row()>destInx.row() )
//        {
//            parentNode->children.move(sourceInx.row(),destInx.row());
//            domains->move(sourceInx.row(),destInx.row());
//        }
//        else
//        {
//            parentNode->children.move(sourceInx.row(),destInx.row()-1);
//            domains->move(sourceInx.row(),destInx.row()-1);
//        }
//    }
//    else//перетаскиваем значения
//    {
//        if(sourceInx.row()>destInx.row() )
//        {
//            parentNode->children.move(sourceInx.row(),destInx.row());
//            QString tmp = parentNode->domain->values.at(sourceInx.row());
//            parentNode->domain->values.remove(sourceInx.row());
//            parentNode->domain->values.insert(destInx.row(),tmp);
//        }
//        else
//        {
//            parentNode->children.move(sourceInx.row(),destInx.row()-1);
//            QString tmp = parentNode->domain->values.at(sourceInx.row());
//            parentNode->domain->values.remove(sourceInx.row());
//            parentNode->domain->values.insert(destInx.row()-1,tmp);
//        }
//    }
//    emit sigDataChanged();
//}


//QStringList NFramenetModel::mimeTypes() const
// {
//     QStringList types;
//     types << "application/qmodelindex";
//     return types;
// }
//QMimeData *NFramenetModel::mimeData(const QModelIndexList &indexes) const
// {
//     QMimeData *mimeData = new QMimeData();
//     QByteArray encodedData;
//     QDataStream stream(&encodedData, QIODevice::WriteOnly);
//     foreach (const QModelIndex &index, indexes)
//     {
//         if (index.isValid())
//         {
//             const QModelIndex* pInx;
//             pInx = &index;
//             char * pChar = (char*)pInx;
//             QByteArray array(pChar,sizeof(QModelIndex));
//             stream << array;
//         }
//     }
//     mimeData->setData("application/qmodelindex", encodedData);
//     return mimeData;
// }
//bool NFramenetModel::dropMimeData(const QMimeData *data,
//                                     Qt::DropAction action, int row, int column, const QModelIndex &parent)
//{
//    if (action == Qt::IgnoreAction)
//        return true;
//    if (!data->hasFormat("application/qmodelindex"))
//        return false;
//    if (column > 0)
//        return false;
//    QByteArray encodedData = data->data("application/qmodelindex");
//    QDataStream stream(&encodedData, QIODevice::ReadOnly);
//    QModelIndex *sourceIndex;
//    QByteArray array;
//    stream>>array;
//    sourceIndex= (QModelIndex *)array.data();
//    QModelIndex destIndex;
//    if(row==-1)
//    {
//        destIndex = parent;
//    }
//    else
//    {
//        destIndex=index(row,0,parent);
//    }
//    moveBefore(*sourceIndex,destIndex);
//    return true;
//}


//generate unique names
//QString
//DomainModel::generateNewUniqueDomainName()
//{
//    bool exists=true;
//    QString base = "Domain";
//    int i=1;

//    while(exists)
//    {
//        QString res = base+QString::number(i);

//        if(!domainExists(res.trimmed()))
//        {
//            return res;
//        }
//        ++i;
//    }
//    return "";
//}

//QString
//DomainModel::generateNewUniqueDomainValue(Domain *domain)
//{
//    bool exists=true;
//    QString base = "Value";
//    int i=1;

//    while(exists)
//    {
//        QString res = base+QString::number(i);

//        if(!valueExists(domain,res.trimmed()))
//        {
//            return res;
//        }
//        ++i;
//    }
//    return "";
//}

int NFramenetModel::getFreeId()
{
    m_freeId++;
    return m_freeId;
}

int NFramenetModel::getIdByIndex(QModelIndex index)
{
    NFrameNode *node = nodeFromIndex(index);
    if(!node)
       return -1;

    NFrame *frame = node->frame;
    return frame->id();
}

void NFramenetModel::update()
{
    this->reset();
}

QModelIndex NFramenetModel::addSlot(QModelIndex& frameIndex)
{
    NFrameNode *node = nodeFromIndex(frameIndex);

    if(!node)
        return QModelIndex();

    if(node->type==NFrameNode::Faset)
    {
        node=node->parent;
        frameIndex = parent(frameIndex);
    }

    if(!node)
        return QModelIndex();

    if(node->type!=NFrameNode::FrameName)
        return QModelIndex();

    int row = node->children.count();
    if(!insertRow(row,frameIndex))
        return QModelIndex();

    return createIndex(row,0,node->children.at(row));
}
bool NFramenetModel::deleteSlot(QModelIndex& slotIndex)
{
    NFrameNode *node = nodeFromIndex(slotIndex);
    if(!node)
        return false;
    if(node->type!=NFrameNode::Faset)
        return false;
    int row = slotIndex.row();
    QModelIndex frameIndex =(QModelIndex) parent(slotIndex);
    if(!removeRow(row,frameIndex))
        return false;
    return true;
}
QVariant NFramenetModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if (role != Qt::DisplayRole)
             return QVariant();
    if (orientation != Qt::Horizontal)
        return QVariant();

    NSlot slot;
    slot.setHasFasetValue(true);

    if(section >= getFasetCount())
        return QVariant();

    QString str = slot.getFasetByIndex(section)->name();
    return QVariant( str);
}


bool NFramenetModel::setSlotFasetValue(QModelIndex slotIndex,QString fasetName,QVariant value)
{
    if(!slotIndex.isValid())
        return false;
    if(fasetName=="")
        return false;
    NFrameNode *node = nodeFromIndex(slotIndex);
    if(!node)
        return false;
    if(node->type!=NFrameNode::Faset)
        return false;

    NFrame *frame = node->frame;

    if(!frame)
        return false;

    NSlot *slot = frame->getSlotByIndex(slotIndex.row());
    NFaset *faset = slot->getFasetByName(fasetName);
    if(!faset)
        return false;

    QModelIndex fasetIndex = getSlotFasetIndex(slotIndex,fasetName);
    setData(fasetIndex,value,Qt::EditRole);

    ///faset->setValue(value);
    emit sigDataChanged();
    return true;
}

QVariant NFramenetModel::getSlotFasetValue(QModelIndex slotIndex,QString fasetName)
{
    if(!slotIndex.isValid())
        return QVariant();
    if(fasetName=="")
        return QVariant();
    NFrameNode *node = nodeFromIndex(slotIndex);
    if(!node)
        return QVariant();
    if(node->type!=NFrameNode::Faset)
        return QVariant();

    NFrame *frame = node->frame;

    if(!frame)
        return QVariant();

    NSlot *slot = frame->getSlotByIndex(slotIndex.row());
    NFaset *faset = slot->getFasetByName(fasetName);
    if(!faset)
        return QVariant();
    return faset->value();
}
bool NFramenetModel::isSlot(QModelIndex slotIndex)
{
    NFrameNode *node = nodeFromIndex(slotIndex);
    if(!node)
        return false;
    if(node->type!=NFrameNode::Faset)
        return false;
    return true;
}


QModelIndex NFramenetModel::getFrameIndexById(int id)
{
    NFrame *frame;
    int row=-1;
    foreach(frame,*frames)
    {
        row++;
        if(frame->id()==id)
            break;
    }

    if(row<0)
        return QModelIndex();

    return createIndex(row,0,rootNode->children.at(row));
}
QModelIndex NFramenetModel::getSlotFasetIndex(QModelIndex frameIndex,QString slotName,QString fasetName)
{
    NFrameNode *frameNode = nodeFromIndex(frameIndex);

    NFrame *frame = frameNode->frame;
    int row = frame->getSlotIndexByName(slotName);
    NFrameNode *slotNode = frameNode->children.at(row);
    NSlot *slot=frame->getSlotByName(slotName);

    if(!slot || row<0)
        return QModelIndex();

    int column = slot->getFasetIndexByName(fasetName);

    return createIndex(row,column,slotNode);
}
QModelIndex  NFramenetModel::getSlotFasetIndex(QModelIndex slotIndex,QString fasetName)
{
    NFrameNode *slotNode0 = nodeFromIndex(slotIndex);
    NFrameNode *frameNode = slotNode0->parent;

    NFrame *frame = frameNode->frame;
    int row = slotIndex.row();
    NFrameNode *slotNode = frameNode->children.at(row);
    NSlot *slot=frame->getSlotByIndex(row);

    if(!slot || row<0)
        return QModelIndex();

    int column = slot->getFasetIndexByName(fasetName);

    return createIndex(row,column,slotNode);
}

QString NFramenetModel::getFrameNameByIndex(QModelIndex index)
{
    NFrameNode *node = nodeFromIndex(index);
    NFrame *frame = node->frame;
    return frame->frameName();
}

int NFramenetModel::getFasetCount() const
{
    return m_fasetCount;
}

bool NFramenetModel::frameExists(QString frameName)
{
    for(int i =0;i<frames->count();i++)
    {
        if(frames->at(i)->frameName()==frameName)
            return true;
    }
    return false;
}

NFrame* NFramenetModel::getFrameByName(QString name)
{
    for(int i =0;i<frames->count();i++)
    {
        if(frames->at(i)->frameName()==name)
            return frames->at(i);
    }
    return NULL;
}
