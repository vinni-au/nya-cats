#include "rulemodel.h"
#include <QDebug>

RuleModel::RuleModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    rootNode=new RuleNode(RuleNode::Root,NULL,NULL);
    rules = new QList<Rule*>();
}
RuleModel::~RuleModel()
{
    delete rootNode;
}
void
RuleModel::setRootNode(RuleNode *node)
{
    delete rootNode;
    rootNode=node;
    reset();
}
QModelIndex
RuleModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!rootNode||row<0||column<0)
        return QModelIndex();
    RuleNode *parentNode=nodeFromIndex(parent);
    RuleNode *childNode=parentNode->children.value(row);
    if(!childNode)
        return QModelIndex();
    return createIndex(row,0,childNode);
}
RuleNode*
RuleModel::nodeFromIndex(const QModelIndex &index) const
{
    if(index.isValid())
    {
        return static_cast<RuleNode*>(index.internalPointer());
    }
    else
    {
        return rootNode;
    }
}
int
RuleModel::rowCount(const QModelIndex &parent) const
{
    if(parent.column()>0)
        return 0;
    RuleNode *parentNode=nodeFromIndex(parent);
    if(!parentNode)
        return 0;
    return parentNode->children.count();
}
int
RuleModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}
QModelIndex
RuleModel::parent(const QModelIndex &child) const
{
    RuleNode *node=nodeFromIndex(child);
    if(!node)
        return QModelIndex();
    RuleNode *parentNode=node->parent;
    if(!parentNode)
        return QModelIndex();
    RuleNode *grandParentNode=parentNode->parent;
    if(!grandParentNode)
        return QModelIndex();
    int row=grandParentNode->children.indexOf(parentNode);
    return createIndex(row,0,parentNode);
}
QVariant
RuleModel::data(const QModelIndex &index, int role) const
{
    Expr *expr;
    Rule *rule;
    QString str;
    if(role!= Qt::DisplayRole && role!=Qt::EditRole )
        return QVariant();
    RuleNode *node=nodeFromIndex(index);
    if(!node)
        return QVariant();
    if(index.column()==0)
    {

        //отображение
        switch(node->type)
        {
        case RuleNode::Root:
            return "root";
        case RuleNode::RuleName:

            rule=node->rule;

            str=node->rule->name+": ";

            str=str+" if ";

            for(int i=0;i<rule->predicate.count();++i)//ifs
            {
                str=str+" ["+rule->predicate[i]->toString()+"] ";
                if(i<rule->predicate.count()-1)
                    str=str+"AND";
            }

            str=str+" then ";
            foreach(expr,rule->conclusion)
            {
                str=str+" ["+expr->toString()+"] ";
            }


            str=str+ "     Reason: "+rule->reason;
            return str;
            //return node->rule->name;

        case RuleNode::RuleReason:
            return node->rule->reason;

        case RuleNode::RuleConclusion:
            return node->rule->conclusion.at(index.row())->toString();

        case RuleNode::RulePredicate:
            return node->rule->predicate.at(index.row())->toString();
        default:
            return "Unkn";
        }
    }
    return QVariant();
}

