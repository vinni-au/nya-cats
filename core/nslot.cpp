#include "nslot.h"
#include <QDebug>

NSlot::NSlot(QObject *parent) :
    QObject(parent)
{
    m_system = false;

    //инициализация слотов
    slotName.setName("name");
    slotName.setValue("defName");
    m_fasets.append(&slotName);

    slotType.setName("slot_type");
    slotType.setValue("domain");
    m_fasets.append(&slotType);

    slotValue.setName("value");
    slotValue.setValue("defValue");
    m_fasets.append(&slotValue);

    slotDefaultValue.setName("default_value");
    slotDefaultValue.setValue("");
    m_fasets.append(&slotDefaultValue);

    slotMarkerType.setName("marker_type");
    slotMarkerType.setValue("domain");
    m_fasets.append(&slotMarkerType);

    slotMarker.setName("marker");
    slotMarker.setValue("defDomain");
    m_fasets.append(&slotMarker);

    slotInheritanceType.setName("inheritance_type");
    slotInheritanceType.setValue(INHERIT_SAME);
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

//получение фасета по имени
//для оптимизации в дальнейшем можно заюзать мапу фасетов по имени
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

NFaset* NSlot::getFasetByIndex(int inx)
{
    if(inx+1>m_fasets.count() || inx<0)
        return NULL;

    return m_fasets.at(inx);
}

//имя слота
QString NSlot::name()
{
    return this->getFasetByName("name")->getStringValue();
}
void NSlot::setName(QString name)
{
    this->getFasetByName("name")->setStringValue(name);
}
//значение слота
QVariant NSlot::value()
{
    NFaset* valFaset = getFasetByName("value");
    return valFaset->value();
}
void NSlot::setValue(QVariant value)
{
    this->getFasetByName("value")->setValue(value);
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

        fasetEl=fasetEl.nextSibling().toElement();
    }
}

int NSlot::fasetCount()
{
    return m_fasets.count();
}
