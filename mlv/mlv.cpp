#include "mlv.h"
#include <QList>

MLV::MLV(NKBManager* manager, Grid* grid) :
    QObject(0)
  ,m_KBManager(manager)
  ,m_Grid(grid)
  ,m_Initialized(false)
  ,m_InstCount(0)
  ,m_Padding(0)
  ,m_FullSearch(false)
  ,m_GameContinues(false)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Вспомогательные функции

NFrame* MLV::FindByProtName(QString name)
{
    NFrame* frame = NULL;
    for (int i = 0; i < m_WorkMemory.count(); i++)
    {
        if (m_WorkMemory[i]->frameName() == name)
        {
            frame = m_WorkMemory[i];
            return frame;
        }
    }
    return NULL;
}

NFrame* MLV::FindByInstName(QString name)
{
    NFrame* frame = NULL;
    for (int i = 0; i < m_WorkMemory.count(); i++)
    {
        if (GetSlotValue(m_WorkMemory[i], SYSSTR_SLOTNAME_NAME) == name)
        {
            frame = m_WorkMemory[i];
            return frame;
        }
    }
    return NULL;
}

NFrame* MLV::FindByPtr(NFrame* framePtr)
{
    NFrame* frame = NULL;
    for (int i = 0; i < m_WorkMemory.count(); i++)
    {
        if (m_WorkMemory[i] == framePtr)
        {
            frame = m_WorkMemory[i];
            return frame;
        }
    }
    return NULL;
}

NFrame* MLV::FindById(int frameId)
{
    NFrame* frame = NULL;
    for (int i = 0; i < m_WorkMemory.count(); i++)
    {
        frame = m_WorkMemory[i];
        if (!frame)
            continue;
        if (frame->id() == frameId)
            return frame;
    }
    return NULL;
}

NFrame* MLV::FindByCell(int x, int y)
{
    NFrame* cell = FindCell(x, y);
    if (!cell)
        return NULL;

    NFrame* frame = GetSubframe(cell, SYSSTR_FRAMENAME_GAMEITEM);
    return frame;
}

NFrame* MLV::FindCell(int x, int y)
{
    for (int i = 0; i < m_CellFrameInsts.count(); i++)
    {
        if (GetSlotValue(m_CellFrameInsts[i], SYSSTR_SLOTNAME_X) == x &&
                GetSlotValue(m_CellFrameInsts[i], SYSSTR_SLOTNAME_Y) == y)
        {
            return m_CellFrameInsts[i];
        }
    }
    return NULL;
}

/* 
	Получаем из экземпляра ячейки экземпляр игрового объекта,
	который в ней содержится
*/
NFrame* MLV::GetItemInst(NFrame* cell)
{
	// Действительно ли это ячейка игрового поля
	if (!m_KBManager->HasParentWithName(cell, SYSSTR_FRAMENAME_GAMECELL))
		return NULL;
	
	return GetSubframe(cell, SYSSTR_FRAMENAME_GAMEITEM);
}

NFrame* MLV::FindInCache(QString name)
{
    NFrame* frame = NULL;
    for (int i = 0; i < m_Cache.count(); i++)
    {
        if (GetSlotValue(m_Cache[i], SYSSTR_SLOTNAME_NAME) == name)
        {
            frame = m_Cache[i];
            return frame;
        }
    }
    return NULL;
}

NFrame* MLV::FindInCache(int frameId)
{
    NFrame* frame = NULL;
    for (int i = 0; i < m_Cache.count(); i++)
    {
        if (m_Cache[i]->id() == frameId)
        {
            frame = m_Cache[i];
            return frame;
        }
    }
    return NULL;
}

NFrame* MLV::CreateFrameInstance(QString name, bool fillDefault)
{
    if (!m_KBManager)
        return NULL;

    NFrame* frame = m_KBManager->GetFrameInstance(name);

    if (fillDefault)
    {
        QList<NSlot*> slotsList = m_KBManager->GetFrameSlots(frame);
        for (int i = 0; i < slotsList.count(); i++)
        {
            QVariant val = GetSlotValue(frame, slotsList[i]->name());
            if (val.toString().isEmpty())
            {
                SetSlotValueVariant(frame, slotsList[i]->name(), slotsList[i]->defValue());
            }
        }
    }

    if (frame)
        m_Cache.append(frame);

    return frame;
}

