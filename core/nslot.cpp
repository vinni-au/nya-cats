#include "nslot.h"

NSlot::NSlot(QObject *parent) :
    QObject(parent)
{
    m_system = false;

    //инициализация слотов
    slotName.setName("name");
    slotName.setStringValue("defName");
    fasets.append(&slotName);

    slotType.setName("slot_type");
    slotType.setStringValue("domain");
    fasets.append(&slotType);

    slotValue.setName("value");
    slotValue.setStringValue("defValue");
    fasets.append(&slotValue);

    slotDefaultValue.setName("default_value");
    slotDefaultValue.setStringValue("");
    fasets.append(&slotDefaultValue);

    slotMarkerType.setName("marker_type");
    slotMarkerType.setStringValue("domain");
    fasets.append(&slotMarkerType);

    slotMarker.setName("marker");
    slotMarker.setStringValue("defDomain");
    fasets.append(&slotMarker);

    slotInheritanceType.setName("inheritance_type");
    slotInheritanceType.setStringValue(INHERIT_SAME);
    fasets.append(&slotInheritanceType);


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
    foreach(faset,this->fasets)
    {
        if(faset->name()==name)
        {
            return faset;
        }
    }
    return faset;
}
