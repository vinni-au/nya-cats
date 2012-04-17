#ifndef GAMEITEMFRAME_H
#define GAMEITEMFRAME_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include "gameitem.h"

class GameItemFrame : QGraphicsPixmapItem
{
protected:
    QRectF m_Rect;
    QColor m_Color;
    GameItem *m_GameItem;

public:
    GameItemFrame(GameItem* item, QRectF rect, QColor &color, QGraphicsItem *parent = 0);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

    void SetItem(GameItem *item);
    GameItem* GetItem();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // GAMEITEMFRAME_H