bool
RuleModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role==Qt::EditRole)
    {
        RuleNode *node=nodeFromIndex(index);
        Rule *rule=node->rule;
        if(!rule)
            qt_assert("нету правила","",109);
        switch(node->type)
        {
        case RuleNode::Root:
            break;
        case RuleNode::RuleName:
            if(value.toString().trimmed() == "")
            {
                emit sigErrorWhileValidating(QString("Правило с пустым именем запрещено!"));
                return false;    //можно испустить какой-нибудь сигнал
            }
            if(!ruleExists(value.toString().trimmed()))
            {
                rule->name=value.toString().trimmed();
                emit dataChanged(index,index);
                emit sigDataChanged();
                return true;
            }
            else
            {
                emit sigValueExists(index,value);
                emit sigErrorWhileValidating(QString("Правило с именем %1 уже есть!").arg("'"+value.toString()+"'"));
            }
            break;
        case RuleNode::RuleReason:
            rule->reason=value.toString();
            emit dataChanged(index,index);
            emit sigDataChanged();
            return true;
            break;
        case RuleNode::RulePredicate://утечка
            rule->predicate.replace(index.row(),(Expr*)value.toUInt());
            emit dataChanged(index,index);
            emit sigDataChanged();
            return true;
            break;
        case RuleNode::RuleConclusion:
            rule->conclusion.replace(index.row(),(Expr*)value.toUInt());
            emit dataChanged(index,index);
            emit sigDataChanged();
            return true;
            break;
        default:
            qDebug()<<"RuleModel::setData// case default:";
            break;
        }
    }
    return false;
}
Qt::ItemFlags
RuleModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsEnabled | Qt::ItemIsSelectable ;
}
bool
RuleModel::removeRow(int row, const QModelIndex &parent)
{
    if (row >= 0 && row < rowCount(parent))
    {
        beginRemoveRows(parent, row, row);

        RuleNode *parentNode = nodeFromIndex(parent);
        RuleNode *node = parentNode->children.at(row);
        Rule *rule=NULL;
        int inx;
        switch(parentNode->type)
        {
        case RuleNode::Root://удаляем правила
            rule=node->rule;
            inx = rules->indexOf(rule);
            rules->removeAt(inx);
            parentNode->children.removeAt(row);//delete?
            emit sigDataChanged();
            break;
        case RuleNode::RulePredicateHead://удаляем условия
            parentNode->children.removeAt(row);

            rule=node->rule;
            rule->predicate.removeAt(row);
            emit sigDataChanged();
            break;
        case RuleNode::RuleConclusionHead://удаляем заключения
            parentNode->children.removeAt(row);

            rule=node->rule;
            rule->conclusion.removeAt(row);
            emit sigDataChanged();
            break;

        }
        endRemoveRows();
        return true;
    }
    return false;
}
bool
RuleModel::insertRow(int row, const QModelIndex &parent)
{
    if (row >= 0)
    {
        beginInsertRows(parent,row,row);

        RuleNode *parentNode = nodeFromIndex(parent);
        RuleNode *node;
        Rule *newRule=NULL;
        Rule *rule=NULL;

        RuleNode *reasonNode;
        RuleNode *predHeadNode;
        RuleNode *conclHeadNode;

        switch(parentNode->type)
        {
        case RuleNode::Root://добавляем правило
            newRule = new Rule();
            newRule->name=generateNewUniqueRuleName();
            //node=new RuleNode(RuleNode::RuleName,newRule,true,rootNode);
            node=new RuleNode(RuleNode::RuleName,newRule,false,rootNode);
            rootNode->children.insert(row,node);//back link
            //this->rules->append(newRule);
            this->rules->insert(row,newRule);//надо еще наверное ноды вставлять в нужное место

            reasonNode = new RuleNode(RuleNode::RuleReason,newRule,true,node);
            predHeadNode = new RuleNode(RuleNode::RulePredicateHead,newRule,true,node);
            conclHeadNode = new RuleNode(RuleNode::RuleConclusionHead,newRule,true,node);

            emit sigDataChanged();
            break;
        case RuleNode::RulePredicateHead://добавляем условия
            rule=parentNode->rule;
            node=new RuleNode(RuleNode::RulePredicate,rule,true,parentNode);
            rule->predicate.append(new Expr());
            emit sigDataChanged();
            break;
        case RuleNode::RuleConclusionHead://добавляем заключения
            rule=parentNode->rule;
            node=new RuleNode(RuleNode::RuleConclusion,rule,true,parentNode);
            rule->conclusion.append(new Expr());
            emit sigDataChanged();
            break;
        default:
            return false;
        }
        endInsertRows();
        return true;
    }
    return false;
}

