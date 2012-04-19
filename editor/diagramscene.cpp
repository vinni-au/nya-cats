#include "diagramscene.hpp"

DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent) :
    QGraphicsScene(parent), m_itemMenu(itemMenu), m_line(0)
{
    m_itemColor = Qt::white;
    m_lineColor = Qt::black;
    m_textColor = Qt::black;
    m_mode = MoveItem;
    m_itemType = DiagramItem::Node;
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    DiagramItem* item;
    switch (m_mode) {
         case InsertLine:
             m_line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                         mouseEvent->scenePos()));
             m_line->setPen(QPen(m_lineColor, 2));
             addItem(m_line);
             break;
        default:
            ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_mode == InsertLine && m_line != 0) {
        QLineF newline(m_line->line().p1(), mouseEvent->scenePos());
        m_line->setLine(newline);
    } else if (m_mode == MoveItem)
        QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_line != 0 && m_mode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(m_line->line().p1());
        if (startItems.count() && startItems.first() == m_line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(m_line->line().p2());
        if (endItems.count() && endItems.first() == m_line)
            endItems.removeFirst();

        removeItem(m_line);
        delete m_line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == DiagramItem::Type &&
            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first()) {
            DiagramItem *startItem =
                qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem =
                qgraphicsitem_cast<DiagramItem *>(endItems.first());
            Arrow *arrow = new Arrow(startItem, endItem);
            arrow->setColor(m_lineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
            emit arrowAdded(arrow);
        }
    }
    m_line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
