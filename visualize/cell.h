#ifndef CELL_H
#define CELL_H

#include <QtGui>
#include "gameitem.h"

class Cell : public QGraphicsItem
{
protected:
    int         m_xIndex;
    int         m_yIndex;
    QRectF      m_Rect;
    QColor      m_Color;
    GameItem*   m_Item;

public:
    Cell(int xIndex, int yIndex, QRectF &rect, QColor &color);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    void SetGameItem(GameItem* item);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
};

#endif // CELL_H
