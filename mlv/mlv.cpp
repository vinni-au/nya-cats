#include "mlv.h"
#include <QList>

MLV::MLV(NKBManager* manager, Grid* grid) :
   m_KBManager(manager)
  ,m_Grid(grid)
  ,m_Initialized(false)
{
}

NFrame* MLV::CreateFrameInstance(QString name)
{
    if (!m_KBManager)
        return NULL;
    return m_KBManager->GetFrameInstance(name);
}

NFrame* MLV::CreateFrameInstanceFull(QString name)
{
    if (!m_KBManager)
        return NULL;
    return m_KBManager->GetFrameInstance(name);
}

void MLV::SetFasetValue(NFrame* frame, QString fasetName, int value)
{
    if (!frame)
        return;

    if (NFaset* faset = frame->GetSlotFaset(fasetName, "value"))
        faset->setIntValue(value);
}

void MLV::SetFasetValueVariant(NFrame* frame, QString fasetName, QVariant value)
{
    if (!frame)
        return;

    if (NFaset* faset = frame->GetSlotFaset(fasetName, "value"))
        faset->setValue(value);
}

void MLV::SetFasetValue(NFrame* frame, QString fasetName, QString value)
{
    if (!frame)
        return;

    if (NFaset* faset = frame->GetSlotFaset(fasetName, "value"))
        faset->setStringValue(value);
}

QVariant MLV::GetFasetValue(NFrame* frame, QString fasetName)
{
    if (!frame)
        return QVariant();

    if (NFaset* faset = frame->GetSlotFaset(fasetName, "value"))
        return faset->value();
    else
        return QVariant();
}

bool MLV::Init()
{
    m_CellFrameInsts.clear();

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
            SetFasetValue(CellInst, "x", i);
            SetFasetValue(CellInst, "y", j);

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
                SetFasetValue(ItemInst, "Команда", item->GetTeam() == gtRed ? "Красный" : "Синий");
            }
            SetFasetValueVariant(CellInst, "Игровой объект", QVariant(reinterpret_cast<long long>(ItemInst)));
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
        QVariant value = GetFasetValue(m_CellFrameInsts[i], "Игровой объект");
        NFrame* frame = (NFrame*)value.toLongLong();
        if (!frame)
            continue;

        if (frame->frameName() != "Пусто")
        {
            NFrame* frameSituation = CreateFrameInstance("Ситуация");
            SetFasetValueVariant(frameSituation, "Место выполнения действия",
                                 QVariant(reinterpret_cast<long long>(m_CellFrameInsts[i])));
            SetFasetValueVariant(frameSituation, "Игрок", QVariant(reinterpret_cast<long long>(frame)));

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
    // TODO
    return true;
}


QVariant MLV::getVal(int frameId,QString aimVar)
{
    //TODO
    // Для примера можно посмотреть NKBManager:  NSlot * getSlotByString( QString frameName, QString str  );

    NFrame* frame = NULL;
    for (int i = 0; i < m_WorkMemory.count(); i++)
    {
        frame = m_WorkMemory[i];
        if (!frame)
            continue;
        if (frame->id() == frameId)
            break;
    }

    if (!frame)
        return QVariant();

    // Тут как то надо парсить строку aimVar и соотв. бегать по субфреймам

    return GetFasetValue(frame, aimVar);
}

void MLV::UpdateGrid()
{

}
