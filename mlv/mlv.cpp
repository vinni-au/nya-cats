#include "mlv.h"
#include <QList>

MLV::MLV(NKBManager* manager, Grid* grid) :
    QObject(0)
  ,m_KBManager(manager)
  ,m_Grid(grid)
  ,m_Initialized(false)
  ,m_InstCount(0)
  ,m_Padding(0)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Вспомогательные функции

NFrame* MLV::FindByProtName(QString name)
{
    NFrame* frame = NULL;
    for (int i = 0; i < m_WorkMemory.count(); i++)
    {
        if (m_WorkMemory[i]->frameName() == name)
        {
            frame = m_WorkMemory[i];
            return frame;
        }
    }
    return NULL;
}

NFrame* MLV::FindByInstName(QString name)
{
    NFrame* frame = NULL;
    for (int i = 0; i < m_WorkMemory.count(); i++)
    {
        if (GetSlotValue(m_WorkMemory[i], "name") == name)
        {
            frame = m_WorkMemory[i];
            return frame;
        }
    }
    return NULL;
}

NFrame* MLV::FindByPtr(NFrame* framePtr)
{
    NFrame* frame = NULL;
    for (int i = 0; i < m_WorkMemory.count(); i++)
    {
        if (m_WorkMemory[i] == framePtr)
        {
            frame = m_WorkMemory[i];
            return frame;
        }
    }
    return NULL;
}

NFrame* MLV::FindById(int frameId)
{
    NFrame* frame = NULL;
    for (int i = 0; i < m_WorkMemory.count(); i++)
    {
        frame = m_WorkMemory[i];
        if (!frame)
            continue;
        if (frame->id() == frameId)
            return frame;
    }
    return NULL;
}

NFrame* MLV::FindByCell(int x, int y)
{
    NFrame* frame = NULL;
    for (int i = 0; i < m_CellFrameInsts.count(); i++)
    {
        if (GetSlotValue(m_CellFrameInsts[i], "x") == x && GetSlotValue(m_CellFrameInsts[i], "y") == y)
        {
            frame = (NFrame*)GetSlotValue(m_CellFrameInsts[i], "Игровой объект").toLongLong();
            return frame;
        }
    }
    return NULL;
}

NFrame* MLV::FindInCache(QString name)
{
    NFrame* frame = NULL;
    for (int i = 0; i < m_Cache.count(); i++)
    {
        if (GetSlotValue(m_Cache[i], "name") == name)
        {
            frame = m_Cache[i];
            return frame;
        }
    }
    return NULL;
}

NFrame* MLV::FindInCache(int frameId)
{
    NFrame* frame = NULL;
    for (int i = 0; i < m_Cache.count(); i++)
    {
        if (m_Cache[i]->id() == frameId)
        {
            frame = m_Cache[i];
            return frame;
        }
    }
    return NULL;
}

NFrame* MLV::CreateFrameInstance(QString name, bool fillDefault)
{
    if (!m_KBManager)
        return NULL;

    NFrame* frame = m_KBManager->GetFrameInstance(name);

    if (fillDefault)
    {
        QList<NSlot*> slotsList = m_KBManager->GetFrameSlots(frame);
        for (int i = 0; i < slotsList.count(); i++)
        {
            QVariant val = GetSlotValue(frame, slotsList[i]->name());
            if (val.toString().isEmpty())
            {
                SetSlotValueVariant(frame, slotsList[i]->name(), slotsList[i]->defValue());
            }
        }
    }

    if (frame)
        m_Cache.append(frame);

    return frame;
}

NFrame* MLV::CreateFrameInstanceFull(QString name, bool fillDefault)
{
    if (!m_KBManager)
        return NULL;

    NFrame* frame = m_KBManager->GetFrameInstanceWithParents(name);

    if (fillDefault)
    {
        QList<NSlot*> slotsList = m_KBManager->GetFrameSlots(frame);
        for (int i = 0; i < slotsList.count(); i++)
        {
            QVariant val = GetSlotValue(frame, slotsList[i]->name());
            if (val.toString().isEmpty())
            {
                if (slotsList[i]->getSlotType() != "frame")
                    SetSlotValueVariant(frame, slotsList[i]->name(), slotsList[i]->defValue());
            }
        }
    }

    if (frame)
        m_Cache.append(frame);

    return frame;
}

bool MLV::SetSlotValue(NFrame* frame, QString slotName, int value)
{
    if (!frame)
        return false;

    return SetSlotValueVariant(frame, slotName, value);
}

