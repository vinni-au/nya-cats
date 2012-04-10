#include "nframe.h"
#include <QDebug>

NFrame::NFrame(QObject *parent) :
    QObject(parent)
{
    name.setName("name");
    is_a.setName("is_a");

    m_slots.append(&name);
    m_slots.append(&is_a);
}


NSlot* NFrame::getSlotByName(QString name)
{
    NSlot* slot=NULL;
    foreach(slot,m_slots)
    {
        if(slot->name()==name)
        {
            return slot;
        }
    }
    return slot;
}


QString NFrame::parentFrame()
{
    return this->is_a.value().toString();
}

int NFrame::id()
{
    return m_id;
}

//сериализация
QDomElement NFrame::toXml(QDomDocument& doc)
{
    QDomElement frameEl = doc.createElement("frame");
    frameEl.setAttribute("id",m_id);
    frameEl.setAttribute("type",m_type);

    QDomElement slotsEl = doc.createElement("slots");
    frameEl.appendChild(slotsEl);

    NSlot* slot;
    foreach(slot,m_slots)
    {
        QDomElement slotEl = slot->toXml(doc);
        slotsEl.appendChild(slotEl);
    }
    return frameEl;
}

void NFrame::fromXml(QDomElement &frame)
{
    m_id = frame.attribute("id").toInt();
    m_type = (FrameType::FRAME_TYPE)frame.attribute("type").toInt();

    QDomElement slotsEl = frame.firstChild().toElement();
    if(!slotsEl.hasChildNodes())
    {
        qDebug()<<"NFrame::fromXml "<<"Нет слотов";
        return;
    }
    QDomElement slotEl = slotsEl.firstChild().toElement();
    NSlot* slot;
    while(!slotEl.isNull())
    {
        slot = getSlotByName(slotEl.attribute("name"));
        if(slot)//такой слот есть (системный)
        {
            if(slot->isSystem())
            {
                slot->fromXml(slotEl);
            }
            else
            {
                qDebug()<<"NFrame::fromXml "<<"слот "<<slot->name()<<" не помечен как системный";
            }
        }
        else
        {//добавить слот
            NSlot *newSlot = new NSlot();
            newSlot->fromXml(slotEl);
            this->m_slots.append(newSlot);
        }
        slotEl.nextSibling().toElement();
    }
}


int NFrame::slotCount()
{
    return m_slots.count();
}
