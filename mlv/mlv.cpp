#include "mlv.h"
#include <QList>

MLV::MLV(NKBManager* manager, Grid* grid) :
    m_KBManager(manager)
  ,m_Grid(grid)
{
}

void MLV::Start(NFrame *frame)
{
    m_CurrFrame = frame;
    bool allRigth = false;

    if (frame)
        allRigth = BindFrame(frame);

}

bool MLV::BindFrame(NFrame *frame)
{
    bool retn = true;
    QList<NSlot*> nslots = m_KBManager->GetFrameSlots(frame);
    for (int i = 0; i < nslots.count(); i++)
    {
        retn &= BindSlot(nslots[i]);
        if (!retn)
            break; // если хоть один слот не привязался, завершаем попытку
    }

    return retn;
}

bool MLV::BindSlot(NSlot *slot)
{
    //
    return true;
}
