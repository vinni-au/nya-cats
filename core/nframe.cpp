#include "nframe.h"
#include <QDebug>

NFrame::NFrame(unsigned id,FrameType::FRAME_TYPE frameType,QObject *parent) :
    QObject(parent)
{
    name.setHasFasetValue( frameType == FrameType::exemplar);
    is_a.setHasFasetValue( frameType == FrameType::exemplar);

    name.setName("name");
    is_a.setName("is_a");

    name.getFasetByName("slot_type")->setValue("string");
    is_a.getFasetByName("slot_type")->setValue("frame");

    name.setSystem(true);
    is_a.setSystem(true);

    m_slots.append(&name);
    m_slots.append(&is_a);

    this->m_id = id;

    this->m_type = frameType;
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

NFaset* NFrame::GetSlotFaset(QString slotName, QString fasetName)
{
    if(NSlot* slot = getSlotByName(slotName))
    {
        return slot->getFasetByName(fasetName);
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

    if(row>-1 && row <m_slots.count())
        m_slots.removeAt(row);
    else
        qDebug()<<"Не могу удалить слота во фрейм";

}
void NFrame::removeSlot(NSlot *slot)
{
    m_slots.removeOne(slot);
}

void NFrame::removeSlot(QString name)
{
    NSlot* slot = getSlotByName(name);
    if (slot)
        removeSlot(slot);
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

FrameType::FRAME_TYPE NFrame::frameType()
{
    return m_type;
}

void NFrame::setParentName(QString name)
{
    getSlotByName("is_a")->setDefValue(name);
}

QStringList NFrame::getSimpleSlotNames()
{
    QStringList slotNames;
    NSlot* slot;
    foreach(slot,m_slots)
    {
        if(slot->getFasetByName("slot_type")->value()!="frame")
        {
            slotNames<<slot->name();
        }
    }
    return slotNames;
}

QStringList NFrame::getSubframesSlotNames(QStringList& frames)
{
    QStringList slotNames;

    NSlot* slot;
    foreach(slot,m_slots)
    {
        if(slot->getFasetByName("slot_type")->value()=="frame")
        {
            slotNames<<slot->name();
            frames<<slot->defValue().toString();
        }
    }
    return slotNames;
}

NFrame* NFrame::createInstance()
{
    NFrame *instFrame = new NFrame(id(),FrameType::exemplar);
    //копируем слоты
    //сначала системные
    instFrame->name.initFromSlot(&this->name );
    instFrame->is_a.initFromSlot(&this->is_a );
    //остальные
    for(int i=2;i<this->slotCount();i++)
    {
        NSlot *slot = new NSlot();//this->getSlotByIndex(i)->clone();
        slot->setHasFasetValue(true);
        slot->initFromSlot(this->getSlotByIndex(i));
        instFrame->addSlot(slot);
    }
    return instFrame;
}

void NFrame::merge(NFrame *frame)
{
    for(int i=0;i<frame->slotCount();i++)
    {
        NSlot* slot = frame->getSlotByIndex(i);
        if(!hasSlot(slot->name()))
        {
            this->addSlot(slot);
        }
    }
}

bool NFrame::hasSlot(QString slotName)
{
    NSlot *slot = getSlotByName(slotName);
//    if(slot)
//        return true;
//    else
//        return false;
    return slot!=NULL;
}

QList<NSlot*> NFrame::getSlotsByDefValue(QString defVal)
{
    QList<NSlot*> list;
    NSlot* s;
    foreach(s,m_slots)
    {
        QString val = s->defValue().toString();
        if(val==defVal)
            list<<s;
    }
    return list;
}

void NFrame::setId(int id)
{
    this->m_id =id;
}
