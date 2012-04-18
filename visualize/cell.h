#ifndef CELL_H
#define CELL_H

#include <QtGui>

class Cell : public QGraphicsItem
{
protected:
    int         m_xIndex;
    int         m_yIndex;
    QRectF      m_Rect;
    QColor      m_Color;

public:
    Cell(int xIndex, int yIndex, QRectF &rect, QColor &color);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // CELL_H
