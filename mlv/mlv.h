#ifndef MLV_H
#define MLV_H

#include "core/nkbmanager.h"
#include "core/nframe.h"
#include "core/nslot.h"
#include "core/nfaset.h"
#include "core/nproc.h"
#include "visualize/visualizer.h"
#include "visualize/grid.h"

class MLV
{
protected:
    NKBManager* m_KBManager;
    NFrame* m_CurrFrame;
    Grid* m_Grid;

public:
    MLV(NKBManager* manager, Grid* grid);
    void Start(NFrame* frame);
    bool BindFrame(NFrame* frame);
    bool BindSlot(NSlot* slot);

    //Для продукционного вывода
    //Функция для получения значения слота фрейма экземпляра при продукционном выводе.
    //frameId - id фрейма-экземпляра
    //aimVar переменная вида slotname или slotName.SlotName.SlotName -если субфреймы
    //return Значение целевого слота
    QVariant getVal(int frameId,QString aimVar);
protected:

};

#endif // MLV_H