void
RuleModel::setRules(QList<Rule *> *rules)
{
    qDebug()<<"RuleModel::setRules";

    this->rules=rules;
    rootNode->children.clear();

    Rule *rule;
    Expr *expr;
    RuleNode *nameNode;
    RuleNode *reasonNode;
    RuleNode *ifNodeHead;
    RuleNode *ifNode;
    RuleNode *thenNodeHead;
    RuleNode *thenNode;

    foreach(rule,*rules)
    {
        nameNode = new RuleNode(RuleNode::RuleName,rule,true,rootNode);

        reasonNode = new RuleNode(RuleNode::RuleReason,rule,true,nameNode);
        ifNodeHead = new RuleNode(RuleNode::RulePredicateHead,rule,true,nameNode);
        foreach(expr,(rule->predicate))
        {
            ifNode = new RuleNode(RuleNode::RulePredicate,rule,true,ifNodeHead);
        }
        thenNodeHead = new RuleNode(RuleNode::RuleConclusionHead,rule,true,nameNode);
        foreach(expr,(rule->conclusion))
        {
            thenNode = new RuleNode(RuleNode::RuleConclusion,rule,true,thenNodeHead);
        }
    }

    reset();
    emit sigDataChanged();
}

Qt::DropActions
RuleModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QModelIndex
RuleModel::addRule()
{
    int rowCount=rules->count();

    if(!insertRow(rowCount,QModelIndex()))
        return QModelIndex();
    return createIndex(rowCount,0,rootNode->children.at(rowCount));
}

QModelIndex
RuleModel::insertRule(QModelIndex afterInddex)
{
    int afterIntIndex = afterInddex.row()+1;
    if(!insertRow(afterIntIndex,QModelIndex()))
        return QModelIndex();
    return createIndex(afterIntIndex,0,rootNode->children.at(afterIntIndex));
}

void
RuleModel::deleteRule(QModelIndex index)
{
    qDebug()<<"RuleModel::deleteRule";
    if(!index.isValid())
        return;
    RuleNode *node=nodeFromIndex(index);
    if(node->type!=RuleNode::RuleName)
    {
        qDebug()<<"не та вершина";
        return;
    }
    removeRow(index.row(),parent(index));
}
bool
RuleModel::ruleExists(QString ruleName)
{
    bool exists=false;
    Rule *rule=NULL;

    foreach(rule,*(this->rules))
    {
        if(rule->name.trimmed().compare(ruleName.trimmed(),Qt::CaseInsensitive)==0)//(rule->name == ruleName)
        {
            exists = true;
            break;
        }
    }
    return exists;
}


//добавить/удалить посылку
QModelIndex
RuleModel::addIf(QModelIndex ruleIndex)
{
    qDebug()<<"RuleModel::addIf";
    if(!ruleIndex.isValid())
        return QModelIndex();
    RuleNode *ruleNode=nodeFromIndex(ruleIndex);
    Rule *rule=ruleNode->rule;

    int rowCount=rule->predicate.count();

    QModelIndex ifHeadIndex = index(1,0,ruleIndex);

    RuleNode *ifHeadNode = nodeFromIndex(ifHeadIndex);

    if(!insertRow(rowCount,ifHeadIndex))
        return QModelIndex();
    return createIndex(rowCount,0,ifHeadNode->children.at(rowCount));
}
void
RuleModel::deleteIf(QModelIndex index)
{
    qDebug()<<"RuleModel::deleteIf";
    if(!index.isValid())
        return;
    RuleNode *node=nodeFromIndex(index);
    if(node->type!=RuleNode::RulePredicate)
    {
        qDebug()<<"не та вершина";
        return;
    }
    removeRow(index.row(),parent(index));
}