NFrame* MLV::CreateFrameInstanceFull(QString name, bool fillDefault)
{
    if (!m_KBManager)
        return NULL;

    NFrame* frame = m_KBManager->GetFrameInstanceWithParents(name);
	if (frame == NULL)
		return NULL;

    if (fillDefault)
    {
        QList<NSlot*> slotsList = m_KBManager->GetFrameSlots(frame);
        for (int i = 0; i < slotsList.count(); i++)
        {
            QVariant val = GetSlotValue(frame, slotsList[i]->name());
            if (val.toString().isEmpty())
            {
                if (slotsList[i]->getSlotType() != SYSSTR_SLOTTYPE_FRAME)
                    SetSlotValueVariant(frame, slotsList[i]->name(), slotsList[i]->defValue());
            }
        }
    }

    if (frame)
        m_Cache.append(frame);

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
        if (NFaset* faset = frame->GetSlotFaset(slotName, SYSSTR_FASETNAME_VALUE))
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
        NFrame* parentFrame = GetSubframe(frame, SYSSTR_SLOTNAME_ISA);
        return GetSlotValue(parentFrame, slotName, findInParents);
    }
    else
    {
        if (NFaset* faset = frame->GetSlotFaset(slotName, SYSSTR_FASETNAME_VALUE))
            return faset->value();
    }

    return QVariant();
}

NFrame* MLV::GetSubframe(NFrame* frame, QString slotName, bool findInParents)
{
	return (NFrame*)GetSlotValue(frame, slotName, findInParents).toLongLong();
}

void MLV::SetSubframe(NFrame* frame, QString slotName, NFrame* subframe)
{
	 SetSlotValueVariant(frame, slotName, QVariant(reinterpret_cast<long long>(subframe)));
}

QVariant MLV::getVal(int frameId, QString aimVar)
{
    return getVal(FindInCache(frameId), aimVar);
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
    return setVal(FindInCache(frameId), aimVar, value);
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
	if (m_Grid == NULL) return;
	
	for(int i = 0; i < m_CellFrameInsts.count(); i++)
	{
		NFrame* cellInst = m_CellFrameInsts[i];
		NFrame* itemInst = GetItemInst(cellInst);
		if (itemInst == NULL)
			continue;

		int newX = GetSlotValue(cellInst, SYSSTR_SLOTNAME_X, true).toInt();
		int newY = GetSlotValue(cellInst, SYSSTR_SLOTNAME_Y, true).toInt();

		Cell* cell = m_Grid->FindCellByItemFrameId(itemInst->id());
		if (cell == NULL) // Такого по идее быть не должно тут
			continue;

		int oldX = cell->GetX();
		int oldY = cell->GetY();

		// Если координаты не совпадают, перемещаем игровой объект
		if (oldX != newX || oldY != newY)
		{
			m_Grid->MoveGameItemTo(oldX, oldY, newX, newY);
		}
	}
}


QVariant MLV::getValSlot(int frameId, QString aimVar)
{
    return getVal(frameId, aimVar);
}

bool MLV::setValSlot(int frameId, QString aimVar, QVariant value)
{
    return setVal(frameId, aimVar, value);
}

QList<NFrame*> MLV::getFrameLeaf(NFrame* root)
{
	return m_KBManager->getFrameLeaf(root);
}

QList<NFrame*> MLV::getSituationInstanceList()
{
	QList<NFrame*> list;
	for (int i = 0; i < m_WorkMemory.count(); i++)
	{
		NFrame* frame = m_WorkMemory[i];
		if (m_KBManager->HasParentWithName(frame, SYSSTR_FRAMENAME_SITUATION) && !m_KBManager->hasChildren(frame))
			list.append(frame);
	}
	return list;
}


////////////////////////////////////////////////////////////////////////
// Вывод

