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
    if(!rootNode||row<0||column<0)
        return QModelIndex();
    NFrameNode *parentNode=nodeFromIndex(parent);
    NFrameNode *childNode=parentNode->children.value(row);
    if(!childNode)
        return QModelIndex();
    return createIndex(row,0,childNode);
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
    if(parent.column()>0)
        return 0;
    NFrameNode *parentNode=nodeFromIndex(parent);
    if(!parentNode)
        return 0;
    return parentNode->children.count();
}
int
NFramenetModel::columnCount(const QModelIndex &parent) const
{
    if(parent.column()>0)
        return 0;
    NFrameNode *parentNode=nodeFromIndex(parent);
    if(!parentNode)
        return 0;

    if(parentNode->type == NFrameNode::FrameName)
    {
        NFrame *frame = parentNode->frame;
        return frame->slotCount();
    }
    else
        return 1;
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

//    if(index.column()==0)
//    {
//        if(role==Qt::EditRole)          //при редактировании
//        {
//            if(node->type==DomainNode::DomainName)
//            {
//                return node->domain->name;
//            }
//            else if(node->type==DomainNode::DomainValue)
//            {
//                return node->domain->values.at(index.row());
//            }
//        }

//        //отображение
//        switch(node->type)
//        {
//        case DomainNode::Root:
//            return "root";

//        case DomainNode::DomainName:
//            return node->domain->name;

//        case DomainNode::DomainValue:
//            return node->domain->values.at(index.row());
//        default:
//            return "Unkn";
//        }
//    }
    return QVariant();
}
bool
NFramenetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
//    if(index.isValid() && role==Qt::EditRole)
//    {
//        if(value.toString().trimmed() == "")
//        {
//            emit sigErrorWhileValidating(QString("Пустое значение запрещено!"));
//            return false;    //можно испустить какой-нибудь сигнал
//        }
//        DomainNode *node=nodeFromIndex(index);
//        Domain *domain=node->domain;
//        if(!domain)
//            qt_assert("нету домена","",109);
//        switch(node->type)
//        {
//        case DomainNode::Root:
//            break;
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
//        case DomainNode::DomainValue:
//            if(!valueExists(domain,value.toString().trimmed()))
//            {
//                emit sigDomainValueChanged(domain->name.trimmed(),domain->values.data()[index.row()].trimmed(),value.toString().trimmed());//сигнал об изменении значения
//                domain->values.data()[index.row()]= value.toString().trimmed();
//                emit dataChanged(index,index);
//                emit sigDataChanged();
//                return true;
//            }
//            else
//            {
//                emit sigValueExists(index,value);
//                emit sigErrorWhileValidating(QString("Значение домена %1 с именем %2 уже есть!").arg("'"+domain->name+"'").arg("'"+value.toString()+"'"));
//            }
//            break;
//        default:
//            qDebug()<<"DomainModel::setData// case default:";
//            break;
//        }
//    }
    return false;
}
Qt::ItemFlags
NFramenetModel::flags(const QModelIndex &index) const
{
//    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
//    return flags;
    Qt::ItemFlags flag = Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsEnabled | Qt::ItemIsSelectable ;
    if(itemsIsEditable)
        flag|= Qt::ItemIsEditable;
    return flag;
}
bool
NFramenetModel::removeRow(int row, const QModelIndex &parent)
{
//    if (row >= 0 && row < rowCount(parent))
//    {
//        beginRemoveRows(parent, row, row);

//        DomainNode *parentNode = nodeFromIndex(parent);
//        DomainNode *node = parentNode->children.at(row);
//        Domain *domain=NULL;
//        int inx;
//        switch(parentNode->type)
//        {
//        case DomainNode::Root:          //удаляем домены
//            domain=node->domain;
//            emit sigDomainDeleted(domain->name);                    //оповещение об удалении домена(пока не удален)
//            inx = domains->indexOf(domain);
//           // domains->remove(inx);
//            domains->removeAt(inx);
//            parentNode->children.removeAt(row);//delete?
//            emit sigDataChanged();
//            break;
//        case DomainNode::DomainName:    //удаляем значения
//            parentNode->children.removeAt(row);
//            domain=parentNode->domain;
//            emit sigDomainValueDeleted(domain->name, domain->values.at(row));    //сигнал об удалении значения
//            domain->values.remove(row);
//            emit sigDataChanged();
//            break;
//        case DomainNode::DomainValue:
//            break;
//        }

//        endRemoveRows();
//        return true;
//    }
    return false;
}
bool
NFramenetModel::insertRow(int row, const QModelIndex &parent)
{
//    if (row >= 0)
//    {
//        beginInsertRows(parent,row,row);

//        DomainNode *parentNode = nodeFromIndex(parent);
//        DomainNode *node;
//        Domain *domain=NULL;
//        Domain *newDomain=NULL;

//        switch(parentNode->type)
//        {
//        case DomainNode::Root://добавляем домены
//            newDomain = new Domain(generateNewUniqueDomainName());
//            node=new DomainNode(DomainNode::DomainName,newDomain,true,rootNode);
//            //this->domains->append(newDomain);
//            domains->insert(row,newDomain);
//            emit sigDataChanged();
//            break;
//        case DomainNode::DomainName://добавляем значения
//            domain = parentNode->domain;
//            node=new DomainNode(DomainNode::DomainValue,domain,true,parentNode);
//            domain->values.insert(row,generateNewUniqueDomainValue(domain));
//            emit sigDataChanged();
//            break;
//        case DomainNode::DomainValue:
//            break;
//        }
//        endInsertRows();
//        return true;
//    }
    return false;
}

void
NFramenetModel::setFrames( QList<NFrame *> *frames )
{
//    qDebug()<<"DomainModel::setDomains";
//    //здесь делаем разбор списка доменов
//    //строим на основе списка дерево DomainNode
//    //this->domains->clear();//хотя может надо удалить?

//    this->domains=domains;
//    rootNode->children.clear();


//    Domain *domain;
//    foreach(domain,*domains)
//    {
//        DomainNode *nameNode=new DomainNode(DomainNode::DomainName,domain,true,rootNode);
//        //значения
//        DomainNode *valNode;
//        QString val;
//        foreach(val,domain->values)
//        {
//            valNode=new DomainNode(DomainNode::DomainValue,domain,true,nameNode);
//        }
//    }
//    reset();
//    emit sigDataChanged();
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
