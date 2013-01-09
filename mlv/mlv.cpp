#include "mlv.h"
#include <QList>

MLV::MLV(NKBManager* manager, Visualizer* viz) :
    QObject(0)
  ,m_KBManager(manager)
  ,m_Initialized(false)
  ,m_InstCount(0)
  ,m_Padding(0)
  ,m_FullSearch(false)
  ,m_GameContinues(false)
  ,m_RandomBind(true)
  ,m_Viz(viz)
{
    qsrand ( QDateTime::currentDateTime().time().msec() );
	m_Grid = m_Viz->GetGrid();
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

    NFrame* frame = GetGameInst(cell);
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

NFrame* MLV::FindCellByItemInst(NFrame* itemInst)
{
	if (itemInst == NULL)
		return NULL;
	for (int i = 0; i < m_CellFrameInsts.count(); i++)
	{
		NFrame* currInst = GetSubframe(m_CellFrameInsts[i], SYSSTR_SLOTNAME_GAMEITEM, true);
		if (currInst == NULL) continue;
		if (currInst->id() == itemInst->id())
		{
			return m_CellFrameInsts[i];
		}
	}
	return NULL;
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

    QList<NSlot*> slotsList = m_KBManager->GetFrameSlots(frame);
    for (int i = 0; i < slotsList.count(); i++)
    {
        QVariant val = GetSlotValue(frame, slotsList[i]->name());
        if (val.toString().isEmpty() && (fillDefault || slotsList[i]->isSystem()))
        {
            SetSlotValueVariant(frame, slotsList[i]->name(), slotsList[i]->defValue());
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
            if (val.toString().isEmpty() && (fillDefault || slotsList[i]->isSystem()))
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
    if (frame == NULL || slotName.isNull())
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

void MLV::SetSubframe(NFrame* frame, QString slotName, NFrame* subframe, bool findInParents)
{
	 SetSlotValueVariant(frame, slotName, QVariant(reinterpret_cast<long long>(subframe)), findInParents);
}

// TODO
void MLV::AddSubframe(NFrame* frame, QString slotName, NFrame* subframe, bool findInParents)
{
	// Создаем слот субфрейма
	NSlot* newSlot = new NSlot();
	newSlot->setName(slotName);
	frame->addSlot(newSlot);

	SetSlotValueVariant(frame, slotName, QVariant(reinterpret_cast<long long>(subframe)), findInParents);
}

QList<NFrame*> MLV::getFrameLeaf(NFrame* root)
{
	return m_KBManager->getFrameLeaf(root);
}

// Возвращает список всех экземпляров фреймов ситуаций
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

// Возвращает список всех экземпляров фреймов ситуаций для данной ячейки
QList<NFrame*> MLV::getSituationInstanceList(NFrame* cell)
{
	QList<NFrame*> list;
	for (int i = 0; i < m_WorkMemory.count(); i++)
	{
		NFrame* frame = m_WorkMemory[i];
		if (m_KBManager->HasParentWithName(frame, SYSSTR_FRAMENAME_SITUATION) && !m_KBManager->hasChildren(frame))
		{
			NFrame* gamerCell = GetSubframe(frame, SYSSTR_SLOTNAME_CELL_GAMER, true);
			if (gamerCell != NULL && gamerCell->id() == cell->id())
				list.append(frame);
		}
	}
	return list;
}

// Возвращает список всех экземпляров фреймов изображений для данного игрового объекта
QList<NFrame*> MLV::getImageInstanceList(NFrame* item)
{
	QList<NFrame*> list;
	for (int i = 0; i < m_WorkMemory.count(); i++)
	{
		NFrame* frame = m_WorkMemory[i];
		if (m_KBManager->HasParentWithName(frame, SYSSTR_FRAMENAME_IMAGE) && !m_KBManager->hasChildren(frame))
		{
			NFrame* itemFrame = GetSubframe(frame, SYSSTR_FRAMENAME_GAMEITEM, true);
			if (itemFrame != NULL && itemFrame->id() == item->id())
				list.append(frame);
		}
	}
	return list;
}

bool MLV::IsContainGameItem(NFrame* cell)
{
	if (!cell) return false;
	NFrame* item = GetSubframe(cell, SYSSTR_FRAMENAME_GAMEITEM);
	if (item == NULL) return false;
	return m_KBManager->HasParentWithName(item, SYSSTR_FRAMENAME_GAMEITEM);
}

NFrame* MLV::GetGameInst(NFrame* cell)
{
	if (!cell) return NULL;
	// Действительно ли это ячейка игрового поля
	if (!m_KBManager->HasParentWithName(cell, SYSSTR_FRAMENAME_GAMECELL)) return NULL;
	NFrame* item = GetSubframe(cell, SYSSTR_FRAMENAME_GAMEITEM);
	if (item == NULL) return NULL;
	if (!m_KBManager->HasParentWithName(item, SYSSTR_FRAMENAME_GAMEITEM)) return NULL;
	return item;
}

void MLV::InitNeighborSituation(NFrame* frameSituation, NFrame* cell, QString Name)
{
	if (IsContainGameItem(cell))
		SetSubframe(frameSituation, Name, cell, true);
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

bool  MLV::isGameContinues()
{
	return m_GameContinues;
}

//////////////////////////////////////////////////////////////////////////
// РАБОТА С ПРОДУКЦИЯМИ
QVariant MLV::getValSlot(int frameId, QString aimVar)
{
	return getVal(frameId, aimVar);
}

bool MLV::setValSlot(int frameId, QString aimVar, QVariant value)
{
	return setVal(frameId, aimVar, value);
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

//////////////////////////////////////////////////////////////////////////
// РАБОТА С ВИЗАУЛИЗАТОРОМ
void MLV::UpdateGrid()
{
	if (m_Grid == NULL) return;
	
	for(int i = 0; i < m_CellFrameInsts.count(); i++)
	{
		NFrame* cellInst = m_CellFrameInsts[i];
		NFrame* itemInst = GetGameInst(cellInst);
		if (itemInst == NULL) continue;

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

void MLV::UpdateCell(NFrame* cellInst, NFrame* imageFrame)
{
	if (cellInst == NULL) return;
	if (!m_KBManager->HasParentWithName(imageFrame, SYSSTR_FRAMENAME_IMAGE)) return;
	if (m_Grid == NULL) return;

	NFrame* itemInst = GetGameInst(cellInst);
	if (itemInst == NULL) return;

	// Обновляем положение игрового объекта на игровом поле
	int newX = GetSlotValue(cellInst, SYSSTR_SLOTNAME_X, true).toInt();
	int newY = GetSlotValue(cellInst, SYSSTR_SLOTNAME_Y, true).toInt();

	QString type = GetSlotValue(itemInst, SYSSTR_SLOTNAME_NAME, true).toString();
	QString team = GetSlotValue(itemInst, SYSSTR_SLOTNAME_TEAM, true).toString();

	Cell* cell = m_Grid->FindCellByItemFrameId(itemInst->id());
	if (cell == NULL) // Это значит что на поле нет такого игрового итема
	{
		cell = m_Grid->GetCell(newX, newY);
		if (cell == NULL) return;
		QColor& col = m_Viz->GetColorByTeam(team);
		cell->SetGameItem(new GameItem(type, QPixmap(), team, col, itemInst->id()));
	}

	// Обновляем картинку, фон
	GameItem* gameItem = cell->GetGameItem();
	if (gameItem == NULL) return;

	QVariant bright = GetSlotValue(imageFrame, SYSSTR_SLOTNAME_BRIGTNESS, true);
	if (!bright.isNull())
		gameItem->UpdateBrigth(bright.toInt());

	QVariant pic = GetSlotValue(imageFrame, SYSSTR_SLOTNAME_PIC, true);
	if (!pic.isNull())
		gameItem->UpdatePic(pic.toString());

	int oldX = cell->GetX();
	int oldY = cell->GetY();

	if (oldX != newX || oldY != newY)
	{
		m_Grid->MoveGameItemTo(oldX, oldY, newX, newY);
	}
}

////////////////////////////////////////////////////////////////////////
// ВЫВОД
bool MLV::Init()
{
    m_CellFrameInsts.clear();
	m_ItemFrameInsts.clear();
    m_KBManager->clearExemplarIds();
	ClearWorkMem();
	m_Cache.clear();
	m_Initialized = false;

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
				ItemInst = FindInCache(item->GetFrameId());
				if (ItemInst == NULL)
				{
					// Создаем персонажа
					ItemInst = CreateGameItemInst(item);
				}
            }

            SetSubframe(CellInst, SYSSTR_FRAMENAME_GAMEITEM, ItemInst);
            m_CellFrameInsts.append(CellInst);
        }
    }
    m_Initialized = true;
    return true;
}

void MLV::ClearWorkMem()
{
	// Для каждого нового шага чистим рабочую память.
	m_WorkMemory.clear();
	ClearLog();
	LOR_Clear();

	// Загоняем в рабочую память экземпляр игрового поля, экземпляры ячеек и игровых объектов
	m_WorkMemory.append(m_GameFieldInst);
	for (int i = 0; i < m_CellFrameInsts.count(); i++)
		m_WorkMemory.append(m_CellFrameInsts[i]);

	for (int i = 0; i < m_ItemFrameInsts.count(); i++)
		m_WorkMemory.append(m_ItemFrameInsts[i]);
}

void MLV::Step()
{
    if (!m_Initialized || !m_GameContinues) return;
	ClearWorkMem();

    // Пробегаем по всем ячейкам и обрабатывем игровые объекты в них
    for (int i = 0; i < m_CellFrameInsts.count(); i++)
    {
        DoCell(m_CellFrameInsts[i]);
    }
}

void MLV::Step(int x, int y)
{
    if (!m_Initialized || !m_GameContinues) return;
	ClearWorkMem();
    DoCell(x, y);
}

void MLV::Start(int x, int y)
{
	if (m_GameContinues) return;
    if (!Init()) return;
    m_GameContinues = true;
    Step(x, y);
}

void MLV::Start()
{
	if (m_GameContinues) return;
    if (!Init()) return;
    m_GameContinues = true;
    Step();
}

void MLV::RandomStart()
{
	if (m_GameContinues) return;
	if (!Init()) return;
	m_GameContinues = true;

	// Тут случайным образом заполняем поле
	int field = m_Grid->GetCount() * m_Grid->GetCount() / 4;
	int count = qrand() % field;
	for (int i = 0; i < count; i++)
	{
		int x = qrand() % m_Grid->GetCount();
		int y = qrand() % m_Grid->GetCount();
		NFrame* itemInst = FindByCell(x, y);
		
		// Если ячейка свободна
		if (itemInst == NULL || itemInst->frameName() == SYSSTR_FRAMENAME_EMPTY)
		{
			NFrame* cellInst = FindCell(x, y);
			NFrame* newItemInst = CreateRandGameItemInst();
			SetSubframe(cellInst, SYSSTR_SLOTNAME_GAMEITEM, newItemInst, true);
		}
	}
	
	Step();
}

void MLV::Stop()
{
    m_GameContinues = false;
}

NFrame* MLV::CreateGameItemInst(GameItem* gameItem)
{
	if (gameItem == NULL)
		return NULL;

	// Создаем персонажа
	NFrame* ItemInst = CreateFrameInstanceFull(gameItem->GetType());
	// Сохраняем в итем игрового объекта ид экземпляра соотв. ему фрейма
	gameItem->SetFrameId(ItemInst->id());

	// Заполняем значения команды
	SetSlotValue(ItemInst, SYSSTR_SLOTNAME_TEAM, gameItem->GetTeam());
	m_ItemFrameInsts.append(ItemInst);
	m_WorkMemory.append(ItemInst);
	return ItemInst;
}

NFrame* MLV::CreateRandGameItemInst()
{
	// Выбираем случайный игровой объект
	NFrame* gameItem = m_KBManager->GetFrameByName(SYSSTR_FRAMENAME_GAMEITEM);
	QList<NFrame*> itemsList = m_KBManager->getFrameLeaf(gameItem);
	int randIndex = qrand() % itemsList.size();
	NFrame* randItem = itemsList[randIndex];

	// Создаем персонажа
	NFrame* ItemInst = CreateFrameInstanceFull(randItem->frameName());

	// Если персонаж, заполняем случайную команду и случайное здоровье
	if (m_KBManager->HasParentWithName(ItemInst, SYSSTR_FRAMENAME_PERSON))
	{
		QVector<QString> teamsList = m_KBManager->GetDomainValsByString(SYSSTR_DOMAINNAME_TEAMS);
		randIndex = qrand() % teamsList.size();
		QString randTeam = teamsList[randIndex];

		// Заполняем значение команды
		SetSlotValue(ItemInst, SYSSTR_SLOTNAME_TEAM, randTeam);

		// Заполняем значение здоровье
		SetSlotValue(ItemInst, SYSSTR_SLOTNAME_HEALTH, qrand() % 5);
	}
	m_ItemFrameInsts.append(ItemInst);
	m_WorkMemory.append(ItemInst);
	return ItemInst;
}

void MLV::MoveItemInst(NFrame* itemInst, NFrame* toCellInst)
{
	NFrame* fromCell = FindCellByItemInst(itemInst);
	if (fromCell != NULL)
	{
		NFrame* EmpryInst = CreateFrameInstanceFull(SYSSTR_FRAMENAME_EMPTY);
		SetSubframe(fromCell, SYSSTR_FRAMENAME_GAMEITEM, EmpryInst);	
	}
	SetSubframe(toCellInst, SYSSTR_FRAMENAME_GAMEITEM, itemInst);
}

void MLV::DoVisualizerCell(Cell* cell)
{
	// Если игра еще не начата, то нужно делать полную инициализацию
	if (!m_GameContinues) return;
	if (cell == NULL || cell->GetGameItem() == NULL)
		return;

	ClearWorkMem();

	NFrame* ItemInst = NULL;
	GameItem* gameItem = cell->GetGameItem();

	// Ищем экземпляр ячейки
	NFrame* CellInst = FindCell(cell->GetX(), cell->GetY());
	if (CellInst == NULL)
	{
		CellInst = CreateFrameInstanceFull(SYSSTR_FRAMENAME_GAMECELL);
		if (!CellInst) return;
		m_CellFrameInsts.append(CellInst);
		if (gameItem == NULL)
		{
			NFrame* EmpryInst = CreateFrameInstanceFull(SYSSTR_FRAMENAME_EMPTY);
			SetSubframe(CellInst, SYSSTR_FRAMENAME_GAMEITEM, EmpryInst);
			return;
		}
	}

	// Ищем экземпляр игрового объекта
	ItemInst = FindInCache(gameItem->GetFrameId());
	if (ItemInst == NULL)
	{
		ItemInst = CreateGameItemInst(gameItem);
		SetSubframe(CellInst, SYSSTR_FRAMENAME_GAMEITEM, ItemInst);
	}
	else
	{
		MoveItemInst(ItemInst, CellInst);
	}

	// Привязываем изображение
	NFrame* imageFrame = BindImage(CellInst);

	// Если изображение привязалось, обновляем поле визуализатора
	if (imageFrame != NULL)
	{
		UpdateCell(CellInst, imageFrame);
	}
}

// Обрабатываем ячейку игрового поля
void MLV::DoCell(int x, int y)
{
	NFrame* cell = FindCell(x ,y);
	DoCell(cell);
}

void MLV::DoCell(NFrame* cell)
{
	NFrame* frame = GetGameInst(cell);
	if (!frame) return;

	// Если в ячейке не игровой объект или пусто - ничего не делаем
	if (!m_KBManager->HasParentWithName(frame, SYSSTR_FRAMENAME_GAMEITEM) ||
		frame->name.defValue() == SYSSTR_FRAMENAME_EMPTY)
		return;

	// Привязываем ситуацию и выполняем действие только для персонажа
	if (m_KBManager->HasParentWithName(frame, SYSSTR_FRAMENAME_PERSON))
	{
		// Привязываем ситуацию
		BindPerson(cell);
		QList<NFrame*> situations = getSituationInstanceList(cell);
		// Выполняем действие для персонажа в зависимости от привязанной ситуации
		if (situations.size() > 0)
			DoAction(situations[0]);
		else
		{
			// Если ни одна ситуация не привязалась - нечего делать
			NFrame* nothingToDo = CreateFrameInstance(SYSSTR_FRAMENAME_NOTHING_TODO);
			FillSituationByCell(cell, nothingToDo);
			DoAction(nothingToDo);
		}
	}

	// Привязываем изображение для всех игровых объектов
	NFrame* imageFrame = BindImage(cell);

	// Если изображение привязалось, обновляем поле визуализатора
	if (imageFrame != NULL)
	{
		UpdateCell(cell, imageFrame);
	}
}

// Привязывем изображение игрового объекта, содержащегося в ячейке
NFrame* MLV::BindImage(NFrame* cell)
{
	NFrame* item = GetGameInst(cell);
	if (!item) return NULL;

	NFrame* frameImage = CreateFrameInstance(SYSSTR_FRAMENAME_IMAGE, false);
	SetSubframe(frameImage, SYSSTR_FRAMENAME_GAMEITEM, item);

	QString x = GetSlotValue(cell, SYSSTR_SLOTNAME_X).toString();
	QString y = GetSlotValue(cell, SYSSTR_SLOTNAME_Y).toString();

	LOR_AddRoot("Определяем изображение для '" + item->frameName().toUpper() + 
		" (" + x + "," + y + ")'");
	bool retn = BindFrame(frameImage);
	LOR_SetCurrState(retn);

	QList<NFrame*> images = getImageInstanceList(item);
	return images.size() > 0 ? images[0] : NULL;
}

// Привязываем ситуацию для персонажа
NFrame* MLV::BindPerson(int x, int y)
{
    if (!Init())
        return false;

    NFrame* frame = FindCell(x ,y);
    return BindPerson(frame);
}

NFrame* MLV::BindPerson(NFrame* cell)
{
    if (!cell) return NULL;

    NFrame* frame = GetGameInst(cell);
    if (!frame) return NULL;

    m_Padding = 0;
    AddMsgToLog(GetSpaces(m_Padding) + "Определяем ситуацию для '" + frame->frameName().toUpper() + "'");

    NFrame* frameSituation = CreateFrameInstance(SYSSTR_FRAMENAME_SITUATION, false);
	FillSituationByCell(cell, frameSituation);

	QString x = GetSlotValue(cell, SYSSTR_SLOTNAME_X).toString();
	QString y = GetSlotValue(cell, SYSSTR_SLOTNAME_Y).toString();

	LOR_AddRoot("Определяем ситуацию для '" + frame->frameName().toUpper() + 
		" (" + x + "," + y + ")'");
    bool retn = BindFrame(frameSituation);
	LOR_SetCurrState(retn);

    AddMsgToLog(GetSpaces(m_Padding) + "'" + frame->frameName().toUpper() + "' - конец вывода");
    AddMsgToLog("");

	if (retn) 
		return frameSituation;
    return NULL;
}

void MLV::FillSituationByCell(NFrame* cell, NFrame* situation)
{
	if (cell == NULL || situation == NULL)
		return;

	SetSubframe(situation, SYSSTR_SLOTNAME_CELL_GAMER, cell);

	int x = GetSlotValue(cell, SYSSTR_SLOTNAME_X).toInt();
	int y = GetSlotValue(cell, SYSSTR_SLOTNAME_Y).toInt();

	NFrame *cframe = 0;
	cframe = FindCell(x, y - 1);
	InitNeighborSituation(situation, cframe, SYSSTR_SLOTNAME_CELL_TOP);
	cframe = FindCell(x, y + 1);
	InitNeighborSituation(situation, cframe, SYSSTR_SLOTNAME_CELL_BOTTOM);
	cframe = FindCell(x + 1, y);
	InitNeighborSituation(situation, cframe, SYSSTR_SLOTNAME_CELL_RIGTH);
	cframe = FindCell(x - 1, y);
	InitNeighborSituation(situation, cframe, SYSSTR_SLOTNAME_CELL_LEFT);
}

void MLV::FillSubSituation(NFrame* mainSit, NFrame* subSit)
{
	if (mainSit == NULL || subSit == NULL)
		return;

	if (!m_KBManager->HasParentWithName(mainSit, SYSSTR_FRAMENAME_SITUATION) ||
		!m_KBManager->HasParentWithName(subSit, SYSSTR_FRAMENAME_SITUATION))
		return;

	NFrame* gamerCell = GetSubframe(mainSit, SYSSTR_SLOTNAME_CELL_GAMER, true);
	FillSituationByCell(gamerCell, subSit);
}

//////////////////////////////////////////////////////////////////////////
// ЯДРО МВЛ: ПРИВЯЗКА ФРЕЙМА, СЛОТА
bool MLV::BindFrame(NFrame *frame, bool fillDefault, bool bindChildren)
{
    bool retn = true;
    if (!frame)
        return false;

    NFrame* frameInst = NULL;
    if (frame->frameType() == FrameType::prototype)
    {
        // Создаем экземпляр
        frameInst = CreateFrameInstance(frame->frameName(), fillDefault);
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
			return true;
        }
    }

    m_Padding += 1;
    AddMsgToLog(GetSpaces(m_Padding) + "Пытаемся привязать фрейм '" + frameInst->frameName().toUpper() + "'");
	LOR_AddNextLevel("Фрейм: '" + frameInst->frameName().toUpper() + "'");

    // Пытаемся привязать слоты
    QList<NSlot*> nslots = m_KBManager->GetFrameSlots(frameInst);
    for (int i = 0; i < nslots.count(); i++)
    {
        if (nslots[i]->isSystem())
            continue;

        retn &= BindSlot(frameInst, nslots[i], fillDefault);
        if (!retn)
            break; // если хоть один слот не привязался, завершаем перебор
    }

    QString str = retn? "' привязался" : "' НЕ привязался";
    AddMsgToLog(GetSpaces(m_Padding) + "Фрейм '" + frameInst->frameName().toUpper() + str);


    if (retn && bindChildren)
    {
		if (!m_WorkMemory.contains(frameInst))
			m_WorkMemory.append(frameInst);

        // Пытаемся привязать потомков
        QList<NFrame*> children = m_KBManager->GetFrameChildren(frameInst);

		if (!m_RandomBind)
		{
			for (int i = 0; i < children.count(); i++)
			{
				NFrame* frameInst1 = CreateFrameInstance(children[i]->frameName(), fillDefault);
				if (frameInst1)
				{
					SetSubframe(frameInst1, SYSSTR_SLOTNAME_ISA, frameInst);
					retn = BindFrame(frameInst1, fillDefault);
					if (retn && !m_FullSearch)
						break;
				}
			}
		}
		else
		{
			while (children.size() > 0)
			{
				int count = children.size();
				int randIndex = qrand() % count;
				NFrame* child = children[randIndex];
				NFrame* frameInst1 = CreateFrameInstance(child->frameName(), fillDefault);
				if (frameInst1)
				{
					SetSubframe(frameInst1, SYSSTR_SLOTNAME_ISA, frameInst);
					retn = BindFrame(frameInst1, fillDefault);
					if (retn && !m_FullSearch)
						break;
				}
				children.removeAt(randIndex);
			}
		}
    }

    m_Padding -= 1;
	LOR_SetCurrState(retn);
	LOR_GotoPrevLevel();

    return retn;
}

bool MLV::BindSlot(NFrame* frame, NSlot *slot, bool fillDefault)
{
    bool retn = false;
    if (!slot || !frame)
        return false;

    // Если субфрейм
    if (slot->getSlotType() == SYSSTR_SLOTTYPE_FRAME)
    {
        NFrame* subframe = GetSubframe(frame, slot->name());
        if (!subframe)
        {
            // Создаем экземпляр
            QString subframeName = slot->defValue().toString();
            subframe = CreateFrameInstanceFull(subframeName, fillDefault);
            if (!subframe)
                return false;
			FillSubSituation(frame, subframe);
            SetSubframe(frame, slot->name(), subframe);
        }

        // Вызваем привязку экземпляра
        return BindFrame(subframe, fillDefault, false);
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

    QString str = retn? "' привязался" :"' НЕ привязался";
    AddMsgToLog(GetSpaces(m_Padding) + "Cлот '" + slot->name().toUpper() + str);
    m_Padding -= 1;

	QString lorstr = "Слот: " + slot->name().toUpper() + "=" + slotValue.toString() + 
		" (задание отсутствия: " + slot->defValue().toString() + ")";
	LOR_AddNextLevel(lorstr);
	LOR_SetCurrState(retn);
	LOR_GotoPrevLevel();

    return retn;
}

//////////////////////////////////////////////////////////////////////////
// РАБОТА С ПРОЦЕДУРАМИ
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

    QScriptValue system = engine->newQObject(this);
    engine->globalObject().setProperty("sys", system);

    //запускаем скрипт
    QScriptValue result = engine->evaluate(script);
    int res = result.toInt32();
    if (result.isError());
        qDebug() << "Script error:" << result.toString();

}

void MLV::ShowMsg(QString msg)
{
    QMessageBox::information(NULL,"",msg,QMessageBox::Ok);
}

void MLV::NowhereToGo(QScriptValue qsvCell)
{
	// Если некуда идти

    QSProxyCell* qspCell = qobject_cast<QSProxyCell*>( qsvCell.toQObject());
    if(!qspCell)
        return;

    NFrame* cell = qspCell->getFrame();
    if(!cell)
        return;

    NFrame* nowhereToGo = CreateFrameInstance(SYSSTR_FRAMENAME_NOWHERE_TOGO);
    FillSituationByCell(cell, nowhereToGo);
    DoAction(nowhereToGo);
}
