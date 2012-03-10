#include "startconswnd.h"
#include <QDebug>

StartConsWnd::StartConsWnd(QWidget *parent) :
    QDialog(parent)
{
    continueWork = true;
    /*QVBoxLayout **/layMain = new QVBoxLayout();           this->setLayout(layMain);
    {
        /*QHBoxLayout     **/layAim = new QHBoxLayout();    layMain->addLayout(layAim);
        {
            /*QLabel      **/lbAim = new QLabel("Цель:");   layAim->addWidget(lbAim);
            /*QComboBox   **/cmbAim = new QComboBox();      layAim->addWidget(cmbAim);
        }
        layMain->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding));
        /*QHBoxLayout     **/layButtons = new QHBoxLayout();    layMain->addLayout(layButtons);
        {
            /*QPushButton **/btnStart = new QPushButton("Начать консультацию"); layButtons->addWidget(btnStart);
            /*QPushButton **/btnCancel = new QPushButton("Отмена"); layButtons->addWidget(btnCancel);
        }
    }

    cmbAim->setModel(DataModels::instance()->varModel);

    connect(btnStart,SIGNAL(clicked()),SLOT(onStartConsClick()));
    connect(btnCancel,SIGNAL(clicked()),SLOT(reject()));
}



void
StartConsWnd::onStartConsClick()
{
    qDebug()<<"StartConsWnd::onStartConsClick";
    QString aim = cmbAim->currentText();
    if(aim.isEmpty())
        return;
    consResult = ProveAim(aim);
    accept();
}



QString
StartConsWnd::ProveAim(QString aimVar)
{
    qDebug()<<"StartConsWnd::ProveAim: "<<aimVar;
    if(!continueWork)
    {
        qDebug()<<"Cancelled by User";
        return "";
    }
    QModelIndex varIndex = DataModels::instance()->varModel->indexByName(aimVar);
    QModelIndex varAskingIndex = DataModels::instance()->varModel->index(0,0,varIndex);
    QModelIndex varDerivableIndex = DataModels::instance()->varModel->index(1,0,varIndex);

    bool asking = DataModels::instance()->varModel->data(varAskingIndex,Qt::DisplayRole).toBool();
    bool derivable = DataModels::instance()->varModel->data(varDerivableIndex,Qt::DisplayRole).toBool();

    qDebug()<<"Aim Asking: "<<asking;
    //а может быть она уже известна?

    if(asking && (!derivable))
    {//чисто запрашиваемая
        QuestionWnd qw;
        qw.SetQuestion(varIndex );
        if(qw.exec())
        {//сохранить ответ
            QString ans = qw.GetAnswer();

            AddVarUsed(aimVar,ans);
            //ConsVars.insert(aimVar,ans);    //добавляем в означенные переменные
            qDebug()<<"Aim Answer: "<<ans;
            return ans;
        }
        else//cancel
        {
            continueWork = false;//на выход
            return "";
        }
    }
    else
    {//не запрашиваемая => выводимая или выводимо-запрашиваемая

        //Пока цель не вывелась
        while(true)
        {
            //строим список FireRules
            //туда попадают только те, правила, которые еще не рассматривались, и в заключении которые есть цель
            QList<Rule*> *fireRules = buildFireRules(aimVar);
            //в соответствии со стратегией выборки правил выбираем первое правило, если оно есть
            if(fireRules->count()==0)
            {
                //если правил больше нет
                //спросить если выводимо-запрашиваемая
                if(asking && derivable)///////////////////////////////////////beg
                {
                    QuestionWnd qw;
                    qw.SetQuestion(varIndex );
                    if(qw.exec())
                    {//сохранить ответ
                        QString ans = qw.GetAnswer();

                        AddVarUsed(aimVar,ans);
                        //ConsVars.insert(aimVar,ans);    //добавляем в означенные переменные
                        qDebug()<<"Aim Answer: "<<ans;
                        return ans;
                    }
                    else//cancel
                    {
                        continueWork = false;//на выход
                        return "";
                    }
                }
                else///////////////////end
                    return false;//не удалось вывести
            }

            Rule* nextRule = fireRules->at(0);
            qDebug()<<"Выбрано правило: "<<DataModels::instance()->ruleModel->data(DataModels::instance()->ruleModel->indexByName(nextRule->name)  ,Qt::DisplayRole ).toString();

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


}

QList<Rule*> *
StartConsWnd::buildFireRules(QString aimVar)
{
    qDebug()<<">>StartConsWnd::buildFireRules: "<<aimVar;


    QList<Rule*>* allRules = DataModels::instance()->ruleModel->getRules();

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
        qDebug()<<"===" <<DataModels::instance()->ruleModel->data(DataModels::instance()->ruleModel->indexByName(r->name)  ,Qt::DisplayRole ).toString();
    }

    return fireRules;
}
bool
StartConsWnd::ruleAccepted(Rule *rule)
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
            if(e->sign == "=")
            {
                if(! (e->val == ConsVars[e->var]))//не то значение
                {
                    AddRuleUsed(rule->name,StartConsWnd::Rejected);
                    //ConsRules.insert(rule->name,StartConsWnd::Rejected);//добавляем в список неудачных правил
                    qDebug()<<"Expr not accepted:"<<e->toString();
                    return false;
                }
            }
            else
            {// !=
                if(! (e->val != ConsVars[e->var]))
                {
                    AddRuleUsed(rule->name,StartConsWnd::Rejected);
                   // ConsRules.insert(rule->name,StartConsWnd::Rejected);
                    qDebug()<<"Expr not accepted:"<<e->toString();
                    return false;
                }
            }
            qDebug()<<"Expr accepted:"<<e->toString();
        }
        else
        {//это неизвестная переменная
            qDebug()<<"It is unknown var:"<<e->var;
            QString val = ProveAim(e->var);
            qDebug()<<"Value was got:"<<e->var<<" = "<<val;
            if(val.isEmpty())
            {
                AddRuleUsed(rule->name,StartConsWnd::Rejected);
                //ConsRules.insert(rule->name,StartConsWnd::Rejected);
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
        AddVarUsed(e->var,e->val);
        //ConsVars.insert(e->var,e->val);
        qDebug()<<"Then: set: "<<e->toString();
    }

    AddRuleUsed(rule->name,StartConsWnd::Accepted);
    //ConsRules.insert(rule->name,StartConsWnd::Accepted);
    return true;
}

void
StartConsWnd::AddVarUsed(QString varName, QString varValue)
{
    if(!ConsVars.contains(varName))
    {
        ConsVars.insert(varName,varValue);
        ConsVarsVec.append(varName);
        emit sigVarUse(varName,varValue);
    }
}

void
StartConsWnd::AddRuleUsed(QString ruleName, RuleResult result)
{
    if(!ConsRules.contains(ruleName))
    {
        ConsRules.insert(ruleName,result);
        ConsRulesVec.append(ruleName);
        emit sigRuleUse(ruleName,result);
    }
}