bool MLV::SetSlotValueVariant(NFrame* frame, QString slotName, QVariant value, bool findInParents)
{
    if (!frame)
        return false;

    NSlot* slot = frame->getSlotByName(slotName);
    if(!slot)
    {
        if (!findInParents)
            return false;

        //ищем в родительских
        NFrame* parentFrame = m_KBManager->GetFrameParent(frame);
        return SetSlotValueVariant(parentFrame, slotName, true);
    }
    else
    {
        if (NFaset* faset = frame->GetSlotFaset(slotName, "value"))
        {
            faset->setValue(value);
            return true;
        }
    }

    return false;
}

bool MLV::SetSlotValue(NFrame* frame, QString slotName, QString value)
{
    if (!frame)
        return false;

    return SetSlotValueVariant(frame, slotName, value);
}

QVariant MLV::GetSlotValue(NFrame* frame, QString slotName, bool findInParents)
{
    if (!frame)
        return QVariant();

    NSlot* slot = frame->getSlotByName(slotName);
    if(!slot)
    {
        if (!findInParents)
            return QVariant();

        //ищем в родительских
        NFrame* parentFrame = (NFrame*)GetSlotValue(frame, "is_a").toLongLong();
        return GetSlotValue(parentFrame, slotName, findInParents);
    }
    else
    {
        if (NFaset* faset = frame->GetSlotFaset(slotName, "value"))
            return faset->value();
    }

    return QVariant();
}

QVariant MLV::getVal(int frameId, QString aimVar)
{
    return getVal(FindInCache(frameId), aimVar);
}

QVariant MLV::getVal(NFrame* frame, QString aimVar)
{
    if (!frame)
        return QVariant();

    //тут надо еще наследование учесть
    if(!aimVar.contains("."))
    {//просто слот
        return GetSlotValue(frame, aimVar, true);
    }
    else
    {//субфреймы
        int pointInx = aimVar.indexOf(".");
        QString newStr = aimVar.right(aimVar.length()-pointInx-1);
        QString subfName = aimVar.left(pointInx);

        QVariant slotVal = GetSlotValue(frame, subfName, true);
        NFrame* subframe = (NFrame*)slotVal.toLongLong();
        return getVal(subframe, newStr);
    }
}

bool MLV::setVal(int frameId, QString aimVar, QVariant value)
{
    return setVal(FindInCache(frameId), aimVar, value);
}

bool MLV::setVal(NFrame* frame, QString aimVar, QVariant value)
{
    if (!frame)
        return false;

    //тут надо еще наследование учесть
    if(!aimVar.contains("."))
    {//просто слот
        return SetSlotValueVariant(frame, aimVar, value);
    }
    else
    {//субфреймы
        int pointInx = aimVar.indexOf(".");
        QString newStr = aimVar.right(aimVar.length()-pointInx-1);
        QString subfName = aimVar.left(pointInx);

        QVariant slotVal = GetSlotValue(frame, subfName, true);
        NFrame* subframe = (NFrame*)slotVal.toLongLong();
        return setVal(subframe, newStr, value);
    }
}


void MLV::UpdateGrid()
{

}

QVariant MLV::getValSlot(int frameId, QString aimVar)
{
    return getVal(frameId, aimVar);
}

bool MLV::setValSlot(int frameId, QString aimVar, QVariant value)
{
    return setVal(frameId, aimVar, value);
}


////////////////////////////////////////////////////////////////////////
// Вывод

