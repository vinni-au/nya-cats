#include "validator.h"

Validator::Validator(QObject *parent) :
    QObject(parent)
{
}


bool
Validator::NoCopies(QStringList& strList)
{
    QStringList temp;

    for(int i=0;i<strList.count();++i)
    {
        if(!temp.contains(strList[i]))
        {
            temp << strList[i];
        }
        else
        {
            return false;
        }
    }
    return true;
}


bool
Validator::NoCopies(QList<QString> strList)
{
    QStringList temp;

    for(int i=0;i<strList.count();++i)
    {
        if(!temp.contains(strList[i]))
        {
            temp << strList[i];
        }
        else
        {
            return false;
        }
    }
    return true;
}
