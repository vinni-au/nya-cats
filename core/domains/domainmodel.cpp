#include "domainmodel.h"
#include <QDebug>

DomainModel::DomainModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    rootNode=new DomainNode(DomainNode::Root,NULL,NULL);
    itemsIsEditable = true;
}
DomainModel::~DomainModel()
{
    delete rootNode;
}
void
DomainModel::setRootNode(DomainNode *node)
{
    delete rootNode;
    rootNode=node;
    reset();
}
QModelIndex
DomainModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!rootNode||row<0||column<0)
        return QModelIndex();
    DomainNode *parentNode=nodeFromIndex(parent);
    DomainNode *childNode=parentNode->children.value(row);
    if(!childNode)
        return QModelIndex();
    return createIndex(row,0,childNode);
}
DomainNode*
DomainModel::nodeFromIndex(const QModelIndex &index) const
{
    if(index.isValid())
    {
        return static_cast<DomainNode*>(index.internalPointer());
    }
    else
    {
        return rootNode;
    }
}
int
DomainModel::rowCount(const QModelIndex &parent) const
{
    if(parent.column()>0)
        return 0;
    DomainNode *parentNode=nodeFromIndex(parent);
    if(!parentNode)
        return 0;
    return parentNode->children.count();
}
int
DomainModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}
QModelIndex
DomainModel::parent(const QModelIndex &child) const
{
    DomainNode *node=nodeFromIndex(child);
    if(!node)
        return QModelIndex();
    DomainNode *parentNode=node->parent;
    if(!parentNode)
        return QModelIndex();
    DomainNode *grandParentNode=parentNode->parent;
    if(!grandParentNode)
        return QModelIndex();
    int row=grandParentNode->children.indexOf(parentNode);
    return createIndex(row,0,parentNode);
}
QVariant
DomainModel::data(const QModelIndex &index, int role) const
{
    if(role!= Qt::DisplayRole && role!=Qt::EditRole )
        return QVariant();

    DomainNode *node=nodeFromIndex(index);
    if(!node)
        return QVariant();

    if(index.column()==0)
    {
        if(role==Qt::EditRole)          //при редактировании
        {
            if(node->type==DomainNode::DomainName)
            {
                return node->domain->name;
            }
            else if(node->type==DomainNode::DomainValue)
            {
                return node->domain->values.at(index.row());
            }
        }

        //отображение
        switch(node->type)
        {
        case DomainNode::Root:
            return "root";

        case DomainNode::DomainName:
            return node->domain->name;

        case DomainNode::DomainValue:
            return node->domain->values.at(index.row());
        default:
            return "Unkn";
        }
    }
    return QVariant();
}
bool
DomainModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role==Qt::EditRole)
    {
        if(value.toString().trimmed() == "")
        {
            emit sigErrorWhileValidating(QString("Пустое значение запрещено!"));
            return false;    //можно испустить какой-нибудь сигнал
        }
        DomainNode *node=nodeFromIndex(index);
        Domain *domain=node->domain;
        if(!domain)
            qt_assert("нету домена","",109);
        switch(node->type)
        {
        case DomainNode::Root:
            break;
        case DomainNode::DomainName:
            if(!domainExists(value.toString().trimmed()))
            {
                emit sigDomainNameChanged(domain->name.trimmed(),value.toString().trimmed());       //сигнал об изменении имени домена
                domain->name=value.toString().trimmed();
                emit dataChanged(index,index);
                emit sigDataChanged();
                return true;
            }
            else
            {
                emit sigValueExists(index,value);
                emit sigErrorWhileValidating(QString("Домен с именем %1 уже есть!").arg("'"+value.toString()+"'"));
            }
            break;
        case DomainNode::DomainValue:
            if(!valueExists(domain,value.toString().trimmed()))
            {
                emit sigDomainValueChanged(domain->name.trimmed(),domain->values.data()[index.row()].trimmed(),value.toString().trimmed());//сигнал об изменении значения
                domain->values.data()[index.row()]= value.toString().trimmed();
                emit dataChanged(index,index);
                emit sigDataChanged();
                return true;
            }
            else
            {
                emit sigValueExists(index,value);
                emit sigErrorWhileValidating(QString("Значение домена %1 с именем %2 уже есть!").arg("'"+domain->name+"'").arg("'"+value.toString()+"'"));
            }
            break;
        default:
            qDebug()<<"DomainModel::setData// case default:";
            break;
        }
    }
    return false;
}
Qt::ItemFlags
DomainModel::flags(const QModelIndex &index) const
{
//    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
//    return flags;
    Qt::ItemFlags flag = Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsEnabled | Qt::ItemIsSelectable ;
    if(itemsIsEditable)
        flag|= Qt::ItemIsEditable;
    return flag;
}
bool
DomainModel::removeRow(int row, const QModelIndex &parent)
{
    if (row >= 0 && row < rowCount(parent))
    {
        beginRemoveRows(parent, row, row);

        DomainNode *parentNode = nodeFromIndex(parent);
        DomainNode *node = parentNode->children.at(row);
        Domain *domain=NULL;
        int inx;
        switch(parentNode->type)
        {
        case DomainNode::Root:          //удаляем домены
            domain=node->domain;
            emit sigDomainDeleted(domain->name);                    //оповещение об удалении домена(пока не удален)
            inx = domains->indexOf(domain);
           // domains->remove(inx);
            domains->removeAt(inx);
            parentNode->children.removeAt(row);//delete?
            emit sigDataChanged();
            break;
        case DomainNode::DomainName:    //удаляем значения
            parentNode->children.removeAt(row);
            domain=parentNode->domain;
            emit sigDomainValueDeleted(domain->name, domain->values.at(row));    //сигнал об удалении значения
            domain->values.remove(row);
            emit sigDataChanged();
            break;
        case DomainNode::DomainValue:
            break;
        }

        endRemoveRows();
        return true;
    }
    return false;
}
bool
DomainModel::insertRow(int row, const QModelIndex &parent)
{
    if (row >= 0)
    {
        beginInsertRows(parent,row,row);

        DomainNode *parentNode = nodeFromIndex(parent);
        DomainNode *node;
        Domain *domain=NULL;
        Domain *newDomain=NULL;

        switch(parentNode->type)
        {
        case DomainNode::Root://добавляем домены
            newDomain = new Domain(generateNewUniqueDomainName());
            node=new DomainNode(DomainNode::DomainName,newDomain,true,rootNode);
            //this->domains->append(newDomain);
            domains->insert(row,newDomain);
            emit sigDataChanged();
            break;
        case DomainNode::DomainName://добавляем значения
            domain = parentNode->domain;
            node=new DomainNode(DomainNode::DomainValue,domain,true,parentNode);
            domain->values.insert(row,generateNewUniqueDomainValue(domain));
            emit sigDataChanged();
            break;
        case DomainNode::DomainValue:
            break;
        }
        endInsertRows();
        return true;
    }
    return false;
}

