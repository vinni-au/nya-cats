#include "grid.h"
#include <QtGui>


Grid::Grid(int count, QRectF &rect, QColor &color, QGraphicsScene *scene) :
     m_Count (count)
    ,m_Rect  (rect)
    ,m_Color (color)
    ,m_Scene (scene)
    ,m_contextMenuCell(0)
{
    QMenu* menu = new QMenu;
    menu->addAction("Вывести ситуацию", this, SLOT(findOutSituation()));

    float xSize = m_Rect.width() / m_Count;
    float ySize = m_Rect.height() / m_Count;

    for (int i = 0; i < m_Count; i++)
    {
        for (int j = 0; j < m_Count; j++)
        {
            QRectF rect(i * xSize, j * ySize, xSize, ySize);
            Cell *cell = new Cell(i, j, rect, m_Color, menu);

            QObject::connect(cell, SIGNAL(contextMenuExecutedOn(Cell*)),
                             SLOT(setContextMenuCell(Cell*)));
			QObject::connect(cell, SIGNAL(sigDoVisualizerCell(Cell*)),
				SLOT(slotDoVisualizerCell(Cell*)));
			QObject::connect(cell, SIGNAL(sigGetInfo(int, int)),
				SLOT(slotGetInfo(int, int)));

            m_Cells.append(cell);
            m_Scene->addItem(cell);
        }
    }
}

QList<Cell*> Grid::GetCells()
{
    return m_Cells;
}

Cell* Grid::GetCell(int x, int y)
{
    for (int i = 0; i < m_Count * m_Count; i++)
    {
        if (m_Cells[i]->GetX() == x && m_Cells[i]->GetY() == y)
            return m_Cells[i];
    }
    return NULL;
}

Cell* Grid::GetCell(int index)
{
    if (m_Cells.count() >= index)
        return m_Cells[index];
    else
        return NULL;
}

int Grid::GetSideCount()
{
    return m_Count;
}

int Grid::GetCount()
{
    return m_Count;
}

GameItem* Grid::GetGameItem(int x, int y)
{
    if (Cell* cell = GetCell(x, y))
        return cell->GetGameItem();
    else
        return NULL;
}

GameItem* Grid::GetGameItem(int index)
{
    if (Cell* cell = GetCell(index))
        return cell->GetGameItem();
    else
        return NULL;

}

void Grid::MoveGameItemTo(int fromX, int fromY, int toX, int toY)
{
	Cell* fromCell = GetCell(fromX, fromY);
	GameItem* fromItem = fromCell->GetGameItem();
	if (fromItem == NULL) 
		return;

	Cell* toCell = GetCell(toX, toY);
        if(!toCell)
            return;
	GameItem* toItem = toCell->GetGameItem();
	//if (toItem != NULL) // Не можем перемещать итем в уже занятую клетку
	//	return;

	fromCell->SetGameItem(NULL);
	toCell->SetGameItem(fromItem);
}

Cell* Grid::FindCellByItemFrameId(int id)
{
	for (int i = 0; i < m_Cells.count(); i++)
	{
		Cell* cell =  m_Cells[i];
		GameItem* item = cell->GetGameItem();
		if (item == NULL)
			continue;

		if (item->GetFrameId() == id)
			return  cell;
	}
	return NULL;
}

void Grid::Clear()
{
	if (m_Cells.isEmpty())
		return;

	for (int i = 0; i < m_Cells.count(); i++)
	{
		Cell* cell = m_Cells[i];
		GameItem* item = cell->GetGameItem();
		cell->SetGameItem(NULL);
		delete item;
	}
}

void Grid::setContextMenuCell(Cell *cell)
{
    m_contextMenuCell = cell;
}

void Grid::findOutSituation()
{
    emit sigFindOutSituation(m_contextMenuCell->GetX(), m_contextMenuCell->GetY());
}

void Grid::slotDoVisualizerCell(Cell* cell)
{
	emit sigDoVisualizerCell(cell);
}

QString Grid::slotGetInfo(int x, int y)
{
	return emit sigGetInfo(x, y);
}

