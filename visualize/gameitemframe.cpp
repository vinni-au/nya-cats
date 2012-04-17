#include "gameitemframe.h"

GameItemFrame::GameItemFrame(GameItem* item, QRectF rect, QColor &color, QGraphicsItem *parent = 0) :
    QGraphicsPixmapItem(parent),
    m_Rect(rect),
    m_Color(color)
{
    SetItem(item);
}

QRectF GameItemFrame::boundingRect() const
{
    return m_Rect;
}

QPainterPath GameItemFrame::shape() const
{
    QPainterPath path;
    path.addRect(m_Rect);
    return path;
}

void GameItemFrame::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    QColor fillColor = (option->state & QStyle::State_MouseOver) ? m_Color.dark(255) : m_Color;
    QBrush b = painter->brush();
    QPen p = painter->pen();

    painter->setBrush(fillColor);
    painter->setPen(m_Color);
    painter->drawRect(m_Rect);
    painter->setBrush(b);
    painter->setPen(p);

    QGraphicsPixmapItem::paint(painter, item, widget);
}

void GameItemFrame::SetItem(GameItem *item)
{
    if (!item)
        return;

    m_GameItem = item;
    this->setPixmap(item->GetPicture());
}

GameItem* GameItemFrame::GetItem()
{
    return m_GameItem;
}

void GameItemFrame::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsPixmapItem::hoverEnterEvent(event);
}

void GameItemFrame::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsPixmapItem::hoverLeaveEvent(event);
}

