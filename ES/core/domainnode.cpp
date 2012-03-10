#include "domainnode.h"

DomainNode::DomainNode(Type type,Domain *domain,bool createBackLink,DomainNode *parent) :
    QObject(NULL)
{
    this->type=type;
    this->domain=domain;
    this->parent=parent;

    if(parent && createBackLink)
    {
        parent->children.append(this);
        backLinkCreated = true;
    }
    else
        backLinkCreated = false;
}
DomainNode::~DomainNode()
{
    DomainNode *parent = this->parent;
    if(parent && backLinkCreated)
    {
        //int i = parent->children.indexOf(this);
        parent->children.removeOne(this);
    }

    qDeleteAll(this->children);
}
