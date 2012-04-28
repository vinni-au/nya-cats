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

    NFrame* m_GameFieldInst;        // Экземпляр фрейма игрового поля
    QList<NFrame*> m_CellFrameInsts;   // Экземпляры фреймов ячеек

    bool m_Initialized;

public:
    MLV(NKBManager* manager, Grid* grid);

    void Start();
    void Step();
    bool BindFrame(NFrame* frame);
    bool BindSlot(NSlot* slot);

    //Для продукционного вывода
    //Функция для получения значения слота фрейма экземпляра при продукционном выводе.
    //frameId - id фрейма-экземпляра
    //aimVar переменная вида slotname или slotName.SlotName.SlotName - если субфреймы
    //return Значение целевого слота
    QVariant getVal(int frameId,QString aimVar);

protected:

    // Тут создаются фреймы экземпляры игрового поля и персонаже.
    // В код вшиты названия! что есть очень плохо - быдло быдло
    bool Init();
    void UpdateGrid();

    NFrame* CreateFrameInstance(QString name);
    void SetFasetValue(NFrame* frame, QString fasetName, int value);
    void SetFasetValue(NFrame* frame, QString fasetName, QString value);

    QVariant GetFasetValue(NFrame* frame, QString fasetName);


};

#endif // MLV_H
