#include "nframenode.h"


NFrameNode::NFrameNode(Type type,NFrame *frame,bool createBackLink,NFrameNode *parent) :
    QObject(parent)
{
    this->type=type;
    this->frame=frame;
    this->parent=parent;

    if(parent && createBackLink)
    {
        parent->children.append(this);
        backLinkCreated = true;
    }
    else
        backLinkCreated = false;
}
NFrameNode::~NFrameNode()
{
    NFrameNode *parent = this->parent;
    if(parent && backLinkCreated)
    {
        //int i = parent->children.indexOf(this);
        parent->children.removeOne(this);
    }

    qDeleteAll(this->children);
}