bool MLV::Init()
{
    m_CellFrameInsts.clear();
    m_WorkMemory.clear();
    m_Cache.clear();
    m_KBManager->clearExemplarIds();

    // Экземпляр игрового поля
    m_GameFieldInst = CreateFrameInstanceFull(SYSSTR_FRAMENAME_GAMEFIELD);
    if (!m_GameFieldInst)
        return false;

    // Создаем ячейки игрового поля
    for (int i = 0; i < m_Grid->GetSideCount(); i++)
    {
        for (int j = 0; j < m_Grid->GetSideCount(); j++)
        {
            // Создаем экземпляр фрейма ячейки
            NFrame* CellInst = CreateFrameInstanceFull(SYSSTR_FRAMENAME_GAMECELL);
            if (!CellInst)
                return false;

            // Устанавливаем координаты ячейки
            SetSlotValue(CellInst, SYSSTR_SLOTNAME_X, i);
            SetSlotValue(CellInst, SYSSTR_SLOTNAME_Y, j);

            // Устанавливаем игровой объект
            GameItem* item = m_Grid->GetGameItem(i, j);
            NFrame* ItemInst = NULL;
            if (!item) // Если ячейка пустая
            {
                ItemInst = CreateFrameInstanceFull(SYSSTR_FRAMENAME_EMPTY);
            }
            else
            {
                // Создаем персонажа
                ItemInst = CreateFrameInstanceFull(item->GetType());

                // Заполняем значения
                SetSlotValue(ItemInst, SYSSTR_SLOTNAME_TEAM, item->GetTeam());

				// Сохраняем в итем игрового объекта ид экземпляра соотв. ему фрейма
				item->SetFrameId(ItemInst->id());
            }

            SetSubframe(CellInst, SYSSTR_FRAMENAME_GAMEITEM, ItemInst);
            m_CellFrameInsts.append(CellInst);
			m_ItemFrameInsts.append(ItemInst);
        }
    }
    m_Initialized = true;
    return true;
}

bool MLV::IsContainGameItem(NFrame* cell)
{
	NFrame* item = GetSubframe(cell, SYSSTR_FRAMENAME_GAMEITEM);
	if (item == NULL)
		return false;
	return m_KBManager->HasParentWithName(item, SYSSTR_FRAMENAME_GAMEITEM);
}

void MLV::InitNeighborSituation(NFrame* frameSituation, NFrame* cell, QString Name)
{
    if (IsContainGameItem(cell))
        SetSubframe(frameSituation, Name, cell);
    else
        frameSituation->removeSlot(Name);
}

QString MLV::GetSpaces(int count)
{
    QString str = "";
    for (int i = 0; i < count; i++)
    {
        str += "|      ";
    }
    return str;
}

void MLV::Step()
{
    if (!m_Initialized || !m_GameContinues) return;
    qDebug()<<"void MLV::Step()";

    // Для каждого нового шага чистим рабочую память.
    m_WorkMemory.clear();

    // Загоняем в рабочую память экземпляр игрового поля, экземпляры ячеек и игровых объектов
    m_WorkMemory.append(m_GameFieldInst);
    for (int i = 0; i < m_CellFrameInsts.count(); i++)
        m_WorkMemory.append(m_CellFrameInsts[i]);

    for (int i = 0; i < m_ItemFrameInsts.count(); i++)
		m_WorkMemory.append(m_ItemFrameInsts[i]);

    // Для каждого персонажа привязываем ситуацию
    for (int i = 0; i < m_CellFrameInsts.count(); i++)
    {
        BindPerson(m_CellFrameInsts[i]);
    }

    //для каждой привязанной ситуации исполняем действие

    //надо где то запомнить привязанные к персонажам ситуации
    QList<NFrame*> situations = getSituationInstanceList();

    for (int i = 0; i < situations.count(); i++)
    {
        DoAction(situations.at(i));
    }
    //разрешаем конфликты
    //TODO

    //обновляем игровое поле
    UpdateGrid();
}

void MLV::Step(int x, int y)
{
    if (!m_Initialized || !m_GameContinues) return;
    BindPerson(x, y);
    UpdateGrid();
}

void MLV::Start(int x, int y)
{
    if (!Init()) return;
    m_GameContinues = true;
    Step(x, y);
}

void MLV::Start()
{
    if (!Init()) return;
    m_GameContinues = true;
    Step();
}

void MLV::Stop()
{
    m_GameFieldInst = false;
    m_GameContinues = false;
}

bool MLV::BindPerson(int x, int y)
{
    if (!Init())
        return false;

    NFrame* frame = FindCell(x ,y);
    return BindPerson(frame);
}

