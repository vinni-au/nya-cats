#ifndef NPRODUCTIONMLV_H
#define NPRODUCTIONMLV_H

#include <QObject>
#include "rulemodel.h"
#include <QDebug>
#include "core/nkbmanager.h"
#include "nproduction.h"
#include "mlv/mlv.h"

//sigVarUse - сигнал о том, какая переменная(слот) получила значение

class NProductionMLV : public QObject
{
    Q_OBJECT
public:
    explicit NProductionMLV(MLV* mlv,int frameId,NProduction *production, QObject *parent = 0);
    enum RuleResult{Accepted,Rejected,None};//сработало, откатилось, не использоваось

    QString StartConsultation(QString aim);
signals:
    void sigRuleUse(QString ruleName,int result);
    void sigVarUse(QString varName,QString varValue);
public slots:

private:
    QString ProveAim(QString aimVar);  //рекурсивная процедура. true - если удалось доказать. иначе - false
    QList<Rule*>* buildFireRules(QString aimVar);//строит список горящих правил
    bool ruleAccepted(Rule *rule);//проверяет сработало ли правило, запускает доказательство подцелей из посылки, означивает переменные

    void AddVarUsed(QString varName,QString varValue);
    void AddRuleUsed(QString ruleName,RuleResult result);

    QString calcVal(QString value);
private:
    NProduction *m_production;
    MLV* m_mlv;
    int m_frameId;              //id фрйма-экземпляра, в котором находится слот с продукционным маркером.

    bool continueWork;
    QMap<QString,QString>       ConsVars;     //означенные переменные
    QMap<QString,RuleResult>    ConsRules;    //правила, которые рассматривались
    QVector<QString>            ConsVarsVec;
    QVector<QString>            ConsRulesVec;
public:
    QString consResult;
};

#endif // NPRODUCTIONMLV_H
