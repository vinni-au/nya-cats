#include "variable.h"

Variable::Variable(QString name, QObject *parent) :
    QObject(parent)
{
    this->name = name;

    this->asking=false;
    this->derivable=false;
    this->domain="";
    this->question="";
}
