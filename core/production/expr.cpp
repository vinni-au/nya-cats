#include "expr.h"

Expr::Expr(QObject *parent) :
    QObject(parent)
{
    this->var="";
    this->sign="=";
    this->val="";
}
QString
Expr::toString()
{
    return var+" "+sign+" "+"'"+val+"'";
}
