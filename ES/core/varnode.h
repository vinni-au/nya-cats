#ifndef VARNODE_H
#define VARNODE_H

#include <QObject>
#include "variable.h"

class VarNode : public QObject
{
    Q_OBJECT

public:
    enum Type {Root,VarName,VarDerivable,VarAsking,VarDomain,VarQuestion};
    explicit VarNode(Type type,Variable *var,bool createBackLink,VarNode *parent = 0);
    ~VarNode();

signals:
public slots:
public:
    Type type;
    Variable *var;

    VarNode *parent;
    QList<VarNode*> children;
private:
    bool backLinkCreated;
};

#endif // VARNODE_H
