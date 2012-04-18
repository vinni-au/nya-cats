#ifndef GAMEGRID_H
#define GAMEGRID_H

#include <QtGui>
#include "gameitembase.h"

class Cell;
class Grid;

// Cell
class Cell : public QGraphicsItem, public GameItemBase
{
protected:
    int m_xIndex;
    int m_yIndex;

public:
    Cell(int xIndex, int yIndex, QRectF &rect, QColor &color);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

// Grid
class Grid : public GameItemBase
{
protected:
    int m_Count;

    QList<Cell*> m_Cells;
    QGraphicsScene* m_Scene;

public:
    Grid(int count, QRectF &rect, QColor &color, QGraphicsScene *scene);
    void Init();
};

#endif // GAMEGRID_H
