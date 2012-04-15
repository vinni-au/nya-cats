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
    setAcceptsHoverEvents(true);
}

QRectF GridCell::boundingRect() const
{
    return m_Rect;
}

QPainterPath GridCell::shape() const
{
    QPainterPath path;
    path.addRect(m_Rect);
    return path;
}

void GridCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor fillColor = (option->state & QStyle::State_MouseOver) ? m_Color.dark(255) : m_Color;
    QBrush b = painter->brush();
    QPen p = painter->pen();
    painter->setBrush(fillColor);
    painter->setPen(m_Color);
    painter->drawRect(m_Rect);
    painter->setBrush(b);
    painter->setPen(p);

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
