#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QtGui/QColor>
#include <QtGui/QGraphicsItem>

class GameItem : public QGraphicsItem
{
public:
    GameItem(const QColor &color, int x, int y);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    int x, y;
    QColor color;
    QList<QPointF> stuff;
};

#endif // GAMEFIELD_H
