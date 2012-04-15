#include "gamegrid.h"


GameGrid::GameGrid(int xCount, int yCount, int xSize, int ySize, QObject *parent) :
    QGraphicsScene(parent),
    m_iXCount(xCount),
    m_iYCount(yCount),
    m_iXSize(xSize),
    m_iYSize(ySize),
    m_bIsGridVisible(true)
{
}

void GameGrid::Init()
{
    this->addRect(0, 0, m_iXCount * m_iXSize, m_iYCount * m_iXSize);

    for (int i = 0; i < m_iXCount; i++)
        for (int j = 0; j < m_iYCount; j++)
        {
            QRectF rect(i * m_iXSize, j * m_iYSize, m_iXSize, m_iYSize);
            QColor color(200, 200, 200, 100);
            GridCell *cell = new GridCell(i, j, rect, color);
            m_Cells.append(cell);
            this->addItem(cell);
        }
}