bool MLV::BindPerson(NFrame* cell)
{
    qDebug()<<"bool MLV::BindPerson(NFrame* cell)";
    bool retn = false;
    if (!cell)
        return retn;

    QVariant value = GetSlotValue(cell, SYSSTR_FRAMENAME_GAMEITEM);
    NFrame* frame = (NFrame*)value.toLongLong();
    if (!frame)
        return retn;

    m_Padding = 0;
    AddMsgToLog(GetSpaces(m_Padding) + "Определяем ситуацию для '" + frame->frameName().toUpper() + "'");

    NFrame* frameSituation = CreateFrameInstance(SYSSTR_FRAMENAME_SITUATION, false);
    SetSubframe(frameSituation, SYSSTR_SLOTNAME_CELL_GAMER, cell);


    //////////////////////////
    // Заполнение верха, низа, права, лева
    {
        int x = GetSlotValue(cell, SYSSTR_SLOTNAME_X).toInt();
        int y = GetSlotValue(cell, SYSSTR_SLOTNAME_Y).toInt();

        // Пробегаем по всем соседним ячейкам, и смотрим, есть ли там еда или враг
        // если есть - добавляем в ситуацию

        NFrame *cframe = 0;
        cframe = FindCell(x, y - 1);
        InitNeighborSituation(frameSituation, cframe, SYSSTR_SLOTNAME_CELL_TOP);
        cframe = FindCell(x, y + 1);
        InitNeighborSituation(frameSituation, cframe, SYSSTR_SLOTNAME_CELL_BOTTOM);
        cframe = FindCell(x + 1, y);
        InitNeighborSituation(frameSituation, cframe, SYSSTR_SLOTNAME_CELL_RIGTH);
        cframe = FindCell(x - 1, y);
        InitNeighborSituation(frameSituation, cframe, SYSSTR_SLOTNAME_CELL_LEFT);
    }

    retn = BindFrame(frameSituation);
    AddMsgToLog(GetSpaces(m_Padding) + "'" + frame->frameName().toUpper() + "' - конец вывода");
    AddMsgToLog("");

    return retn;
}

// Жесткая рекурсия, надо тестить
bool MLV::BindFrame(NFrame *frame)
{
    qDebug()<<"bool MLV::BindFrame(NFrame *frame)";
    bool retn = true;
    if (!frame)
        return false;

    qDebug()<<frame->frameName();

    NFrame* frameInst = NULL;
    if (frame->frameType() == FrameType::prototype)
    {
        // Создаем экземпляр
        frameInst = CreateFrameInstance(frame->frameName(), false);
        if (!frameInst)
        {
            return false;
        }
    }
    else
    {
        frameInst = frame;

        // Если фрейм есть в рабочей памяти, то считаем, что он уже привязан
        if (FindByPtr(frame) != NULL)
        {
            qDebug()<<"Frame in a work memory";
            return true;
        }
    }

    m_Padding += 1;
    AddMsgToLog(GetSpaces(m_Padding) + "Пытаемся привязать фрейм '" + frameInst->frameName().toUpper() + "'");

    // Пытаемся привязать слоты
    QList<NSlot*> nslots = m_KBManager->GetFrameSlots(frameInst);

    qDebug()<<"Slots count: "<<nslots.count();

    for (int i = 0; i < nslots.count(); i++)
    {
        if (nslots[i]->isSystem())
            continue;

        retn &= BindSlot(frameInst, nslots[i]);
        if (!retn)
            break; // если хоть один слот не привязался, завершаем перебор
    }

    QString str = retn? "' привязался" : "' НЕ привязался";
    AddMsgToLog(GetSpaces(m_Padding) + "Фрейм '" + frameInst->frameName().toUpper() + str);


    if (retn)
    {
        m_WorkMemory.append(frameInst);

        // Пытаемся привязать потомков
        QList<NFrame*> children = m_KBManager->GetFrameChildren(frameInst);
        for (int i = 0; i < children.count(); i++)
        {
            NFrame* frameInst1 = CreateFrameInstance(children[i]->frameName(), false);
            if (frameInst1)
            {
                SetSubframe(frameInst1, SYSSTR_SLOTNAME_ISA, frameInst);
                bool end = BindFrame(frameInst1);
                if (end && !m_FullSearch)
                    break;
            }
        }
    }

    m_Padding -= 1;

    return retn;
}