bool MLV::Init()
{
    m_CellFrameInsts.clear();
    m_WorkMemory.clear();
    m_Cache.clear();

    // Экземпляр игрового поля
    m_GameFieldInst = CreateFrameInstanceFull("Игровое поле");
    if (!m_GameFieldInst)
        return false;

    m_WorkMemory.append(m_GameFieldInst);

    // Создаем ячейки игрового поля
    for (int i = 0; i < m_Grid->GetSideCount(); i++)
    {
        for (int j = 0; j < m_Grid->GetSideCount(); j++)
        {
            // Создаем экземпляр фрейма ячейки
            NFrame* CellInst = CreateFrameInstanceFull("Ячейка игрового поля");
            if (!CellInst)
                return false;

            // Устанавливаем координаты ячейки
            SetSlotValue(CellInst, "x", i);
            SetSlotValue(CellInst, "y", j);

            // Устанавливаем игровой объект
            GameItem* item = m_Grid->GetGameItem(i, j);
            NFrame* ItemInst = NULL;
            if (!item) // Если ячейка пустая
            {
                ItemInst = CreateFrameInstanceFull("Пусто");
            }
            else
            {
                // Если мужик
                if (IsPerson(item))
                {
                    // Создаем персонажа
                    if (item->GetType() == gitWarior)
                        ItemInst = CreateFrameInstanceFull("Мечник");

                    else if (item->GetType() == gitArcher)
                        ItemInst = CreateFrameInstanceFull("Лучник");

                    else if (item->GetType() == gitHealer)
                        ItemInst = CreateFrameInstanceFull("Лекарь");

                    // Заполняем значения
                    SetSlotValue(ItemInst, "Команда", item->GetTeam() == gtRed ? "Красный" : "Синий");
                }

                // Если еда
                else if (IsFood(item))
                {
                    ItemInst = CreateFrameInstanceFull("Еда");
                }
            }

            SetSlotValueVariant(CellInst, "Игровой объект", QVariant(reinterpret_cast<long long>(ItemInst)));
            m_CellFrameInsts.append(CellInst);

            m_WorkMemory.append(CellInst);
            m_WorkMemory.append(ItemInst);
        }
    }
    m_Initialized = true;
    return true;
}

void MLV::Start()
{
    if (!Init())
        return;

    // Пока один шаг. В будущем тут можно будет замутить цикл со слипами
    Step();

}

// Костыль
bool MLV::IsPerson(NFrame* frame)
{
    if (frame == 0)
        return false;

    bool pers =  frame->frameName() == "Лучник" ||
                 frame->frameName() == "Мечник" ||
                 frame->frameName() == "Лекарь" ||
                 frame->frameName() == "Персонаж";

    return pers;
}

bool MLV::IsFood(NFrame* frame)
{
    if (frame == 0)
        return false;

    bool food = frame->frameName() == "Еда";

    return food;
}

bool MLV::IsPerson(GameItem* item)
{
    if (item == 0)
        return false;

    bool pers =     item->GetType() == gitWarior ||
                    item->GetType() == gitHealer ||
                    item->GetType() == gitArcher;
    return pers;
}

bool MLV::IsFood(GameItem* item)
{
    if (item == 0)
        return false;

    bool food = item->GetType() == gitMeat;

    return food;
}

void MLV::InitNeighborSituation(NFrame* frameSituation, NFrame* item, QString Orientation)
{
    if (IsPerson(item))
        SetSlotValueVariant(frameSituation, "Сосед " + Orientation, QVariant(reinterpret_cast<long long>(item)));
    else
        frameSituation->removeSlot("Сосед " + Orientation);

    if (IsFood(item))
        SetSlotValueVariant(frameSituation, "Еда " + Orientation, QVariant(reinterpret_cast<long long>(item)));
    else
        frameSituation->removeSlot("Еда " + Orientation);
}

QString MLV::GetSpaces(int count)
{
    QString str = "";
    for (int i = 0; i < count; i++)
    {
        str += "|      ";
    }
    return str;
}

void MLV::Step()
{
    if (!m_Initialized)
        return;

    // Для каждого персонажа привязываем ситуацию
    for (int i = 0; i < m_CellFrameInsts.count(); i++)
    {
        // Определяем что находится в ячейке
        QVariant value = GetSlotValue(m_CellFrameInsts[i], "Игровой объект");
        NFrame* frame = (NFrame*)value.toLongLong();
        if (!frame)
            continue;

        // Если не пусто
        if (IsPerson(frame))
        {
            m_Padding = 0;
            AddMsgToLog(GetSpaces(m_Padding) + "Определяем ситуацию для '" + frame->frameName().toUpper() + "'");

            NFrame* frameSituation = CreateFrameInstance("Ситуация");
            SetSlotValueVariant(frameSituation, "Место выполнения действия", QVariant(reinterpret_cast<long long>(m_CellFrameInsts[i])));
            SetSlotValueVariant(frameSituation, "Игрок", QVariant(reinterpret_cast<long long>(frame)));


            //////////////////////////
            // Заполнение верха, низа, права, лева
            {
                int x = GetSlotValue(m_CellFrameInsts[i], "x").toInt();
                int y = GetSlotValue(m_CellFrameInsts[i], "y").toInt();

                // Пробегаем по всем соседним ячейкам, и смотрим, есть ли там еда или враг
                // если есть - добавляем в ситуацию

                NFrame *cframe = 0;
                cframe = FindByCell(x, y - 1);
                InitNeighborSituation(frameSituation, cframe, "сверху");
                cframe = FindByCell(x, y + 1);
                InitNeighborSituation(frameSituation, cframe, "снизу");
                cframe = FindByCell(x + 1, y);
                InitNeighborSituation(frameSituation, cframe, "справа");
                cframe = FindByCell(x - 1, y);
                InitNeighborSituation(frameSituation, cframe, "слева");
            }

            m_Padding += 1;
            BindFrame(frameSituation);
            m_Padding -= 1;
            AddMsgToLog(GetSpaces(m_Padding) + "'" + frame->frameName().toUpper() + "' - конец вывода");
        }
    }

    UpdateGrid();
}

