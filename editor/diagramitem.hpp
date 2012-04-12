#ifndef DIAGRAMITEM_HPP
#define DIAGRAMITEM_HPP

#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

#include "arrow.hpp"

class DiagramItem : public QGraphicsPolygonItem
{
    //TODO позиционирование надписи на ноде
public:
    enum { Type = UserType + 1 };
    enum DiagramType { Node };

    DiagramItem(unsigned id, DiagramType type = Node, QString title = QString(), QMenu* contextMenu = 0,
                QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);

    void removeArrow(Arrow *arrow);
    void removeArrows();

    DiagramType diagramType() const
    {   return m_type;  }

    QPolygonF polygon() const
    {   return m_polygon;   }

    void addArrow(Arrow *arrow);

    QPixmap image() const;

    int type() const
    {   return Type;    }

    unsigned id() const
    {   return m_id;    }

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    DiagramType m_type;
    QPolygonF m_polygon;
    QMenu *m_contextMenu;
    QList<Arrow *> m_arrows;
    QString m_title;
    unsigned m_id;
};

#endif // DIAGRAMITEM_HPP