//добавить/удалить заключение
QModelIndex
RuleModel::addThen(QModelIndex ruleIndex)
{
    qDebug()<<"RuleModel::addThen";
    if(!ruleIndex.isValid())
        return QModelIndex();
    RuleNode *ruleNode=nodeFromIndex(ruleIndex);
    Rule *rule=ruleNode->rule;

    int rowCount=rule->conclusion.count();

    QModelIndex thenHeadIndex = index(2,0,ruleIndex);

    RuleNode *thenHeadNode = nodeFromIndex(thenHeadIndex);

    if(!insertRow(rowCount,thenHeadIndex))
        return QModelIndex();
    return createIndex(rowCount,0,thenHeadNode->children.at(rowCount));
}
void
RuleModel::deleteThen(QModelIndex index)
{
    qDebug()<<"RuleModel::deleteThen";
    if(!index.isValid())
        return;
    RuleNode *node=nodeFromIndex(index);
    if(node->type!=RuleNode::RuleConclusion)
    {
        qDebug()<<"не та вершина";
        return;
    }
    removeRow(index.row(),parent(index));
}
QString
RuleModel::getRuleName(QModelIndex index)
{
    RuleNode *node = nodeFromIndex(index);
    return node->rule->name;
}
QString
RuleModel::getExprPart(QModelIndex index,RuleModel::ExprPart exprPart)
{
    if(!index.isValid())
        return "";
    RuleNode *node = nodeFromIndex(index);
    if(!node)
        return "";
    if(node->type != RuleNode::RulePredicate && node->type != RuleNode::RuleConclusion)
        return "";

    Rule *rule = node->rule;
    if(!rule)
        return "";

    if(node->type == RuleNode::RulePredicate)
    {
        switch(exprPart)
        {
        case RuleModel::ExprVar:
            return rule->predicate.at(index.row())->var;
            break;
        case RuleModel::ExprSign:
            return rule->predicate.at(index.row())->sign;
            break;
        case RuleModel::ExprVal:
            return rule->predicate.at(index.row())->val;
            break;
        default:
            return "";
            break;
        }
    }
    else
    {
        switch(exprPart)
        {
        case RuleModel::ExprVar:
            return rule->conclusion.at(index.row())->var;
            break;
        case RuleModel::ExprSign:
            return rule->conclusion.at(index.row())->sign;
            break;
        case RuleModel::ExprVal:
            return rule->conclusion.at(index.row())->val;
            break;
        default:
            return "";
            break;
        }
    }

}

QModelIndex
RuleModel::indexByName(QString ruleName)
{
    for(int i=0;i<rules->count();++i)
    {
        if(rules->at(i)->name == ruleName)
            return createIndex(i,0,rootNode->children.at(i));
    }
    return QModelIndex();
}

QDomElement
RuleModel::toXml(QDomDocument& doc)
{
    QDomElement rules = doc.createElement("rules");
    {
        Rule *r;
        foreach(r,*(this->rules))
        {
            QDomElement rule = doc.createElement("rule");
            rule.setAttribute("Name",r->name);
            rules.appendChild(rule);
            {
                QDomElement reason = doc.createElement("reason");   rule.appendChild(reason);
                {
                    QDomText reason_text = doc.createTextNode(r->reason);   reason.appendChild(reason_text);
                }
                QDomElement ifs = doc.createElement("ifs");         rule.appendChild(ifs);
                {
                    Expr *e;
                    foreach(e,r->predicate)
                    {
                        QDomElement if1 = doc.createElement("if");         ifs.appendChild(if1);
                        {
                            QDomElement var = doc.createElement("var");         if1.appendChild(var);
                            {
                                QDomText var_text = doc.createTextNode(e->var);   var.appendChild(var_text);
                            }
                            QDomElement sign = doc.createElement("sign");         if1.appendChild(sign);
                            {
                                QDomText sign_text = doc.createTextNode(e->sign);   sign.appendChild(sign_text);
                            }
                            QDomElement val = doc.createElement("val");         if1.appendChild(val);
                            {
                                QDomText val_text = doc.createTextNode(e->val);   val.appendChild(val_text);
                            }
                        }
                    }
                }
                QDomElement thens = doc.createElement("thens");         rule.appendChild(thens);
                {
                    Expr *e;
                    foreach(e,r->conclusion)
                    {
                        QDomElement then1 = doc.createElement("then");         thens.appendChild(then1);
                        {
                            QDomElement var = doc.createElement("var");         then1.appendChild(var);
                            {
                                QDomText var_text = doc.createTextNode(e->var);   var.appendChild(var_text);
                            }
                            QDomElement sign = doc.createElement("sign");         then1.appendChild(sign);
                            {
                                QDomText sign_text = doc.createTextNode(e->sign);   sign.appendChild(sign_text);
                            }
                            QDomElement val = doc.createElement("val");         then1.appendChild(val);
                            {
                                QDomText val_text = doc.createTextNode(e->val);   val.appendChild(val_text);
                            }
                        }
                    }
                }
            }
        }
    }
    return rules;
}

