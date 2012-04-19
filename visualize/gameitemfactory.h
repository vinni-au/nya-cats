#ifndef GAMEITEMFACTORY_H
#define GAMEITEMFACTORY_H

#include <QtGui>
#include "gameitem.h"

class GameItemFactory;

class FactoryFrame
{
protected:
    QList<GameItemFactory*>     m_FactoryList;
    QGraphicsScene*             m_Scene;

public:
    FactoryFrame(QGraphicsScene* scene);
    void AddItem(GameItem* item, QRectF& rect);

};

class GameItemFactory : public QGraphicsPixmapItem
{
protected:
   GameItem* m_Item;
   QRectF    m_Rect;

   QList<QPointF> stuff;

public:
    GameItemFactory(GameItem* item, QRectF& rect);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // GAMEITEMFACTORY_H
