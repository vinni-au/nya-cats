#include "diagramscene.hpp"

DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent) :
    QGraphicsScene(parent), m_itemMenu(itemMenu)
{
    m_itemColor = Qt::white;
    m_lineColor = Qt::black;
    m_textColor = Qt::black;
    m_mode = MoveItem;
    m_itemType = DiagramItem::Node;
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

}