void
RuleModel::fromXml(QDomElement &rules)
{
    if(!rules.hasChildNodes())
        return;
    QList<Rule*> *rs = new QList<Rule*>();
    QDomElement rule = rules.firstChild().toElement();
    while(!rule.isNull())
    {
        //правило
        Rule* r = new Rule();
        r->name = rule.attribute("Name");
        rs->append(r);

        QDomElement reason = rule.firstChildElement("reason");
        {
            r->reason = reason.firstChild().toText().nodeValue();
        }


        QDomElement ifs = rule.firstChildElement("ifs");
        {
            QDomElement if1 = ifs.firstChild().toElement();
            while(!if1.isNull())
            {
                //условие
                Expr *expr = new Expr();
                expr->var = if1.firstChildElement("var").firstChild().toText().nodeValue();
                expr->sign = if1.firstChildElement("sign").firstChild().toText().nodeValue();
                expr->val = if1.firstChildElement("val").firstChild().toText().nodeValue();
                r->predicate.append(expr);
                if1=if1.nextSibling().toElement();
            }
        }
        QDomElement thens = rule.firstChildElement("thens");
        {
            QDomElement then1 = thens.firstChild().toElement();
            while(!then1.isNull())
            {
                //условие
                Expr *expr = new Expr();
                expr->var = then1.firstChildElement("var").firstChild().toText().nodeValue();
                expr->sign = then1.firstChildElement("sign").firstChild().toText().nodeValue();
                expr->val = then1.firstChildElement("val").firstChild().toText().nodeValue();
                r->conclusion.append(expr);
                then1=then1.nextSibling().toElement();
            }
        }


        rule=rule.nextSibling().toElement();
    }

    if(rs->count()>0)
    {
        this->setRules(rs);
    }
}

void
RuleModel::onDomainValueChanged(QString domainName, QString oldValue, QString newValue)
{
//    qDebug()<<"RuleModel::onDomainValueChanged "<<domainName<<" "<<oldValue<<" "<<newValue;
//    Rule *r;
//    foreach(r,*rules)
//    {
//        Expr *e;
//        foreach(e,r->predicate)
//        {
//            QModelIndex varIndex = DataModels::instance()->varModel->indexByName(e->var);
//            QModelIndex varDomainIndex = DataModels::instance()->varModel->index(2,0,varIndex);
//            QString varDomain = DataModels::instance()->varModel->data(varDomainIndex,Qt::DisplayRole).toString();//получили домен переменной
//            if(domainName == varDomain)
//            {//наш клиент
//                if(oldValue == e->val)
//                    e->val = newValue;
//            }
//        }
//        foreach(e,r->conclusion)
//        {
//            QModelIndex varIndex = DataModels::instance()->varModel->indexByName(e->var);
//            QModelIndex varDomainIndex = DataModels::instance()->varModel->index(2,0,varIndex);
//            QString varDomain = DataModels::instance()->varModel->data(varDomainIndex,Qt::DisplayRole).toString();//получили домен переменной
//            if(domainName == varDomain)
//            {//наш клиент
//                if(oldValue == e->val)
//                    e->val = newValue;
//            }
//        }
//    }
}
void
RuleModel::onDomainValueDeleted(QString domainName,QString value)//при удалении значения удаляем выражение, содержащее его
{
//    qDebug()<<"RuleModel::onDomainValueDeleted "<<" "<<domainName <<" "<<value;
//    Rule *r;
//    foreach(r,*rules)
//    {
//        Expr *e;
//        foreach(e,r->predicate)
//        {
//            QModelIndex varIndex = DataModels::instance()->varModel->indexByName(e->var);
//            QModelIndex varDomainIndex = DataModels::instance()->varModel->index(2,0,varIndex);
//            QString varDomain = DataModels::instance()->varModel->data(varDomainIndex,Qt::DisplayRole).toString();//получили домен переменной
//            if(domainName == varDomain)
//            {//наш клиент
//                if(value == e->val)
//                {//удаляем выражение
//                    QModelIndex ruleIndex = indexByName(r->name);
//                    QModelIndex ruleIfsIndex = index(1,0,ruleIndex);
//                    QModelIndex ruleIfIndex = index(r->predicate.indexOf(e),0,ruleIfsIndex);
//                    deleteIf(ruleIfIndex);
//                }
//            }
//        }
//        foreach(e,r->conclusion)
//        {
//            QModelIndex varIndex = DataModels::instance()->varModel->indexByName(e->var);
//            QModelIndex varDomainIndex = DataModels::instance()->varModel->index(2,0,varIndex);
//            QString varDomain = DataModels::instance()->varModel->data(varDomainIndex,Qt::DisplayRole).toString();//получили домен переменной
//            if(domainName == varDomain)
//            {//наш клиент
//                if(value == e->val)
//                {//удаляем выражение
//                    QModelIndex ruleIndex = indexByName(r->name);
//                    QModelIndex ruleThensIndex = index(2,0,ruleIndex);
//                    QModelIndex ruleThenIndex = index(r->conclusion.indexOf(e),0,ruleThensIndex);
//                    deleteThen(ruleThenIndex);
//                }
//            }
//        }
//    }
}

