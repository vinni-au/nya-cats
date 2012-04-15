#ifndef GRIDCELL_H
#define GRIDCELL_H

#include <QGraphicsPixmapItem>

class GridCell : public QGraphicsPixmapItem
{
protected:
    int m_iXIndex;
    int m_iYIndex;
    QRectF m_Rect;
    QColor m_Color;

public:
    GridCell(int x, int y, QRectF rect, QColor &color, QGraphicsItem *parent = 0);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // GRIDCELL_H
