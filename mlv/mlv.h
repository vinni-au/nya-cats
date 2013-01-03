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


////////////////////////////////////////////////////////////////////////////////////////////////////

#define SYSSTR_FRAMENAME_GAMEFIELD	"Игровое поле"
#define SYSSTR_FRAMENAME_GAMEITEM	"Игровой объект"
#define SYSSTR_FRAMENAME_GAMECELL	"Ячейка игрового поля"
#define SYSSTR_FRAMENAME_SITUATION	"Ситуация"
#define SYSSTR_FRAMENAME_PERSON		"Персонаж"
#define SYSSTR_FRAMENAME_EMPTY		"Пусто"
#define SYSSTR_FRAMENAME_FOOD		"Еда"

#define SYSSTR_SLOTNAME_CELL_LEFT	"Ячейка слева"
#define SYSSTR_SLOTNAME_CELL_RIGTH	"Ячейка справа"
#define SYSSTR_SLOTNAME_CELL_TOP	"Ячейка сверху"
#define SYSSTR_SLOTNAME_CELL_BOTTOM	"Ячейка снизу"
#define SYSSTR_SLOTNAME_CELL_GAMER	"Ячейка Игрока"
#define SYSSTR_SLOTNAME_ISA			"is_a"
#define SYSSTR_SLOTNAME_NAME		"name"
#define SYSSTR_SLOTNAME_ACTION		"action"
#define SYSSTR_SLOTNAME_X			"x"
#define SYSSTR_SLOTNAME_Y			"y"
#define SYSSTR_SLOTNAME_TEAM		"Команда"

#define SYSSTR_SLOTTYPE_FRAME		"frame"
#define SYSSTR_SLOTTYPE_PRODUCTION	"production"

#define SYSSTR_FASETNAME_VALUE		"value"


////////////////////////////////////////////////////////////////////////////////////////////////////


class MLV : public QObject
{
    Q_OBJECT
protected:
    NKBManager* m_KBManager;
    NFrame* m_CurrFrame;
    Grid* m_Grid;
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

public:
    MLV(NKBManager* manager, Grid* grid);

    void Start();
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

    bool isGameContinues();                                 //запущена ли в данный момент игра
	QList<NFrame*> getSituationInstanceList();
	QList<NFrame*> getFrameLeaf(NFrame* root);

protected:

    // Тут создаются фреймы экземпляры игрового поля и персонаже.
    // В код вшиты названия! что есть очень плохо - быдло быдло
    bool Init();
    void UpdateGrid();

    bool BindFrame(NFrame* frame);
    bool BindSlot(NFrame* frame, NSlot* slot);
    bool BindPerson(NFrame* frame);
    bool BindPerson(int x, int y);

    NFrame* CreateFrameInstance(QString name, bool fillDefault = true);
    NFrame* CreateFrameInstanceFull(QString name, bool fillDefault = true);
    bool SetSlotValue(NFrame* frame, QString slotName, int value);
    bool SetSlotValue(NFrame* frame, QString slotName, QString value);
    bool SetSlotValueVariant(NFrame* frame, QString slotName, QVariant value, bool findInParents = false);

    QVariant GetSlotValue(NFrame* frame, QString slotName, bool findInParents = false);
	NFrame* GetSubframe(NFrame* frame, QString slotName, bool findInParents = false);
	void SetSubframe(NFrame* frame, QString slotName, NFrame* subframe);

    void DoAction(NFrame* frameSituation);

    // Поиск в рабочей памяти
    NFrame* FindByProtName(QString name);   // Поиск по имени прототипа
    NFrame* FindByInstName(QString name);   // Поиск по уникальному имени экземпляра
    NFrame* FindByPtr(NFrame* framePtr);
    NFrame* FindById(int frameId);

    // Поиск в списке ячеек
    NFrame* FindByCell(int x, int y);

	NFrame* GetItemInst(NFrame* cell);

    // Поиск в кеше
    NFrame* FindInCache(QString name);
    NFrame* FindInCache(int frameId);

    NFrame* FindCell(int x, int y);

	bool IsContainGameItem(NFrame* cell);

    void InitNeighborSituation(NFrame* frameSituation, NFrame* item, QString Name);

    QString GetSpaces(int count);

public slots:
    QVariant getValSlot(int frameId, QString aimVar);
    bool setValSlot(int frameId, QString aimVar, QVariant value);
    bool getGameContinues() {return m_GameContinues;}

signals:
    void AddMsgToLog(QString msg);
    void ClearLog();
};

#endif // MLV_H
