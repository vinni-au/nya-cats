#include "varnode.h"


VarNode::VarNode(Type type,Variable *var,bool createBackLink,VarNode *parent) :
    QObject(NULL)
{
    this->type=type;
    this->var=var;
    this->parent=parent;

    if(parent && createBackLink)
    {
        parent->children.append(this);
        backLinkCreated = true;
    }
    else
        backLinkCreated = false;
}
VarNode::~VarNode()
{
    VarNode *parent = this->parent;
    if(parent && backLinkCreated)
    {
        //int i = parent->children.indexOf(this);
        parent->children.removeOne(this);
    }

    qDeleteAll(this->children);
}
