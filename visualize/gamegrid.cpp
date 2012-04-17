#include "gamegrid.h"


GameGrid::GameGrid(int xCellCount, int yCellCount, int xCellSize, int yCellSize, QObject *parent) :
    QGraphicsScene(parent),
    m_iXCellCount(xCellCount),
    m_iYCellCount(yCellCount),
    m_iXCellSize(xCellSize),
    m_iYCellSize(yCellSize),
    m_bIsGridVisible(true)
{
}

void GameGrid::Init()
{
    // Линия ограничения игрового поля
    this->addRect(0, 0, m_iXCellCount * m_iXCellSize, m_iYCellCount * m_iXCellSize);

    // Добавляем ячейки игрового поля
    for (int i = 0; i < m_iXCellCount; i++)
        for (int j = 0; j < m_iYCellCount; j++)
        {
            QRectF rect(i * m_iXCellSize, j * m_iYCellSize, m_iXCellSize, m_iYCellSize);
            QColor color(200, 200, 200, 100);
            GridCell *cell = new GridCell(i, j, rect, color);
            m_Cells.append(cell);
            this->addItem(cell);
        }


}
