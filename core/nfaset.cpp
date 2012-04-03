#include "nfaset.h"
#include <QDebug>

NFaset::NFaset(QObject *parent/*=0*/) :
    QObject(parent)
{
    this->m_type = NFaset::type_string;
    this->m_intValue = 0;
}

NFaset::NFaset(QString name,QObject *parent/*=0*/)
{
    m_name=name;
    this->m_type = NFaset::type_string;
    this->m_intValue = 0;
}

NFaset::FASET_TYPE NFaset::type()
{
    return m_type;
}

QString NFaset::name()
{
    return m_name;
}
void NFaset::setName(QString name)
{
    m_name=name;
}


QString NFaset::getStringValue()
{
    return m_stringValue;
}

int NFaset::getIntValue()
{
    return m_intValue;
}

void NFaset::setStringValue(QString value)
{
    this->m_stringValue=value;
    this->m_type = NFaset::type_string;
}

void NFaset::setIntValue(int value)
{
    this->m_intValue = value;
    this->m_type = NFaset::type_int;
}


//сериализация
QDomElement NFaset::toXml(QDomDocument& doc)
{
    QDomElement faset = doc.createElement("faset"); //doc.appendChild(domains);
    faset.setAttribute("name",this->name());
    faset.setAttribute("type",this->type());

    switch(type())
    {
    case type_int:
        faset.setAttribute("value",this->getIntValue());
        break;
    case type_string:
        faset.setAttribute("value",this->getStringValue());
        break;
    default:
        qDebug()<<"NFaset::toXml "<<"Неизвестный тип фасета при сериализации";
        break;
    }

    return faset;
}

void NFaset::fromXml(QDomElement &faset)
{

    this->setName(faset.attribute("name"));
    this->m_type =(FASET_TYPE) faset.attribute("type").toInt();
    switch(type())
    {
    case type_int:
        this->setIntValue( faset.attribute("value").toInt());
        break;
    case type_string:
        this->setStringValue( faset.attribute("value"));
        break;
    default:
        qDebug()<<"NFaset::fromXml "<<"Неизвестный тип фасета при десериализации";
        break;
    }
}
