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

    void removeArrowTo(DiagramItem* item);
    void removeArrowFrom(DiagramItem* item);

    DiagramType diagramType() const
    {   return m_type;  }

    QPolygonF polygon() const
    {   return m_polygon;   }

    void addArrow(Arrow *arrow);

    QList<Arrow*> arrows()
    {   return m_arrows;    }

    QPixmap image() const;

    int type() const
    {   return Type;    }

    unsigned id() const
    {   return m_id;    }

    QString title() const
    {   return m_title; }
    void setTitle(QString title)
    {   m_title = title;    }

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
    void imitateMousePress();

private:
    DiagramType m_type;
    QPolygonF m_polygon;
    QMenu *m_contextMenu;
    QList<Arrow *> m_arrows;
    QString m_title;
    unsigned m_id;
};

#endif // DIAGRAMITEM_HPP
