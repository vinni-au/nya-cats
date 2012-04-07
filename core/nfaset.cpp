#include "nfaset.h"
#include <QDebug>

NFaset::NFaset(QObject *parent/*=0*/) :
    QObject(parent)
{

}

NFaset::NFaset(QString name,QObject *parent/*=0*/)
{
    m_name=name;
    this->m_value.clear();
}

//имя
QString NFaset::name()
{
    return m_name;
}
void NFaset::setName(QString name)
{
    m_name=name;
    emit nameChanged();
}

//значение
QVariant NFaset::value()
{
    return m_value;
}

void NFaset::setValue(QVariant val)
{
    m_value = val;
    emit valueChanged();
}
//

QString NFaset::getStringValue()
{
    return m_value.toString();
}

int NFaset::getIntValue()
{
    return m_value.toInt();
}

void NFaset::setStringValue(QString value)
{
    this->m_value = value;
    emit valueChanged();
}

void NFaset::setIntValue(int value)
{
    this->m_value = value;
    emit valueChanged();
}



//сериализация
QDomElement NFaset::toXml(QDomDocument& doc)
{
    QDomElement faset = doc.createElement("faset");
    faset.setAttribute("name",this->name());
    faset.setAttribute("value",this->value().toString());
    return faset;
}

void NFaset::fromXml(QDomElement &faset)
{
    this->setName(faset.attribute("name"));
    this->setValue(faset.attribute("value"));
}