void
DomainModel::setDomains( QList<Domain *> *domains )
{
    qDebug()<<"DomainModel::setDomains";
    //здесь делаем разбор списка доменов
    //строим на основе списка дерево DomainNode
    //this->domains->clear();//хотя может надо удалить?

    this->domains=domains;
    rootNode->children.clear();


    Domain *domain;
    foreach(domain,*domains)
    {
        DomainNode *nameNode=new DomainNode(DomainNode::DomainName,domain,true,rootNode);
        //значения
        DomainNode *valNode;
        QString val;
        foreach(val,domain->values)
        {
            valNode=new DomainNode(DomainNode::DomainValue,domain,true,nameNode);
        }
    }
    reset();
    emit sigDataChanged();
}

Qt::DropActions
DomainModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

QModelIndex
DomainModel::addDomain()
{
    int rowCount=domains->count();

    if(!insertRow(rowCount,QModelIndex()))
        return QModelIndex();
    return createIndex(rowCount,0,rootNode->children.at(rowCount));
    //////

//    Domain *newDomain = new Domain("def");
//    DomainNode *node=new DomainNode(DomainNode::DomainName,newDomain,true,rootNode);
//    this->domains->append(newDomain);

//    QModelIndex index = createIndex(domains->count()-1,0,node );
//    emit this->dataChanged(index,index);

//    return index;
}
void
DomainModel::editDomain(QModelIndex domain, QString newName)
{
    DomainNode *node=nodeFromIndex(domain);
    node->domain->name=newName;
    emit sigDataChanged();
}
void
DomainModel::deleteDomain(QModelIndex index)
{
    if(!index.isValid())
        return;

    removeRow(index.row(),parent(index));


//    DomainNode *node=nodeFromIndex(index);
//    if(!node)
//        return;
//    Domain *domain = node->domain;
//    if(!domain)
//        return;
//    int pos = domains->indexOf(domain);
//    domains->remove(pos);//удалили из списка

//    delete domain;

//    delete node;

//    reset();
    //emit dataChanged(index,index);

}
bool
DomainModel::domainExists(QString domainName)
{
    bool exists=false;
    Domain *domain=NULL;

    foreach(domain,*(this->domains))
    {
        if(domain->name.trimmed().compare(domainName.trimmed(),Qt::CaseInsensitive)==0)//(domain->name == domainName)
        {
            exists = true;
            break;
        }
    }
    return exists;
}


