#ifndef MLV_H
#define MLV_H

#include <QDebug>
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

    QList<NFrame*> m_WorkMemory;        // Рабочая память, тут хранятся фреймы, которые привязались
    QList<NFrame*> m_Cache;             // Тут хранятся ВСЕ фреймы экхемпляры


    bool m_Initialized;
    int m_InstCount;

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

    QList<NFrame*> workMemory() const
    {   return m_WorkMemory;    }

protected:

    // Тут создаются фреймы экземпляры игрового поля и персонаже.
    // В код вшиты названия! что есть очень плохо - быдло быдло
    bool Init();
    void UpdateGrid();

    NFrame* CreateFrameInstance(QString name, bool fillDefault = true);
    NFrame* CreateFrameInstanceFull(QString name, bool fillDefault = true);
    bool SetSlotValue(NFrame* frame, QString slotName, int value);
    bool SetSlotValue(NFrame* frame, QString slotName, QString value);
    bool SetSlotValueVariant(NFrame* frame, QString slotName, QVariant value, bool findInParents = false);

    QVariant GetSlotValue(NFrame* frame, QString slotName, bool findInParents = false);


    // Поиск в рабочей памяти
    NFrame* FindByProtName(QString name);   // Поиск по имени прототипа
    NFrame* FindByInstName(QString name);   // Поиск по уникальному имени экземпляра
    NFrame* FindByPtr(NFrame* framePtr);
    NFrame* FindById(int frameId);

    // Поиск в списке ячеек
    NFrame* FindByCell(int x, int y);

    // Поиск в кеше
    NFrame* FindInCache(QString name);
    NFrame* FindInCache(int frameId);


    bool IsPerson(NFrame* frame);
    bool IsFood(NFrame* frame);
    void InitNeighborSituation(NFrame* frameSituation, NFrame* item, QString slotName);

public slots:
    QVariant getValSlot(int frameId, QString aimVar);
    bool setValSlot(int frameId, QString aimVar, QVariant value);

signals:
    void AddMsgToLog(QString msg);
    void ClearLog();
};

#endif // MLV_H
