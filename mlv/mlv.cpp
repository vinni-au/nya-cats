#include "mlv.h"
#include <QList>

MLV::MLV(NKBManager* manager, Grid* grid) :
   m_KBManager(manager)
  ,m_Grid(grid)
  ,m_Initialized(false)
{
}

bool MLV::Init()
{
    // Экземпляр игрового поля
    m_GameFieldInst = m_KBManager->GetFrameInstance("Игровое поле");
    if (!m_GameFieldInst)
        return false;

    // А надо ли создавать фреймы экземпляры для всех ячеек???

    // Создаем экземпляры фреймов для персонажей, раскиданых по игровому полю
    for (int i = 0; i < m_Grid->GetCount(); i++)
    {
        GameItem* item = m_Grid->GetGameItem(i);
        if (!item)
            continue;

        NFrame* PersonInst = NULL;

        if (item->GetType() == gitWarior)
            PersonInst = m_KBManager->GetFrameInstance("Мечник");

        else if (item->GetType() == gitArcher)
            PersonInst = m_KBManager->GetFrameInstance("Лучник");

        else if (item->GetType() == gitHealer)
            PersonInst = m_KBManager->GetFrameInstance("Лекарь");

        if (!PersonInst)
            continue;

        // Заполняем координаты персонажа
        if (NFaset* faset = PersonInst->GetSlotFaset("x", "value"))
            faset->setIntValue(m_Grid->GetCell(i)->GetX());

        if (NFaset* faset = PersonInst->GetSlotFaset("y", "value"))
            faset->setIntValue(m_Grid->GetCell(i)->GetY());

        // Задаем цвет
        if (NFaset* faset = PersonInst->GetSlotFaset("Команда", "value"))
            faset->setStringValue(item->GetTeam() == gtRed ? "Красный" : "Синий");

        m_PersonInsts.append(PersonInst);
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
    for (int i = 0; i < m_PersonInsts.count(); i++)
    {
        BindFrame(m_KBManager->GetFrameByName("Ситуация"));
    }

    UpdateGrid();
}

// Жесткая рекурсия, надо тестить
bool MLV::BindFrame(NFrame *frame)
{
    bool retn = true;

    // Создаем экземпляр
    NFrame* frameInst = frame->createInstance();
    if (!frameInst)
        return false;

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
    return QVariant();
}

void MLV::UpdateGrid()
{

}
