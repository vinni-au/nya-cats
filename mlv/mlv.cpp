#include "mlv.h"
#include <QList>

MLV::MLV(NKBManager* manager, Grid* grid) :
    QObject(0)
  ,m_KBManager(manager)
  ,m_Grid(grid)
  ,m_Initialized(false)
{
}

NFrame* MLV::CreateFrameInstance(QString name)
{
    if (!m_KBManager)
        return NULL;

    NFrame* frame = m_KBManager->GetFrameInstance(name);
    m_WorkMemory.append(frame);
    return frame;
}

NFrame* MLV::CreateFrameInstanceFull(QString name)
{
    if (!m_KBManager)
        return NULL;

    NFrame* frame = m_KBManager->GetFrameInstanceWithParents(name);
    m_WorkMemory.append(frame);
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
        NFrame* parentFrame = m_KBManager->GetFrameParent(frame);
        return GetSlotValue(parentFrame, slotName);
    }
    else
    {
        if (NFaset* faset = frame->GetSlotFaset(slotName, "value"))
            return faset->value();
    }

    return QVariant();
}

bool MLV::Init()
{
    m_CellFrameInsts.clear();
    m_WorkMemory.clear();

    // Экземпляр игрового поля
    m_GameFieldInst = CreateFrameInstanceFull("Игровое поле");
    if (!m_GameFieldInst)
        return false;

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
                // Создаем персонажа
                if (item->GetType() == gitWarior)
                    ItemInst = CreateFrameInstanceFull("Мечник");

                else if (item->GetType() == gitArcher)
                    ItemInst = CreateFrameInstanceFull("Лучник");

                else if (item->GetType() == gitHealer)
                    ItemInst = CreateFrameInstanceFull("Лекарь");

                // Задаем цвет
                SetSlotValue(ItemInst, "Команда", item->GetTeam() == gtRed ? "Красный" : "Синий");
            }
            SetSlotValueVariant(CellInst, "Игровой объект", QVariant(reinterpret_cast<long long>(ItemInst)));
            m_CellFrameInsts.append(CellInst);
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
        if (frame->frameName() != "Пусто")
        {
            NFrame* frameSituation = CreateFrameInstance("Ситуация");
            SetSlotValueVariant(frameSituation, "Место выполнения действия",
                                 QVariant(reinterpret_cast<long long>(m_CellFrameInsts[i])));
            SetSlotValueVariant(frameSituation, "Игрок", QVariant(reinterpret_cast<long long>(frame)));

            BindFrame(frameSituation);
        }
    }

    UpdateGrid();
}

// Жесткая рекурсия, надо тестить
bool MLV::BindFrame(NFrame *frame)
{
    bool retn = true;

    NFrame* frameInst = NULL;
    if (frame->frameType() == FrameType::prototype)
    {
        // Создаем экземпляр
        NFrame* frameInst = frame->createInstance();
        if (!frameInst)
            return false;
    }
    else
        frameInst = frame;

    // Пытаемся привязать слоты
    QList<NSlot*> nslots = m_KBManager->GetFrameSlots(frameInst);
    for (int i = 0; i < nslots.count(); i++)
    {
        retn &= BindSlot(nslots[i]);
        if (!retn)
            break; // если хоть один слот не привязался, завершаем перебор
    }

    // Если фрейм привязался, пытаемся привязать потомков
    if (retn)
    {
        QList<NFrame*> children = m_KBManager->GetFrameChildren(frame);
        for (int i = 0; i < children.count(); i++)
        {
            if (BindFrame(children[i]))
                break;
        }
    }

    return retn;
}

bool MLV::BindSlot(NSlot *slot)
{
    if (!slot)
        return false;

    if (slot->getSlotType() == "")
        return false;

    // Если субфрейм
    if (slot->getSlotType() == "frame")
    {
        // Создаем экземпляр
        QString subframeName = slot->defValue().toString();
        NFrame* subframe = CreateFrameInstance(subframeName);
        if (!subframe)
            return false;

        // Вызваем привязку экземпляра
        return BindFrame(subframe);
    }

    //Сюда попадаем, если тип слота - какой-то домен, строка или число
    QString markerType = slot->getSlotMarkerType();

    if (markerType == "domain")
    {

    }

    if (markerType == "procedure")
    {

    }

    if (markerType == "production")
    {

    }

    return false;
}

NFrame* MLV::getFrameFromWorkMem(int frameId)
{
    NFrame* frame = NULL;
    for (int i = 0; i < m_WorkMemory.count(); i++)
    {
        frame = m_WorkMemory[i];
        if (!frame)
            continue;
        if (frame->id() == frameId)
            break;
    }
    return frame;
}

QVariant MLV::getVal(int frameId, QString aimVar)
{
    //TODO
    // Для примера можно посмотреть NKBManager:  NSlot * getSlotByString( QString frameName, QString str  );

    return getVal(getFrameFromWorkMem(frameId), aimVar);
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
    return setVal(getFrameFromWorkMem(frameId), aimVar, value);
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
