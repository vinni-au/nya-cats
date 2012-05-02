#include "nproductionmlv.h"

NProductionMLV::NProductionMLV(MLV* mlv,int frameId,NProduction *production,QObject *parent) :
    QObject(parent)
{
    continueWork = true;
    m_production = production;
    m_mlv = mlv;
    m_frameId = frameId;
}

QString
NProductionMLV::StartConsultation(QString aim)
{
    qDebug()<<"NProductionMLV::StartConsultation";
    if(aim.isEmpty())
        return "";
    consResult = ProveAim(aim);
    return consResult;
}

QString
NProductionMLV::ProveAim(QString aimVar)
{
    qDebug()<<"NProductionMLV::ProveAim: "<<aimVar;
    if(!continueWork)
    {
        qDebug()<<"Cancelled by User";
        return "";
    }


    //целью или подцелью может быть слот фрейма экземпляра. Получаем значение от МЛВ.
    QString slotVal = m_mlv->getVal(m_frameId,aimVar).toString();
    if(!slotVal.isEmpty())
        return slotVal;

    //Пока цель не вывелась
    while(true)
    {
        //строим список FireRules
        //туда попадают только те, правила, которые еще не рассматривались, и в заключении которые есть цель
        QList<Rule*> *fireRules = buildFireRules(aimVar);
        //в соответствии со стратегией выборки правил выбираем первое правило, если оно есть
        if(fireRules->count()==0)
            return "";//не удалось вывести

        Rule* nextRule = fireRules->at(0);
        qDebug()<<"Выбрано правило: "<<m_production->getModel()->data(m_production->getModel()->indexByName(nextRule->name)  ,Qt::DisplayRole ).toString();

        if(ruleAccepted(nextRule))//если правило сработало
        {//то цель вывелась
            qDebug()<<"Rule accepted: "<<nextRule->name;
            return ConsVars[aimVar];
        }
        else
        {
            qDebug()<<"Rule rejected: "<<nextRule->name;
        }
    }
}

QList<Rule*> *
NProductionMLV::buildFireRules(QString aimVar)
{
    qDebug()<<">>NProductionMLV::buildFireRules: "<<aimVar;
    QList<Rule*>* allRules =m_production->getModel()->getRules();
    QList<Rule*>*   fireRules = new QList<Rule*>();
    if(!continueWork)
        return fireRules;
    Rule* r;
    foreach(r,*allRules)
    {
        bool appropriate = !ConsRules.contains(r->name);
        bool hasAimInThen = false;
        Expr* e;
        foreach(e,r->conclusion)
        {
            if(e->var == aimVar)
            {
                hasAimInThen = true;
                break;
            }
        }
        if(hasAimInThen && appropriate)
        {//добавляем в fireRules
            fireRules->append(r);
        }
    }
    foreach(r,*fireRules)
    {
        qDebug()<<"===" <<m_production->getModel()->data(m_production->getModel()->indexByName(r->name)  ,Qt::DisplayRole ).toString();
    }
    return fireRules;
}

bool
NProductionMLV::ruleAccepted(Rule *rule)
{
    //а у меня ленивые вычисления условий?
    //разбираем посылку

    Expr* e;
    for(int i=0;i<rule->predicate.count();i++)
    {
        if(!continueWork)
            return false;
        e = rule->predicate.at(i);
        qDebug()<<"Current rule predicate: "<<rule->name<<" "<<e->toString();
        if(ConsVars.contains(e->var))//если такая переменная уже означена
        {
            qDebug()<<"Var exists:  "<<e->var<<" value: "<<ConsVars[e->var];
            //////Блок сопоставления
            QString varValue = calcVal(e);//в значении может быть выражение
            if(e->sign == "=")
            {
                if(! (varValue == ConsVars[e->var]))//не то значение
                {
                    AddRuleUsed(rule->name,NProductionMLV::Rejected);
                    qDebug()<<"Expr not accepted:"<<e->toString();
                    return false;
                }
            }
            else
            {// !=
                if(! (varValue != ConsVars[e->var]))
                {
                    AddRuleUsed(rule->name,NProductionMLV::Rejected);
                    qDebug()<<"Expr not accepted:"<<e->toString();
                    return false;
                }
            }
            /////конец Блока сопоставления
            qDebug()<<"Expr accepted:"<<e->toString();
        }
        else
        {//это неизвестная переменная
            qDebug()<<"It is unknown var:"<<e->var;
            QString val = ProveAim(e->var);
            qDebug()<<"Value was got:"<<e->var<<" = "<<val;
            if(val.isEmpty())
            {
                AddRuleUsed(rule->name,NProductionMLV::Rejected);
                return false;//не удалось доказать
            }
            else
            {
                //добавляем переменную в список означенных переменных
                AddVarUsed(e->var,val);
                ConsVars.insert(e->var,val);//лишний раз
                i--;
                continue;
            }
        }
    }

    //если дошли досюда, значит посылка доказана
    //выполняем часть then
    foreach(e,rule->conclusion)
    {
        //означиваем переменные

        QString varValue = calcVal(e);//в значении может быть выражение
        AddVarUsed(e->var,varValue);
        m_mlv->setVal(m_frameId,e->var,varValue);
        qDebug()<<"Then: set: "<<e->toString();
    }

    AddRuleUsed(rule->name,NProductionMLV::Accepted);
    return true;
}

void
NProductionMLV::AddVarUsed(QString varName, QString varValue)
{
    if(!ConsVars.contains(varName))
    {
        ConsVars.insert(varName,varValue);
        ConsVarsVec.append(varName);
        emit sigVarUse(varName,varValue);

    }
}

void
NProductionMLV::AddRuleUsed(QString ruleName, RuleResult result)
{
    if(!ConsRules.contains(ruleName))
    {
        ConsRules.insert(ruleName,result);
        ConsRulesVec.append(ruleName);
        emit sigRuleUse(ruleName,result);
    }
}

QString NProductionMLV::calcVal(Expr* e)
{
    if(e->arSign=="+")
    {
        return e->val+e->summer;
    }
    else
    {
        return e->val;
    }
}
