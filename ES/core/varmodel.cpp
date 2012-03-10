#include "varmodel.h"
#include <QDebug>

VarModel::VarModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    rootNode=new VarNode(VarNode::Root,NULL,NULL);
}
VarModel::~VarModel()
{
    delete rootNode;
}
void
VarModel::setRootNode(VarNode *node)
{
    delete rootNode;
    rootNode=node;
    reset();
}
QModelIndex
VarModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!rootNode||row<0||column<0)
        return QModelIndex();
    VarNode *parentNode=nodeFromIndex(parent);
    VarNode *childNode=parentNode->children.value(row);
    if(!childNode)
        return QModelIndex();
    return createIndex(row,0,childNode);
}
VarNode*
VarModel::nodeFromIndex(const QModelIndex &index) const
{
    if(index.isValid())
    {
        return static_cast<VarNode*>(index.internalPointer());
    }
    else
    {
        return rootNode;
    }
}
int
VarModel::rowCount(const QModelIndex &parent) const
{
    if(parent.column()>0)
        return 0;
    VarNode *parentNode=nodeFromIndex(parent);
    if(!parentNode)
        return 0;
    return parentNode->children.count();
}
int
VarModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}
QModelIndex
VarModel::parent(const QModelIndex &child) const
{
    VarNode *node=nodeFromIndex(child);
    if(!node)
        return QModelIndex();
    VarNode *parentNode=node->parent;
    if(!parentNode)
        return QModelIndex();
    VarNode *grandParentNode=parentNode->parent;
    if(!grandParentNode)
        return QModelIndex();
    int row=grandParentNode->children.indexOf(parentNode);
    return createIndex(row,0,parentNode);
}
QVariant
VarModel::data(const QModelIndex &index, int role) const
{
    if(role!= Qt::DisplayRole && role!=Qt::EditRole )
        return QVariant();
    VarNode *node=nodeFromIndex(index);
    if(!node)
        return QVariant();
    if(index.column()==0)
    {
        if(role==Qt::EditRole)          //при редактировании
        {
            if(node->type==VarNode::VarName)
            {
                return node->var->name;
            }
        }
        //отображение
        switch(node->type)
        {
        case VarNode::Root:
            return "root";
        case VarNode::VarName:
            return node->var->name;
        case VarNode::VarDerivable:
            return node->var->derivable;
        case VarNode::VarAsking:
            return node->var->asking;
        case VarNode::VarDomain:
            return node->var->domain;
        case VarNode::VarQuestion:
            return node->var->question;
        default:
            return "Unkn";
        }
    }
    return QVariant();
}
bool
VarModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role==Qt::EditRole)
    {
        VarNode *node=nodeFromIndex(index);
        Variable *var=node->var;
        if(!var)
            qt_assert("нету переменной","",109);
        switch(node->type)
        {
        case VarNode::Root:
            break;
        case VarNode::VarName:
            if(value.toString().trimmed() == "")
            {
                emit sigErrorWhileValidating(QString("Переменная с пустым именем запрещена!"));
                return false;    //можно испустить какой-нибудь сигнал
            }
            if(!variableExists(value.toString()))
            {
                emit sigVarNameChanged(var->name.trimmed(),value.toString().trimmed());         //сигнал об изменении имени переменной
                var->name=value.toString().trimmed();
                emit sigDataChanged();
                emit dataChanged(index,index);
                return true;
            }
            else
            {
                emit sigValueExists(index,value);
                emit sigErrorWhileValidating(QString("Переменная с именем %1 уже есть!").arg("'"+value.toString()+"'"));
            }
            break;
        case VarNode::VarDerivable:
            var->derivable=value.toBool();
            emit dataChanged(index,index);
            emit sigDataChanged();
            return true;
            break;
        case VarNode::VarAsking:
            var->asking=value.toBool();
            emit dataChanged(index,index);
            emit sigDataChanged();
            return true;
            break;
        case VarNode::VarQuestion:
            var->question=value.toString();
            emit dataChanged(index,index);
            emit sigDataChanged();
            return true;
            break;
        case VarNode::VarDomain:
            emit sigVarDomainChanged(var->name.trimmed(),var->domain.trimmed(),value.toString().trimmed());   //сигнал об изменении домена переменной
            var->domain=value.toString().trimmed();
            emit dataChanged(index,index);
            emit sigDataChanged();
            return true;
            break;
        default:
            qDebug()<<"VarModel::setData// case default:";
            break;
        }
    }
    return false;
}
Qt::ItemFlags
VarModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flag =Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if(itemsIsEditable)
        flag|= Qt::ItemIsEditable;
    return  flag;
}
bool
VarModel::removeRow(int row, const QModelIndex &parent)
{
    if (row >= 0 && row < rowCount(parent))
    {
        beginRemoveRows(parent, row, row);

        VarNode *parentNode = nodeFromIndex(parent);
        VarNode *node = parentNode->children.at(row);
        Variable *var=NULL;
        int inx;
        switch(parentNode->type)
        {
        case VarNode::Root://удаляем переменную
            var=node->var;
            emit sigVarDeleted(var->name);          //сигнал об удалении переменной
            inx = variables->indexOf(var);
            variables->removeAt(inx);
            parentNode->children.removeAt(row);//delete?
            emit sigDataChanged();
            break;
        case VarNode::VarName://удаляем значения???????????
            parentNode->children.removeAt(row);
            emit sigDataChanged();
            break;

        }
        endRemoveRows();
        return true;
    }
    return false;
}
bool
VarModel::insertRow(int row, const QModelIndex &parent)
{
    if (row >= 0)
    {
        beginInsertRows(parent,row,row);

        VarNode *parentNode = nodeFromIndex(parent);
        VarNode *node;
        VarNode *attrNode;
        Variable *newVar=NULL;

        switch(parentNode->type)
        {
        case VarNode::Root://добавляем переменную
            newVar = new Variable(generateNewUniqueVarName());
            newVar->derivable = true;
            node=new VarNode(VarNode::VarName,newVar,true,rootNode);
            this->variables->append(newVar);

            attrNode = new VarNode(VarNode::VarAsking,newVar,true,node);
            attrNode = new VarNode(VarNode::VarDerivable,newVar,true,node);
            attrNode = new VarNode(VarNode::VarDomain,newVar,true,node);
            attrNode = new VarNode(VarNode::VarQuestion,newVar,true,node);

            emit sigDataChanged();
            break;
//        case DomainNode::DomainName://добавляем значения
//            domain = parentNode->domain;
//            node=new DomainNode(DomainNode::DomainValue,domain,true,parentNode);
//            domain->values.insert(row,"defaultValue");
//            break;
//        case DomainNode::DomainValue:
//            break;
        default:
            return false;
        }
        endInsertRows();
        return true;
    }
    return false;
}

