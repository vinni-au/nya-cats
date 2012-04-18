#ifndef ITEMSFRAME_H
#define ITEMSFRAME_H

#include <QtGui>
#include "gameitembase.h"
#include "gameitem.h"
#include "grid.h"

class ItemFactory;
class ItemsFrame;

// ItemTemplate
class ItemTemplate : public Cell
{
protected:
    GameItem* m_Item;

public:
    ItemTemplate(GameItem* item, QRectF &rect, QColor &color);
};

// ItemTemplatesFrames
class ItemTemplatesFrame : public QGraphicsItem, public GameItemBase
{
private:
    QList<QPointF> stuff;
    QList<ItemTemplate*> m_ItemTemplates;
    QGraphicsScene* m_Scene;

public:
    ItemTemplatesFrame(QRectF &rect, QColor &color, QGraphicsScene* scene);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

    void AddItemTemplate(GameItem* item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // ITEMSFRAME_H
