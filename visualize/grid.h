#ifndef GRID_H
#define GRID_H

#include <QGraphicsItem>

class Grid : public QGraphicsItem
{
public:
    Grid(int xCount, int yCount, int xSize, int ySize);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

protected:
    int m_iXCount, m_iYCount;
    int m_iXSize,  m_iYSize;
};

#endif // GRID_H
