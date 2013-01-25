#include "rulenode.h"


RuleNode::RuleNode(Type type,Rule *rule,bool createBackLink,RuleNode *parent) :
    QObject(parent)
{
    this->type=type;
    this->rule=rule;
    this->parent=parent;

    if(parent && createBackLink)
    {
        parent->children.append(this);
        backLinkCreated = true;
    }
    else
        backLinkCreated = false;
}
RuleNode::~RuleNode()
{
    RuleNode *parent = this->parent;
    if(parent && backLinkCreated)
    {
        //int i = parent->children.indexOf(this);
        parent->children.removeOne(this);
    }

    qDeleteAll(this->children);
}
