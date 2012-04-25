#include "diagramitem.hpp"
#include "diagramscene.hpp"

DiagramItem::DiagramItem(unsigned id, DiagramType type, QString title, QMenu *contextMenu,
                         QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsPolygonItem(parent, scene), m_type(type), m_contextMenu(contextMenu),
    m_title(title), m_id(id)
{
    switch (m_type) {
        case Node:
        default:
            m_polygon << QPointF(-50, -25) << QPointF(-50, 25)
                      << QPointF(50, 25) << QPointF(50, -25)
                      << QPointF(-50, -25);
            break;
    }
    setPolygon(m_polygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void DiagramItem::removeArrow(Arrow *arrow)
{
    int index = m_arrows.indexOf(arrow);

    if (index != -1)
        m_arrows.removeAt(index);
}

void DiagramItem::removeArrows()
{
    foreach (Arrow *arrow, m_arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void DiagramItem::removeArrowTo(DiagramItem *item)
{
    foreach (Arrow* arrow, m_arrows) {
        if (arrow->endItem() == item) {
            scene()->removeItem(arrow);
            m_arrows.removeAll(arrow);
            delete arrow;
        }
    }
}

void DiagramItem::removeArrowFrom(DiagramItem *item)
{
    foreach (Arrow* arrow, m_arrows) {
        if (arrow->startItem() == item) {
            //TODO жёсткий костыль, метод по сути не работает теперь!!!
            /*DiagramScene* s = static_cast<DiagramScene*>(scene());
            if (s) {
                s->removeItem(arrow);
                m_arrows.removeAll(arrow);
                delete arrow;
            }*/
        }
    }
}

void DiagramItem::addArrow(Arrow *arrow)
{
    m_arrows << arrow;
}

QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(m_polygon);

    return pixmap;
}

void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    if (m_contextMenu)
        m_contextMenu->exec(event->screenPos());
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow *arrow, m_arrows) {
            arrow->updatePosition();
        }
    }

    return value;
}

void DiagramItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPolygonItem::paint(painter, option, widget);
    painter->drawText(-47, 0, m_title);
}

void DiagramItem::imitateMousePress()
{
    mousePressEvent(new QGraphicsSceneMouseEvent(QEvent::MouseButtonPress));
    mouseReleaseEvent(new QGraphicsSceneMouseEvent(QEvent::MouseButtonRelease));
}
