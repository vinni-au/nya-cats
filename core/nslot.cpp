#include "nslot.h"
#include <QDebug>

NSlot::NSlot(QObject *parent) :
    QObject(parent)
{
    m_system = false;

    //инициализация слотов
    slotName.setName("name");
    slotName.setStringValue("defName");
    m_fasets.append(&slotName);

    slotType.setName("slot_type");
    slotType.setStringValue("domain");
    m_fasets.append(&slotType);

    slotValue.setName("value");
    slotValue.setStringValue("defValue");
    m_fasets.append(&slotValue);

    slotDefaultValue.setName("default_value");
    slotDefaultValue.setStringValue("");
    m_fasets.append(&slotDefaultValue);

    slotMarkerType.setName("marker_type");
    slotMarkerType.setStringValue("domain");
    m_fasets.append(&slotMarkerType);

    slotMarker.setName("marker");
    slotMarker.setStringValue("defDomain");
    m_fasets.append(&slotMarker);

    slotInheritanceType.setName("inheritance_type");
    slotInheritanceType.setStringValue(INHERIT_SAME);
    m_fasets.append(&slotInheritanceType);


}


//системность
bool NSlot::isSystem()
{
    return m_system;
}

void NSlot::setSystem(bool isSystem)
{
    this->m_system = isSystem;
}

NFaset* NSlot::getFasetByName(QString name)
{
    NFaset* faset=NULL;
    foreach(faset,this->m_fasets)
    {
        if(faset->name()==name)
        {
            return faset;
        }
    }
    return faset;
}
QString NSlot::name()//имя слота
{
    return this->getFasetByName("name")->getStringValue();
}

void NSlot::setName(QString name)
{
    this->getFasetByName("name")->setStringValue(name);
}
QVariant NSlot::value()//значение слота
{
    NFaset* valFaset = getFasetByName("value");
    switch(valFaset->type())
    {
    case FasetType::type_int:
        return QVariant(valFaset->getIntValue());
        break;

    case FasetType::type_string:
        return QVariant(valFaset->getStringValue());
        break;

    };
    return QVariant();
}
void NSlot::setValue(QVariant value)
{
    this->getFasetByName("value")->setStringValue(value.toString());
    this->getFasetByName("value")->setIntValue(value.toInt());//сомнительно!
}

QDomElement NSlot::toXml(QDomDocument& doc)
{
    QDomElement slotEl = doc.createElement("slot");
    slotEl.setAttribute("system",m_system);

    QDomElement fasetsEl = doc.createElement("fasets");
    slotEl.appendChild(fasetsEl);

    NFaset* faset;
    foreach(faset,m_fasets)
    {
        QDomElement fasetEl = faset->toXml(doc);
        fasetsEl.appendChild(fasetEl);
    }
    return slotEl;
}

void NSlot::fromXml(QDomElement &slot)
{
    m_system=slot.attribute("system").toInt();
    QDomElement fasetsEl = slot.firstChild().toElement();
    if(!fasetsEl.hasChildNodes())
    {
        qDebug()<<"NSlot::fromXml "<<"Нет фасетов";
        return;
    }
    QDomElement fasetEl = fasetsEl.firstChild().toElement();
    NFaset* faset;
    while(!fasetEl.isNull())
    {
        faset = getFasetByName(fasetEl.attribute("name"));
        if(!faset)
        {
            qDebug()<<"NSlot::fromXml "<<"Неизвестное имя фасета: "<<fasetEl.attribute("name");
            continue;
        }
        faset->fromXml(fasetEl);
    }
}
