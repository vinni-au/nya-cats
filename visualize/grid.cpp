#include "grid.h"
#include <QtGui>


Grid::Grid(int count, QRectF &rect, QColor &color, QGraphicsScene *scene) :
     m_Count (count)
    ,m_Rect  (rect)
    ,m_Color (color)
    ,m_Scene (scene)

{
    float xSize = m_Rect.width() / m_Count;
    float ySize = m_Rect.height() / m_Count;

    for (int i = 0; i < m_Count; i++)
    {
        for (int j = 0; j < m_Count; j++)
        {
            QRectF rect(i * xSize, j * ySize, xSize, ySize);
            Cell *cell = new Cell(i, j, rect, m_Color);

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
    return m_Count * m_Count;
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
