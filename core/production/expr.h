#ifndef EXPR_H
#define EXPR_H

#include <QObject>

class Expr : public QObject
{
    Q_OBJECT
public:

    explicit Expr(QObject *parent = 0);
    QString toString();
signals:

public slots:

public:
    QString var;
    QString sign;
    QString val;
};

#endif // EXPR_H