// Жесткая рекурсия, надо тестить
bool MLV::BindFrame(NFrame *frame)
{
    bool retn = true;
    if (!frame)
        return false;

    NFrame* frameInst = NULL;
    if (frame->frameType() == FrameType::prototype)
    {
        // Создаем экземпляр
        frameInst = CreateFrameInstance(frame->frameName(), false);
        if (!frameInst)
        {
            return false;
        }
    }
    else
    {
        frameInst = frame;

        // Если фрейм есть в рабочей памяти, то считаем, что он уже привязан
        if (FindByPtr(frame) != NULL)
        {
            return true;
        }
    }

    AddMsgToLog(GetSpaces(m_Padding) + "Пытаемся привязать фрейм '" + frameInst->frameName().toUpper() + "'");

    // Пытаемся привязать слоты
    QList<NSlot*> nslots = m_KBManager->GetFrameSlots(frameInst);
    for (int i = 0; i < nslots.count(); i++)
    {
        if (nslots[i]->isSystem())
            continue;
        m_Padding += 1;
        retn &= BindSlot(frameInst, nslots[i]);
        m_Padding -= 1;
        if (!retn)
            break; // если хоть один слот не привязался, завершаем перебор
    }

    if (retn)
    {
        m_WorkMemory.append(frame);

        // Пытаемся привязать потомков
        QList<NFrame*> children = m_KBManager->GetFrameChildren(frameInst);
        for (int i = 0; i < children.count(); i++)
        {
            frameInst = CreateFrameInstance(children[i]->frameName(), false);
            if (frameInst)
            {
                SetSlotValueVariant(frameInst, "is_a", QVariant(reinterpret_cast<long long>(frame)));
                m_Padding += 1;
                bool end = BindFrame(frameInst);
                m_Padding -= 1;
                if (end)
                    break;
            }
        }
    }

    QString str = retn? "' привязался" : "' НЕ привязался";
    AddMsgToLog(GetSpaces(m_Padding) + "Фрейм '" + frameInst->frameName().toUpper() + str);
    return retn;
}

bool MLV::BindSlot(NFrame* frame, NSlot *slot)
{
    bool retn = false;
    if (!slot || !frame)
        return false;

    // Если субфрейм
    if (slot->getSlotType() == "frame")
    {
        NFrame* subframe = (NFrame*)GetSlotValue(frame, slot->name()).toLongLong();
        if (!subframe)
        {
            // Создаем экземпляр
            QString subframeName = slot->defValue().toString();
            subframe = CreateFrameInstance(subframeName);
            if (!subframe)
                return false;
            SetSlotValueVariant(frame, slot->name(), QVariant(reinterpret_cast<long long>(subframe)));
        }

        // Вызваем привязку экземпляра
        return BindFrame(subframe);
    }

    AddMsgToLog(GetSpaces(m_Padding) + "Пытаемся привязать слот '" + slot->name().toUpper() + "'");

    // Если тип слота - домен, строка или число
    QString markerType  = slot->getSlotMarkerType();
    QString markerVal   = slot->getSlotMarker();

    if (!markerVal.isEmpty())
    {
        if (markerType == "production")
        {
            NProduction* production = m_KBManager->getProduction(slot->getSlotMarker());
            NProductionMLV* productionMLV = new NProductionMLV(this, frame->id(), production);
            QString val = productionMLV->StartConsultation(slot->name());
            SetSlotValue(frame, slot->name(), val);

            retn = !val.isEmpty();
        }
    }

    // Сравниваем с заданием отсутствия
    retn = slot->defValue() == GetSlotValue(frame, slot->name());

    QString str = retn? "' привязался" :"' НЕ привязался";
    AddMsgToLog(GetSpaces(m_Padding) + "Cлот '" + slot->name().toUpper() + str);
    return retn;
}
