#ifndef DOMAINNODE_H
#define DOMAINNODE_H

#include <QObject>
#include "domain.h"

/**
  @class DomainNode
  Класс вершины в иерархической модели доменов
*/
class DomainNode : public QObject
{
    Q_OBJECT
public:
    ///Тип вершины в модели
    enum Type {Root,DomainName,DomainValue};

    /// Конструктор
    /// @param type - тип вершины
    /// @param createBackLink - создать обратную ссылку у родителя
    /// @param domain - ссылка на домен
    explicit DomainNode(Type type,Domain *domain,bool createBackLink,DomainNode *parent = 0);
    ~DomainNode();

signals:

public slots:
public:
    /// Тип вершины
    Type type;
    /// Ссылка на домен
    Domain *domain;

    /// Ссылка на родителя
    DomainNode *parent;
    /// Ссылки на детей
    QList<DomainNode*> children;
private:
    /// Была ли создана обратныя ссылка
    bool backLinkCreated;
};

#endif // DOMAINNODE_H