bool MLV::BindSlot(NFrame* frame, NSlot *slot)
{
    qDebug()<<"bool MLV::BindSlot(NFrame* frame, NSlot *slot)";
    bool retn = false;
    if (!slot || !frame)
        return false;

    qDebug()<<slot->name();
    // Если субфрейм
    if (slot->getSlotType() == SYSSTR_SLOTTYPE_FRAME)
    {
        NFrame* subframe = GetSubframe(frame, slot->name());
        if (!subframe)
        {
            // Создаем экземпляр
            QString subframeName = slot->defValue().toString();
            subframe = CreateFrameInstance(subframeName);
            if (!subframe)
                return false;
            SetSubframe(frame, slot->name(), subframe);
        }

        // Вызваем привязку экземпляра
        return BindFrame(subframe);
    }

    m_Padding += 1;
    AddMsgToLog(GetSpaces(m_Padding) + "Пытаемся привязать слот '" + slot->name().toUpper() + "'");

    // Если тип слота - домен, строка или число
    QString markerType  = slot->getSlotMarkerType();
    QString markerVal   = slot->getSlotMarker();

    if (!markerVal.isEmpty())
    {
        if (markerType == SYSSTR_SLOTTYPE_PRODUCTION)
        {
            NProduction* production = m_KBManager->getProduction(slot->getSlotMarker());
            NProductionMLV* productionMLV = new NProductionMLV(this, frame->id(), production);
            QString val = productionMLV->StartConsultation(slot->name());
            SetSlotValue(frame, slot->name(), val);

            retn = !val.isEmpty();
        }
    }

    // Сравниваем с заданием отсутствия
    QVariant slotValue = GetSlotValue(frame, slot->name());
    retn = slot->defValue() == slotValue;

    qDebug()<<slot->defValue()<<" == "<<slotValue << "    " << retn;

    QString str = retn? "' привязался" :"' НЕ привязался";
    AddMsgToLog(GetSpaces(m_Padding) + "Cлот '" + slot->name().toUpper() + str);
    m_Padding -= 1;
    return retn;
}


bool  MLV::isGameContinues()
{
    return m_GameContinues;
}

void  MLV::DoAction(NFrame* frameSituation)
{
    NSlot *actionSlot = frameSituation->getSlotByName(SYSSTR_SLOTNAME_ACTION);
    if(!actionSlot)
        return;

    QString procName=actionSlot->getSlotMarker();
    NProc *proc = m_KBManager->getProc(procName);

    if(!proc)
        return;

    QString script = proc->proc();

    if(script=="")
        return;

    QScriptEngine *engine = new QScriptEngine();

    //окружение скрипта
    NFrame *cell = GetSubframe(frameSituation, SYSSTR_SLOTNAME_CELL_GAMER, true);
    QSProxyCell *proxyCell = new QSProxyCell(cell,engine);
    QScriptValue objectCell = engine->newQObject(proxyCell);
    engine->globalObject().setProperty("Cell", objectCell);

    //ячейка слева
    NFrame *cell_left = GetSubframe(frameSituation, SYSSTR_SLOTNAME_CELL_LEFT, true);
    if(cell_left)
    {
        QSProxyCell *proxyCellLeft = new QSProxyCell(cell_left,engine);
        QScriptValue objectCellLeft = engine->newQObject(proxyCellLeft);
        engine->globalObject().setProperty("LeftCell", objectCellLeft);
    }

    //ячейка справа
    NFrame *cell_right = GetSubframe(frameSituation, SYSSTR_SLOTNAME_CELL_RIGTH, true);
    if(cell_right)
    {
        QSProxyCell *proxyCellRight = new QSProxyCell(cell_right,engine);
        QScriptValue objectCellRight = engine->newQObject(proxyCellRight);
        engine->globalObject().setProperty("RightCell", objectCellRight);
    }

    //ячейка сверху
    NFrame *cell_top = GetSubframe(frameSituation, SYSSTR_SLOTNAME_CELL_TOP, true);
    if(cell_top)
    {
        QSProxyCell *proxyCellTop = new QSProxyCell(cell_top,engine);
        QScriptValue objectCellTop = engine->newQObject(proxyCellTop);
        engine->globalObject().setProperty("TopCell", objectCellTop);
    }

    //ячейка снизу
    NFrame *cell_bottom = GetSubframe(frameSituation, SYSSTR_SLOTNAME_CELL_BOTTOM, true);
    if(cell_bottom)
    {
        QSProxyCell *proxyCellBottom = new QSProxyCell(cell_bottom,engine);
        QScriptValue objectCellBottom = engine->newQObject(proxyCellBottom);
        engine->globalObject().setProperty("BottomCell", objectCellBottom);
    }

    //запускаем скрипт
    QScriptValue result = engine->evaluate(script);
    int res = result.toInt32();
    if (result.isError());
        qDebug() << "Script error:" << result.toString();

}
