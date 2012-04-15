#ifndef DIAGRAMSCENE_HPP
#define DIAGRAMSCENE_HPP

#include <QMenu>
#include <QGraphicsScene>
#include "diagramitem.hpp"

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:    
    enum Mode { InsertItem, InsertLine, MoveItem};
    explicit DiagramScene(QMenu* itemMenu = 0, QObject *parent = 0);

    QColor textColor() const
    {   return m_textColor; }
    void setTextColor(QColor color)
    {   m_textColor = color;    }

    QColor itemColor() const
    {   return m_itemColor; }
    void setItemColor(QColor color)
    {   m_itemColor = color;    }

    QColor lineColor() const
    {   return m_lineColor; }
    void setLineColor(QColor color)
    {   m_lineColor = color;    }

    void setItemMenu(QMenu* menu)
    {   m_itemMenu = menu;  }

    DiagramItem::DiagramType itemType() const
    {   return m_itemType;  }
    void setItemType(DiagramItem::DiagramType type)
    {   m_itemType = type;  }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    QColor m_textColor;
    QColor m_itemColor;
    QColor m_lineColor;

    QMenu* m_itemMenu;

    Mode m_mode;

    DiagramItem::DiagramType m_itemType;

    QGraphicsLineItem* m_line;

signals:

public slots:

};


#endif // DIAGRAMSCENE_HPP
