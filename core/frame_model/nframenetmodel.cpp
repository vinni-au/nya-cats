#include "nframenetmodel.h"
#include <QDebug>

NFramenetModel::NFramenetModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    rootNode=new NFrameNode(NFrameNode::Root,NULL,NULL);
    itemsIsEditable = true;


}

NFramenetModel::~NFramenetModel()
{
    delete rootNode;
}

void
NFramenetModel::setRootNode(NFrameNode *node)
{
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

    NSlot slot;
    return slot.fasetCount();
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

        return slotName->value();
        break;
    case NFrameNode::Faset:
        column = index.column();

        frame = node->frame;
        slot = frame->getSlotByIndex(index.row());
        faset = slot->getFasetByIndex(column);
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

            slotName->setValue(value);

            emit dataChanged(index,index);
            emit sigDataChanged();
            return true;
            break;

        case NFrameNode::Faset:
            column = index.column();

            frame = node->frame;
            slot = frame->getSlotByIndex(index.row());

            faset = slot->getFasetByIndex(column);
            faset->setValue(value);

            emit dataChanged(index,index);
            emit sigDataChanged();
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
            //emit sigDomainDeleted(domain->name);                    //оповещение об удалении домена(пока не удален)
            inx = frames->indexOf(frame);
           // domains->remove(inx);
            frames->removeAt(inx);
            parentNode->children.removeAt(row);//delete?
            emit sigDataChanged();
            break;
        case NFrameNode::FrameName:    //удаляем слоты

            frame=parentNode->frame;

            slot = frame->getSlotByIndex(row);
            if(!slot)
                qDebug()<<"NFramenetModel::removeRow slot is null";

            if(slot->isSystem())
                break;

            parentNode->children.removeAt(row);
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

    this->frames = frames;
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
    NFrame *frame;
    int maxId=-1;
    foreach(frame,*frames)
    {
        if(frame->id()>maxId)
        {
            maxId=frame->id();
        }
    }

    return (maxId+1);
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

bool NFramenetModel::addSlot(QModelIndex& frameIndex)
{
    NFrameNode *node = nodeFromIndex(frameIndex);

    if(!node)
        return false;

    if(node->type!=NFrameNode::FrameName)
        return false;

    int row = node->children.count();
    if(!insertRow(row,frameIndex))
        return false;

    return true;
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
    if(section > slot.fasetCount())
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
    faset->setValue(value);
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
