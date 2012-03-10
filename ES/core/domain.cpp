#include "domain.h"

Domain::Domain(QString name, QObject *parent) :
    QObject(parent)
{
    this->name=name;
}
