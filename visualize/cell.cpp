#include "cell.h"

Cell::Cell(int xIndex, int yIndex, QRectF &rect, QColor &color) :
     m_xIndex(xIndex)
    ,m_yIndex(yIndex)
    ,m_Rect(rect)
    ,m_Color(color)
{
    setAcceptsHoverEvents(true);
}

QRectF Cell::boundingRect() const
{
    return m_Rect;
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor fillColor =
            (option->state & QStyle::State_MouseOver) ? m_Color.dark(255) : m_Color;
    QBrush b = painter->brush();
    QPen p = painter->pen();

    painter->setBrush(fillColor);
    painter->setPen(m_Color);
    painter->drawRect(m_Rect);
    painter->setBrush(b);
    painter->setPen(p);
}

void Cell::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void Cell::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}
