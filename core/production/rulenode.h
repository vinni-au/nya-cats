#ifndef RULENODE_H
#define RULENODE_H

#include <QObject>
#include "rule.h"


/**
  @class RuleNode
  Класс для вершины модели правил
  */
class RuleNode : public QObject
{
    Q_OBJECT

public:
    enum Type {Root,RuleName,RuleReason,RulePredicateHead,RulePredicate,RuleConclusionHead,RuleConclusion};
    explicit RuleNode(Type type,Rule *rule,bool createBackLink,RuleNode *parent = 0);
    ~RuleNode();

signals:
public slots:
public:
    Type type;
    Rule *rule;

    RuleNode *parent;
    QList<RuleNode*> children;
private:
    bool backLinkCreated;

};

#endif // RULENODE_H
