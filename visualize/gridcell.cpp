#include "gridcell.h"
#include <QtGui>

// GridCell
GridCell::GridCell(int x, int y, QRectF rect, QColor &color, QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent),
    m_iXIndex(x),
    m_iYIndex(y),
    m_Rect(rect),
    m_Color(color)
{
}

QRectF GridCell::boundingRect() const
{
    return m_Rect;
}

void GridCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //QColor fillColor = (option->state & QStyle::State_MouseOver) ? m_Color.dark(255) : m_Color;
    //QBrush b = painter->brush();
    //painter->setBrush(fillColor);
    painter->drawRect(m_Rect);
    //painter->setBrush(b);

    QGraphicsPixmapItem::paint(painter, option, widget);
}

void GridCell::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsPixmapItem::hoverEnterEvent(event);
}

void GridCell::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsPixmapItem::hoverLeaveEvent(event);
}
