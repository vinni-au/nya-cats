#include "rule.h"

Rule::Rule(QObject *parent) :
    QObject(parent)
{
    this->name="";
    this->reason="";

}

Rule::~Rule()
{
    qDeleteAll(predicate);
    qDeleteAll(conclusion);
}