void
RuleModel::onVarDeleted(QString varName)
{
//удаляется if/then
    qDebug()<<"RuleModel::onVarDeleted "<<varName;
    Rule *r;

    //удаляем правило
    bool ruleNeedToBeDeleted = false;

    for(int i=0;i<rules->count();++i)
    //foreach(r,*rules)
    {
        ruleNeedToBeDeleted = false;
        r=rules->at(i);
        Expr *e;
        foreach(e,r->predicate)
        {
            if(e->var == varName)
            {
                ruleNeedToBeDeleted = true;
//                QModelIndex ruleIndex = indexByName(r->name);
//                QModelIndex ruleIfsIndex = index(1,0,ruleIndex);
//                QModelIndex ruleIfIndex = index(r->predicate.indexOf(e),0,ruleIfsIndex);
//                deleteIf(ruleIfIndex);
            }
        }
        foreach(e,r->conclusion)
        {
            if(e->var == varName)
            {
                ruleNeedToBeDeleted = true;
//                QModelIndex ruleIndex = indexByName(r->name);
//                QModelIndex ruleThensIndex = index(2,0,ruleIndex);
//                QModelIndex ruleThenIndex = index(r->conclusion.indexOf(e),0,ruleThensIndex);
//                deleteThen(ruleThenIndex);
            }
        }
        if(ruleNeedToBeDeleted)
        {
            QModelIndex ruleIndex = index(i,0,QModelIndex());
            deleteRule(ruleIndex);
            i--;
        }
    }
}

void
RuleModel::onVarDomainChanged(QString varName, QString oldDomain, QString newDomain)
{
//тоже удаляем, если изменилось
    qDebug()<<"RuleModel::onVarDomainChanged "<<varName <<" "<<oldDomain<<" "<<newDomain;

    if(oldDomain == newDomain)
        return;
    Rule *r;
    foreach(r,*rules)
    {
        Expr *e;
        foreach(e,r->predicate)
        {
            if(e->var == varName)
            {
                QModelIndex ruleIndex = indexByName(r->name);
                QModelIndex ruleIfsIndex = index(1,0,ruleIndex);
                QModelIndex ruleIfIndex = index(r->predicate.indexOf(e),0,ruleIfsIndex);
                deleteIf(ruleIfIndex);
            }
        }
        foreach(e,r->conclusion)
        {
            if(e->var == varName)
            {
                QModelIndex ruleIndex = indexByName(r->name);
                QModelIndex ruleThensIndex = index(2,0,ruleIndex);
                QModelIndex ruleThenIndex = index(r->conclusion.indexOf(e),0,ruleThensIndex);
                deleteThen(ruleThenIndex);
            }
        }
    }
}
void
RuleModel::onVarNameChanged(QString oldName, QString newName)//просто меняем имя
{
    qDebug()<<"RuleModel::onVarNameChanged "<<oldName <<" "<<newName;
    Rule *r;
    foreach(r,*rules)
    {
        Expr *e;
        foreach(e,r->predicate)
        {
            if(e->var == oldName)
                e->var = newName;
        }
        foreach(e,r->conclusion)
        {
            if(e->var == oldName)
                e->var = newName;
        }
    }
}

