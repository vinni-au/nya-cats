#ifndef CELL_H
#define CELL_H

#include <QtGui>
#include "gameitem.h"

class Cell : public QObject, public QGraphicsItem
{
    Q_OBJECT
protected:
    int         m_xIndex;
    int         m_yIndex;
    QRectF      m_Rect;
    QColor      m_Color;
    GameItem*   m_Item;
    QMenu*      m_contextMenu;

public:
    Cell(int xIndex, int yIndex, QRectF &rect, QColor &color, QMenu* contextMenu = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

    int GetX();
    int GetY();

    GameItem* GetGameItem();
	void SetGameItem(GameItem* item);

    void setContextMenu(QMenu* contextMenu) { m_contextMenu = contextMenu; }

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);

signals:
    void contextMenuExecutedOn(Cell* cell);
	void sigDoVisualizerCell(Cell* cell);
};

#endif // CELL_H
