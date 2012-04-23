#include "nframe.h"
#include <QDebug>

NFrame::NFrame(unsigned id,QObject *parent) :
    QObject(parent)
{
    name.setName("name");
    is_a.setName("is_a");

    name.setSystem(true);
    is_a.setSystem(true);

    m_slots.append(&name);
    m_slots.append(&is_a);

    this->m_id = id;
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
    return NULL;
}


QString NFrame::parentFrame()
{
    return this->is_a.defValue().toString();
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
    int i=0;
    while(!slotEl.isNull())
    {
        //QString attr = slotEl.attribute("name");
        slot = getSlotByIndex(i);
        i++;
       // slot = getSlotByName(slotEl.attribute("name"));
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
        slotEl=slotEl.nextSibling().toElement();
    }
}


int NFrame::slotCount()
{
    return m_slots.count();
}

NSlot* NFrame::getSlotByIndex(int inx)
{
    if(inx+1>m_slots.count() || inx<0)
        return NULL;

    return m_slots.at(inx);
}
void NFrame::addSlot(NSlot *slot)
{
    this->m_slots.append(slot);
}

void NFrame::insertSlot(int row,NSlot *slot)
{
   // if(row>-1 && row <m_slots.count())
        m_slots.insert(row,slot);
   //// else
   //     qDebug()<<"Не могу вставить слота во фрейм";
}

void NFrame::removeSlot(int row)
{

    if(row>-1 && row <m_slots.count()-1)
        m_slots.removeAt(row);
    else
        qDebug()<<"Не могу удалить слота во фрейм";

}
void NFrame::removeSlot(NSlot *slot)
{
    m_slots.removeOne(slot);
}

int NFrame::getSlotIndexByName(QString name)
{
    for(int i=0;i<m_slots.count();++i)
    {
        if(m_slots.at(i)->name()==name)
            return i;
    }
    return -1;
}


QString NFrame::frameName()
{
    return name.defValue().toString();
}

void NFrame::setParentName(QString name)
{
    getSlotByName("is_a")->setDefValue(name);
}