QList<Rule*>*
RuleModel::getRules()//Паблик Морозов
{
    return rules;
}

bool
RuleModel::isValid()
{
    bool valid=true;
    QStringList ruleNamesList;

    Rule* r;
    foreach(r,*rules)
    {
        ruleNamesList<<r->name;
    }
    if(!Validator::NoCopies(ruleNamesList))
    {
        valid = false;
        emit sigErrorWhileValidating(QString("Повторяющиеся имена правил"));
    }

    //в правиле должна быть хотя бы 1 посылка и 1 заключение
    if(valid)
    {
        foreach(r,*rules)
        {
            if(r->predicate.count()==0)
            {
                valid = false;
                emit sigErrorWhileValidating(QString("В правиле %1 нет посылки.").arg(r->name));
                break;
            }
            if(r->conclusion.count()==0)
            {
                valid = false;
                emit sigErrorWhileValidating(QString("В правиле %1 нет заключения.").arg(r->name));
                break;
            }
        }
    }

    return valid;
}


//Drag'&'Drop

void RuleModel::moveBefore(QModelIndex sourceInx,QModelIndex destInx)
{
    if(!sourceInx.isValid())
        return;
    if(!destInx.isValid())
        return;
    QModelIndex parentInx = parent(sourceInx);
    RuleNode* parentNode = nodeFromIndex(parentInx);
    if(!parentNode)
        return;
    qDebug()<<"sourceInx:"<<sourceInx.row()<<" destInx:"<<destInx.row();
    qDebug()<<"DomainModel::: "<<data(sourceInx,Qt::DisplayRole).toString()<<" "<<data(destInx,Qt::DisplayRole).toString();
    switch(parentNode->type)
    {
    case RuleNode::Root://перетаскиваем правила
        if(sourceInx.row()>destInx.row() )
        {
            parentNode->children.move(sourceInx.row(),destInx.row());
            rules->move(sourceInx.row(),destInx.row());
        }
        else
        {
            parentNode->children.move(sourceInx.row(),destInx.row()-1);
            rules->move(sourceInx.row(),destInx.row()-1);
        }
        break;
    case RuleNode::RulePredicateHead:
        if(sourceInx.row()>destInx.row() )
        {
            parentNode->children.move(sourceInx.row(),destInx.row());
            Expr* tmp = parentNode->rule->predicate.at(sourceInx.row());
            parentNode->rule->predicate.removeAt(sourceInx.row());
            parentNode->rule->predicate.insert(destInx.row(),tmp);
        }
        else
        {
            parentNode->children.move(sourceInx.row(),destInx.row()-1);
            Expr* tmp = parentNode->rule->predicate.at(sourceInx.row());
            parentNode->rule->predicate.removeAt(sourceInx.row());
            parentNode->rule->predicate.insert(destInx.row()-1,tmp);
        }
        break;
    case RuleNode::RuleConclusionHead:
        if(sourceInx.row()>destInx.row() )
        {
            parentNode->children.move(sourceInx.row(),destInx.row());
            Expr* tmp = parentNode->rule->conclusion.at(sourceInx.row());
            parentNode->rule->conclusion.removeAt(sourceInx.row());
            parentNode->rule->conclusion.insert(destInx.row(),tmp);
        }
        else
        {
            parentNode->children.move(sourceInx.row(),destInx.row()-1);
            Expr* tmp = parentNode->rule->conclusion.at(sourceInx.row());
            parentNode->rule->conclusion.removeAt(sourceInx.row());
            parentNode->rule->conclusion.insert(destInx.row()-1,tmp);
        }
        break;
    }

    emit sigDataChanged();
}


QStringList RuleModel::mimeTypes() const
 {
     QStringList types;
     types << "application/qmodelindex";
     return types;
 }
QMimeData *RuleModel::mimeData(const QModelIndexList &indexes) const
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
bool RuleModel::dropMimeData(const QMimeData *data,
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
RuleModel::generateNewUniqueRuleName()
{
    bool exists=true;
    QString base = "Rule";
    int i=1;

    while(exists)
    {
        QString res = base+QString::number(i);

        if(!ruleExists(res.trimmed()))
        {
            return res;
        }
        ++i;
    }
    return "";
}
