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
    if(arSign=="+")
        return var+" "+sign+" "+"'"+val+arSign+QString::number(summer)+"'";
    else
        return var+" "+sign+" "+"'"+val+"'";
}
