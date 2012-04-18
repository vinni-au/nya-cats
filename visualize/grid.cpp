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
