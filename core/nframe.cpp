#include "nframe.h"

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


int NFrame::parentFrame()
{
//TODO
}

int NFrame::id()
{
    return m_id;
}

//сериализация
QDomElement NFrame::toXml(QDomDocument& doc)
{
//TODO
}

void NFrame::fromXml(QDomElement &frame)
{
//TODO
}
