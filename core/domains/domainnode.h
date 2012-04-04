#ifndef DOMAINNODE_H
#define DOMAINNODE_H

#include <QObject>
#include "domain.h"

//класс для доменов и значений
class DomainNode : public QObject
{
    Q_OBJECT
public:
    enum Type {Root,DomainName,DomainValue};

    explicit DomainNode(Type type,Domain *domain,bool createBackLink,DomainNode *parent = 0);
    ~DomainNode();

signals:

public slots:
public:
    Type type;
    Domain *domain;

    DomainNode *parent;
    QList<DomainNode*> children;
private:
    bool backLinkCreated;
};

#endif // DOMAINNODE_H
