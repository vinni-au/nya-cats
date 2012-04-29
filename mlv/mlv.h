#ifndef MLV_H
#define MLV_H

#include "core/nkbmanager.h"
#include "core/nframe.h"
#include "core/nslot.h"
#include "core/nfaset.h"
#include "core/nproc.h"
#include "visualize/visualizer.h"
#include "visualize/grid.h"
#include "core/production/nproductionmlv.h"

class MLV : public QObject
{
    Q_OBJECT
protected:
    NKBManager* m_KBManager;
    NFrame* m_CurrFrame;
    Grid* m_Grid;

    NFrame* m_GameFieldInst;        // Экземпляр фрейма игрового поля
    QList<NFrame*> m_CellFrameInsts;   // Экземпляры фреймов ячеек

    QList<NFrame*> m_WorkMemory;        // Рабочая память

    bool m_Initialized;

public:
    MLV(NKBManager* manager, Grid* grid);

    void Start();
    void Step();
    bool BindFrame(NFrame* frame);
    bool BindSlot(NFrame* frame, NSlot* slot);

    //Для продукционного вывода
    //Функция для получения значения слота фрейма экземпляра при продукционном выводе.
    //frameId - id фрейма-экземпляра
    //aimVar переменная вида slotname или slotName.SlotName.SlotName - если субфреймы
    //return Значение целевого слота
    QVariant getVal(int frameId, QString aimVar);
    QVariant getVal(NFrame* frame, QString aimVar);

    bool setVal(int frameId, QString aimVar, QVariant value);
    bool setVal(NFrame* frame, QString aimVar, QVariant value);

protected:

    // Тут создаются фреймы экземпляры игрового поля и персонаже.
    // В код вшиты названия! что есть очень плохо - быдло быдло
    bool Init();
    void UpdateGrid();

    NFrame* CreateFrameInstance(QString name);
    NFrame* CreateFrameInstanceFull(QString name);
    bool SetSlotValue(NFrame* frame, QString fasetName, int value);
    bool SetSlotValue(NFrame* frame, QString fasetName, QString value);
    bool SetSlotValueVariant(NFrame* frame, QString fasetName, QVariant value, bool findInParents = false);

    QVariant GetSlotValue(NFrame* frame, QString slotName, bool findInParents = false);

    NFrame* getFrameFromWorkMem(int frameId);

public slots:
    QVariant getValSlot(int frameId, QString aimVar);
    bool setValSlot(int frameId, QString aimVar, QVariant value);

signals:
    void AddMsgToLog(QString msg);
    void ClearLog();
};

#endif // MLV_H
