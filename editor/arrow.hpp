#ifndef ARROW_HPP
#define ARROW_HPP

#include <QGraphicsLineItem>
#include <QPainter>

class DiagramItem;

class Arrow : public QGraphicsLineItem
{
    //TODO надпись на стрелке
    DiagramItem *m_startItem;
    DiagramItem *m_endItem;
    QColor m_color;
    QPolygonF m_arrowHead;
    QString m_text;

public:
    enum { Type = UserType + 2 };

    Arrow(DiagramItem *startItem, DiagramItem *endItem, QString text = QString(),
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const
    {   return Type;    }

    void setText(QString text)
    {   m_text = text;  }
    QString text() const
    {   return m_text;  }

    QRectF boundingRect() const;

    QPainterPath shape() const;

    void setColor(const QColor &color)
    {   m_color = color;    }

    DiagramItem *startItem() const
    {   return m_startItem; }
    DiagramItem *endItem() const
    {   return m_endItem;   }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

};

#include "diagramitem.hpp"

#endif // ARROW_HPP