QModelIndex
DomainModel::addValue(QModelIndex domainIndex)
{
    if(!domainIndex.isValid())
        return QModelIndex();

    DomainNode *node = nodeFromIndex(domainIndex);
    if(!node)
        return QModelIndex();
    int rowCount = node->children.count();
    if(!insertRow(rowCount,domainIndex))
        return QModelIndex();
    return createIndex(rowCount,0,node->children.at(rowCount));
///----------------
//    DomainNode *domainNameNode = nodeFromIndex(domainIndex);
//    if(!domainNameNode)
//        return QModelIndex();

//    int rowCount = domainNameNode->children.count();

//    insertRow(rowCount,domainIndex);

//    Domain *domain = domainNameNode->domain;

//    //domain->values.append();
//    //
//    Domain *newDomain = new Domain("def");
//    DomainNode *node=new DomainNode(DomainNode::DomainName,newDomain,true,rootNode);
//    this->domains->append(newDomain);

//    QModelIndex index = createIndex(domains->count()-1,0,node );
//    emit this->dataChanged(index,index);

//    return index;
}
void
DomainModel::deleteValue(QModelIndex valueIndex)
{
    if(!valueIndex.isValid())
        return;
    removeRow(valueIndex.row(),parent(valueIndex));
}
bool
DomainModel::valueExists(QString domainName,QString value)
{
    bool exists=false;
    Domain *domain=NULL;

    foreach(domain,*(this->domains))
    {
        if(domain->name == domainName)
        {
            for(int i =0;i<domain->values.count();++i)
            {
                if(domain->values[i].trimmed().compare(value.trimmed(),Qt::CaseInsensitive)==0)
                {
                    exists = true;
                    break;
                }
            }
//            if(domain->values.contains(value))
//            {
//                exists = true;
//                break;
//            }
        }
    }
    return exists;
}
bool
DomainModel::valueExists(Domain *domain,QString value)
{
    bool exists=false;

    for(int i =0;i<domain->values.count();++i)
    {
        if(domain->values[i].trimmed().compare(value.trimmed(),Qt::CaseInsensitive)==0)
        {
            exists = true;
            break;
        }
    }

//    if(domain->values.contains(value))
//    {
//        exists = true;
//    }
    return exists;
}
QModelIndex
DomainModel::indexByName(QString domain)
{
    for(int i=0;i<domains->count();++i)
    {
        if(domains->at(i)->name == domain)
        {
            return createIndex(i,0,rootNode->children.at(i));
        }
    }
    return QModelIndex();
}
QDomElement
DomainModel::toXml(QDomDocument& doc)
{
    QDomElement domains = doc.createElement("domains"); //doc.appendChild(domains);
    {
        Domain *d;
        foreach(d,*(this->domains))
        {
            QDomElement domain = doc.createElement("domain");
            domain.setAttribute("Name",d->name);
            domains.appendChild(domain);
            {
                QString v;
                foreach(v,d->values)
                {
                    QDomElement domain_value = doc.createElement("domain_value");   domain.appendChild(domain_value);
                    {
                        QDomText domain_value_text = doc.createTextNode(v);   domain_value.appendChild(domain_value_text);
                    }
                }
            }
        }
    }
    return domains;
}

void
DomainModel::fromXml(QDomElement &domains)
{
    if(!domains.hasChildNodes())
        return;
    QList<Domain*> *ds = new QList<Domain*>();
    QDomElement domain = domains.firstChild().toElement();
    while(!domain.isNull())
    {
        //домен
        Domain* d = new Domain(domain.attribute("Name"));
        ds->append(d);

        QDomElement domain_value = domain.firstChild().toElement();
        while(!domain_value.isNull())
        {
            //значение
            d->values.append(domain_value.firstChild().toText().nodeValue());
            domain_value=domain_value.nextSibling().toElement();
        }
        domain=domain.nextSibling().toElement();
    }

    if(ds->count()>0)
    {
        this->setDomains(ds);
    }
}


