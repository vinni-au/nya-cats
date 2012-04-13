#include "grid.h"
#include <QtGui>

Grid::Grid(int xCount, int yCount, int xSize, int ySize) :
    m_iXCount(xCount),
    m_iYCount(yCount),
    m_iXSize(xSize),
    m_iYSize(ySize)
{
}

QRectF Grid::boundingRect() const
{
    return QRectF(0, 0, m_iXCount * m_iXSize, m_iYCount * m_iYSize);
}

void Grid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    for (int i = 0; i <= m_iXCount; i++)
    {
        int x = i * m_iXSize;
        painter->drawLine(x, 0, x, m_iYCount * m_iYSize);
    }

    for (int j = 0; j <= m_iYCount; j++)
    {
        int y = j * m_iYSize;
        painter->drawLine(0, y, m_iXCount * m_iXSize, y);
    }
}
