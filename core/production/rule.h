#ifndef RULE_H
#define RULE_H

#include <QObject>
#include <QString>
#include <QList>
#include "expr.h"


/**
  @class Rule
  Класс для представления продукционного правила
  */
class Rule : public QObject
{
    Q_OBJECT
public:
    explicit Rule(QObject *parent = 0);
    ~Rule();

signals:

public slots:

public:
    QString name;
    QString reason;
    QList<Expr*> predicate;
    QList<Expr*> conclusion;
};

#endif // RULE_H
