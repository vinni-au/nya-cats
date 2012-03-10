#ifndef STARTCONSWND_H
#define STARTCONSWND_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QMap>

#include "core/datamodels.h"
#include "questionwnd.h"

class StartConsWnd : public QDialog
{
    Q_OBJECT
public:
    enum RuleResult{Accepted,Rejected,None};//сработало, откатилось, не использоваось
    explicit StartConsWnd(QWidget *parent = 0);

signals:
    void sigRuleUse(QString ruleName,int result);
    void sigVarUse(QString varName,QString varValue);

public slots:
    void onStartConsClick();

private:
    QString ProveAim(QString aimVar);  //рекурсивная процедура. true - если удалось доказать. иначе - false
    QList<Rule*>* buildFireRules(QString aimVar);//строит список горящих правил
    bool ruleAccepted(Rule *rule);//проверяет сработало ли правило, запускает доказательство подцелей из посылки, означивает переменные

    void AddVarUsed(QString varName,QString varValue);
    void AddRuleUsed(QString ruleName,RuleResult result);
private:
    bool continueWork;
    QVBoxLayout *layMain;
        QHBoxLayout     *layAim;
            QLabel      *lbAim;
            QComboBox   *cmbAim;
        QHBoxLayout     *layButtons;
            QPushButton *btnStart;
            QPushButton *btnCancel;

    QMap<QString,QString>       ConsVars;     //означенные переменные
    QMap<QString,RuleResult>    ConsRules;    //правила, которые рассматривались

    QVector<QString>            ConsVarsVec;
    QVector<QString>            ConsRulesVec;

public:
    QString consResult;
};

#endif // STARTCONSWND_H
