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
#include <QScriptValue>
#include <QScriptEngine>
#include "QScriptProxy/qsproxycell.h"
#include "mlv_define.h"
#include <QDateTime>


class MLV : public QObject
{
    Q_OBJECT
protected:
    NKBManager* m_KBManager;
    NFrame* m_CurrFrame;
    Grid* m_Grid;
	Visualizer* m_Viz;
    NFrame* m_GameFieldInst;            // Экземпляр фрейма игрового поля
    QList<NFrame*> m_CellFrameInsts;    // Экземпляры фреймов ячеек
	QList<NFrame*> m_ItemFrameInsts;    // Экземпляры фреймов персонажей
    QList<NFrame*> m_WorkMemory;        // Рабочая память, тут хранятся фреймы, которые привязались
    QList<NFrame*> m_Cache;             // Тут хранятся ВСЕ фреймы экхемпляры

    bool m_Initialized;
    int m_InstCount;
    int m_Padding;
    bool m_FullSearch;
    bool m_GameContinues;
	bool m_RandomBind;

public:
    MLV(NKBManager* manager, Visualizer* viz);

    void Start();
	void RandomStart();
    void Start(int x, int y);
    void Step();
    void Step(int x, int y);
    void Stop();

    //Для продукционного вывода
    //Функция для получения значения слота фрейма экземпляра при продукционном выводе.
    //frameId - id фрейма-экземпляра
    //aimVar переменная вида slotname или slotName.SlotName.SlotName - если субфреймы
    //return Значение целевого слота
    QVariant getVal(int frameId, QString aimVar);
    QVariant getVal(NFrame* frame, QString aimVar);
    bool setVal(int frameId, QString aimVar, QVariant value);
    bool setVal(NFrame* frame, QString aimVar, QVariant value);
    QList<NFrame*>* workMemory() {return &m_WorkMemory;}
    bool GetFullSearch() {return m_FullSearch;}
    void SetFullSearch(bool b) {m_FullSearch = b;}
	bool GetRandomBind() {return m_RandomBind;}
	void SetRandomBind(bool b) {m_RandomBind = b;}

    bool isGameContinues();                                 //запущена ли в данный момент игра
	QList<NFrame*> getSituationInstanceList();
	QList<NFrame*> getSituationInstanceList(NFrame* cell);
	QList<NFrame*> getImageInstanceList(NFrame* item);
	QList<NFrame*> getFrameLeaf(NFrame* root);

protected:

    // Тут создаются фреймы экземпляры игрового поля и персонаже.
    // В код вшиты названия! что есть очень плохо - быдло быдло
    bool Init();
    void UpdateGrid();
	void UpdateCell(NFrame* cellInst, NFrame* imageFrame);
	void ClearWorkMem();

    bool BindFrame(NFrame* frame, bool fillDefault = false, bool bindChildren = true);
    bool BindSlot(NFrame* frame, NSlot* slot, bool fillDefault = false);
    NFrame* BindPerson(NFrame* frame);
    NFrame* BindPerson(int x, int y);

	void FillSubSituation(NFrame* mainSit, NFrame* subSit);
	void FillSituationByCell(NFrame* cell, NFrame* situation);

	void MoveItemInst(NFrame* itemInst, NFrame* toCellInst);
	void DoCell(NFrame* cell);
	void DoCell(int x, int y);
	NFrame* BindImage(NFrame* cell);

	NFrame* CreateRandGameItemInst();
	NFrame* CreateGameItemInst(GameItem* gameItem);
    NFrame* CreateFrameInstance(QString name, bool fillDefault = true);
    NFrame* CreateFrameInstanceFull(QString name, bool fillDefault = true);
    bool SetSlotValue(NFrame* frame, QString slotName, int value);
    bool SetSlotValue(NFrame* frame, QString slotName, QString value);
    bool SetSlotValueVariant(NFrame* frame, QString slotName, QVariant value, bool findInParents = false);

    QVariant GetSlotValue(NFrame* frame, QString slotName, bool findInParents = false);
	NFrame* GetSubframe(NFrame* frame, QString slotName, bool findInParents = false);
	void SetSubframe(NFrame* frame, QString slotName, NFrame* subframe, bool findInParents = false);
	void AddSubframe(NFrame* frame, QString slotName, NFrame* subframe, bool findInParents = false);

	// Работа с процедурами
    void DoAction(NFrame* frameSituation);
    Q_INVOKABLE void ShowMsg(QString msg);
    Q_INVOKABLE void NowhereToGo(QScriptValue qsCell);

    // Поиск в рабочей памяти
    NFrame* FindByProtName(QString name);   // Поиск по имени прототипа
    NFrame* FindByInstName(QString name);   // Поиск по уникальному имени экземпляра
    NFrame* FindByPtr(NFrame* framePtr);
    NFrame* FindById(int frameId);

    // Поиск в списке ячеек
    NFrame* FindByCell(int x, int y);
	NFrame* FindCell(int x, int y);
	NFrame* FindCellByItemInst(NFrame* itemInst);

    // Поиск в кеше
    NFrame* FindInCache(QString name);
    NFrame* FindInCache(int frameId);

	bool IsContainGameItem(NFrame* cell);
	NFrame* GetGameInst(NFrame* cell);

    void InitNeighborSituation(NFrame* frameSituation, NFrame* item, QString Name);

    QString GetSpaces(int count);

public slots:
    QVariant getValSlot(int frameId, QString aimVar);
    bool setValSlot(int frameId, QString aimVar, QVariant value);
    bool getGameContinues() {return m_GameContinues;}

	void DoVisualizerCell(Cell* cell);

signals:
    void AddMsgToLog(QString msg);
    void ClearLog();

	// Для работы с LineOfReasoning
	void LOR_AddRoot(QString name);
	void LOR_AddNextLevel(QString name);
	void LOR_AddCurrLevel(QString name);
	void LOR_GotoPrevLevel();
	void LOR_SetCurrState(bool bOk);
	void LOR_Clear();
};

#endif // MLV_H