bool
DomainModel::isValid()
{
    bool isValid = true;
    Domain *d;
    QStringList domainsStrList;
    foreach(d,*domains)
    {
          domainsStrList<<d->name;

          if(!Validator::NoCopies( d->values.toList() ))
          {
              isValid = false;
              emit sigErrorWhileValidating(QString("Повторяющиеся значения в домене %1").arg(d->name));
              break;
          }
    }
    if(isValid)
    {
        if(!Validator::NoCopies( domainsStrList))
        {
            isValid = false;
            emit sigErrorWhileValidating(QString("Повторяющиеся имена доменов"));
        }
    }

    return isValid;
}

void
DomainModel::setItemsIsEditable(bool isEditable)
{
    itemsIsEditable = isEditable;
}


//Drag'&'Drop

void DomainModel::moveBefore(QModelIndex sourceInx,QModelIndex destInx)
{
    if(!sourceInx.isValid())
        return;
    if(!destInx.isValid())
        return;
    QModelIndex parentInx = parent(sourceInx);
    DomainNode* parentNode = nodeFromIndex(parentInx);
    if(!parentNode)
        return;
    qDebug()<<"sourceInx:"<<sourceInx.row()<<" destInx:"<<destInx.row();
    qDebug()<<"DomainModel::: "<<data(sourceInx,Qt::DisplayRole).toString()<<" "<<data(destInx,Qt::DisplayRole).toString();
    if(parentNode->type == DomainNode::Root)//перетаскиваем домены
    {
        if(sourceInx.row()>destInx.row() )
        {
            parentNode->children.move(sourceInx.row(),destInx.row());
            domains->move(sourceInx.row(),destInx.row());
        }
        else
        {
            parentNode->children.move(sourceInx.row(),destInx.row()-1);
            domains->move(sourceInx.row(),destInx.row()-1);
        }
    }
    else//перетаскиваем значения
    {
        if(sourceInx.row()>destInx.row() )
        {
            parentNode->children.move(sourceInx.row(),destInx.row());
            QString tmp = parentNode->domain->values.at(sourceInx.row());
            parentNode->domain->values.remove(sourceInx.row());
            parentNode->domain->values.insert(destInx.row(),tmp);
        }
        else
        {
            parentNode->children.move(sourceInx.row(),destInx.row()-1);
            QString tmp = parentNode->domain->values.at(sourceInx.row());
            parentNode->domain->values.remove(sourceInx.row());
            parentNode->domain->values.insert(destInx.row()-1,tmp);
        }
    }
    emit sigDataChanged();
}


QStringList DomainModel::mimeTypes() const
 {
     QStringList types;
     types << "application/qmodelindex";
     return types;
 }
QMimeData *DomainModel::mimeData(const QModelIndexList &indexes) const
 {
     QMimeData *mimeData = new QMimeData();
     QByteArray encodedData;
     QDataStream stream(&encodedData, QIODevice::WriteOnly);
     foreach (const QModelIndex &index, indexes)
     {
         if (index.isValid())
         {
             const QModelIndex* pInx;
             pInx = &index;
             char * pChar = (char*)pInx;
             QByteArray array(pChar,sizeof(QModelIndex));
             stream << array;
         }
     }
     mimeData->setData("application/qmodelindex", encodedData);
     return mimeData;
 }
bool DomainModel::dropMimeData(const QMimeData *data,
                                     Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;
    if (!data->hasFormat("application/qmodelindex"))
        return false;
    if (column > 0)
        return false;
    QByteArray encodedData = data->data("application/qmodelindex");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QModelIndex *sourceIndex;
    QByteArray array;
    stream>>array;
    sourceIndex= (QModelIndex *)array.data();
    QModelIndex destIndex;
    if(row==-1)
    {
        destIndex = parent;
    }
    else
    {
        destIndex=index(row,0,parent);
    }
    moveBefore(*sourceIndex,destIndex);
    return true;
}


//generate unique names
QString
DomainModel::generateNewUniqueDomainName()
{
    bool exists=true;
    QString base = "Domain";
    int i=1;

    while(exists)
    {
        QString res = base+QString::number(i);

        if(!domainExists(res.trimmed()))
        {
            return res;
        }
        ++i;
    }
    return "";
}

QString
DomainModel::generateNewUniqueDomainValue(Domain *domain)
{
    bool exists=true;
    QString base = "Value";
    int i=1;

    while(exists)
    {
        QString res = base+QString::number(i);

        if(!valueExists(domain,res.trimmed()))
        {
            return res;
        }
        ++i;
    }
    return "";
}

Domain* DomainModel::getDomainByName(QString name)
{
    for(int i=0;i<domains->count();++i)
    {
        if(domains->at(i)->name == name)
        {
            return domains->at(i);
        }
    }
    return NULL;
}