void
VarModel::setVariables(QList<Variable *> *variables)
{
    qDebug()<<"VarModel::setVariables";
    //здесь делаем разбор списка переменных
    //строим на основе списка дерево DomainNode
    //this->domains->clear();//хотя может надо удалить?

    this->variables=variables;
    rootNode->children.clear();

    Variable *var;
    VarNode *attrNode;

    foreach(var,*variables)
    {
        VarNode *varNameNode = new VarNode(VarNode::VarName,var,true,rootNode);

        attrNode = new VarNode(VarNode::VarAsking,var,true,varNameNode);
        attrNode = new VarNode(VarNode::VarDerivable,var,true,varNameNode);
        attrNode = new VarNode(VarNode::VarDomain,var,true,varNameNode);
        attrNode = new VarNode(VarNode::VarQuestion,var,true,varNameNode);

    }

    reset();
    emit sigDataChanged();
}

Qt::DropActions
VarModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QModelIndex
VarModel::addVariable()
{
    int rowCount=variables->count();

    if(!insertRow(rowCount,QModelIndex()))
        return QModelIndex();
    return createIndex(rowCount,0,rootNode->children.at(rowCount));
}

void
VarModel::deleteVariable(QModelIndex index)
{
    if(!index.isValid())
        return;
    removeRow(index.row(),parent(index));
}
bool
VarModel::variableExists(QString varName)
{
    bool exists=false;
    Variable *var=NULL;

    foreach(var,*(this->variables))
    {
        if(var->name.trimmed().compare(varName.trimmed(),Qt::CaseInsensitive)==0)  ///(var->name == varName)
        {
            exists = true;
            break;
        }
    }
    return exists;
}



QModelIndex
VarModel::indexByName(QString varName)
{
    for(int i=0;i<variables->count();++i)
    {
        if(variables->at(i)->name == varName)
        {
            return createIndex(i,0,rootNode->children.at(i));
        }
    }
    return QModelIndex();
}

QDomElement
VarModel::toXml(QDomDocument& doc)
{
    QDomElement vars = doc.createElement("vars");
    {
        Variable *v;
        foreach(v,*(this->variables))
        {
            QDomElement var = doc.createElement("var");
            var.setAttribute("Name",v->name);
            vars.appendChild(var);
            {
                QDomElement derivable = doc.createElement("derivable");   var.appendChild(derivable);
                {
                    QDomText derivable_text = doc.createTextNode(QVariant( v->derivable).toString());   derivable.appendChild(derivable_text);
                }
                QDomElement asking = doc.createElement("asking");   var.appendChild(asking);
                {
                    QDomText asking_text = doc.createTextNode(QVariant(v->asking).toString());   asking.appendChild(asking_text);
                }
                QDomElement domain = doc.createElement("domain");   var.appendChild(domain);
                {
                    QDomText domain_text = doc.createTextNode(v->domain);   domain.appendChild(domain_text);
                }
                QDomElement question = doc.createElement("question");   var.appendChild(question);
                {
                    QDomText question_text = doc.createTextNode(v->question);   question.appendChild(question_text);
                }
            }
        }
    }
    return vars;
}
void
VarModel::fromXml(QDomElement &vars)
{
    if(!vars.hasChildNodes())
        return;
    QList<Variable*> *vs = new QList<Variable*>();
    QDomElement var = vars.firstChild().toElement();
    while(!var.isNull())
    {
        //домен
        Variable* v = new Variable(var.attribute("Name"));
        vs->append(v);

        QDomElement var_derivable = var.firstChildElement("derivable");
        {
            QDomText    var_derivable_text = var_derivable.firstChild().toText();
            v->derivable = QVariant(var_derivable_text.nodeValue()).toBool();
        }
        QDomElement var_asking= var.firstChildElement("asking");
        {
            QDomText    var_asking_text = var_asking.firstChild().toText();
            v->asking = QVariant(var_asking_text.nodeValue()).toBool();
        }
        QDomElement var_domain = var.firstChildElement("domain");
        {
            QDomText    var_domain_text = var_domain.firstChild().toText();
            v->domain = var_domain_text.nodeValue();
        }
        QDomElement var_question = var.firstChildElement("question");
        {
            QDomText    var_question_text = var_question.firstChild().toText();
            v->question = var_question_text.nodeValue();
        }

        var=var.nextSibling().toElement();
    }

    if(vs->count()>0)
    {
        this->setVariables(vs);
    }
}

