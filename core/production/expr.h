#ifndef EXPR_H
#define EXPR_H

#include <QObject>

/**
  @class Expr
  Класс выражения типа
переменная = значение
переменная != значение
  */

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

    QString arSign;
    int summer;
};

#endif // EXPR_H