void
VarModel::onDomainNameChanged(QString oldDomain, QString newDomain)
{
    qDebug()<<"VarModel::onDomainNameChanged "<<oldDomain <<newDomain;
    Variable *v;
    foreach(v,*variables)
    {
        if(v->domain == oldDomain)
        {
            v->domain = newDomain;
            qDebug()<<">>Change: "<<v->name<<" " <<newDomain;
        }
    }
}

void
VarModel::onDomainDeleted(QString domainName)
{
    qDebug()<<"VarModel::onDomainDeleted "<<domainName;

    QStringList varsToDelete;                   //запоминаем все переменные для удаления
    Variable *v;
    foreach(v,*variables)
    {
        if(v->domain == domainName)
        {
            varsToDelete<<v->name;              //добавляем переменную в список на удаление
        }
    }
    QString s;
    foreach(s,varsToDelete)
    {
        QModelIndex varIndex = indexByName(s);
        deleteVariable(varIndex);
    }

}

bool
VarModel::isValid()
{
    bool valid = true;
    QStringList varsList;

    Variable*   v;
    foreach(v,*variables)
        varsList<<v->name;

    if(!Validator::NoCopies(varsList))//нет повторяющихся имен переменных
    {
        valid = false;
        emit sigErrorWhileValidating(QString("Повторяющиеся имена переменных"));
    }

    //у переменной обязательно должен быть домен
    if(valid)
    {
        foreach(v,*variables)
        {
            if(v->domain.isEmpty())
            {
                valid = false;
                emit sigErrorWhileValidating(QString("У переменной %1 нет домена").arg(v->name));
                break;
            }
        }
    }
    //нельзя, чтобы переменная была и невыводимой, и незапрашиваемой
    if(valid)
    {
        foreach(v,*variables)
        {
            if(v->asking==false && v->derivable == false)
            {
                valid = false;
                emit sigErrorWhileValidating(QString("Переменная %1 невыводимая и незапрашиваемая").arg(v->name));
                break;
            }
        }
    }

    return valid;
}

void
VarModel::setItemsIsEditable(bool isEditable)
{
    itemsIsEditable=isEditable;
}



//Drag'&'Drop

void VarModel::moveBefore(QModelIndex sourceInx,QModelIndex destInx)
{
    if(!sourceInx.isValid())
        return;
    if(!destInx.isValid())
        return;
    QModelIndex parentInx = parent(sourceInx);
    VarNode* parentNode = nodeFromIndex(parentInx);
    if(!parentNode)
        return;
    qDebug()<<"sourceInx:"<<sourceInx.row()<<" destInx:"<<destInx.row();
    qDebug()<<"VarModel::: "<<data(sourceInx,Qt::DisplayRole).toString()<<" "<<data(destInx,Qt::DisplayRole).toString();
    switch(parentNode->type)
    {
    case VarNode::Root://перетаскиваем правила
        if(sourceInx.row()>destInx.row() )
        {
            parentNode->children.move(sourceInx.row(),destInx.row());
            variables->move(sourceInx.row(),destInx.row());
        }
        else
        {
            parentNode->children.move(sourceInx.row(),destInx.row()-1);
            variables->move(sourceInx.row(),destInx.row()-1);
        }
        break;
    }

    emit sigDataChanged();
}


QStringList VarModel::mimeTypes() const
 {
     QStringList types;
     types << "application/qmodelindex";
     return types;
 }
QMimeData *VarModel::mimeData(const QModelIndexList &indexes) const
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
bool VarModel::dropMimeData(const QMimeData *data,
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


//generate unicue names

QString
VarModel::generateNewUniqueVarName()
{
    bool exists=true;
    QString base = "Var";
    int i=1;

    while(exists)
    {
        QString res = base+QString::number(i);

        if(!variableExists(res.trimmed()))
        {
            return res;
        }
        ++i;
    }
    return "";
}
