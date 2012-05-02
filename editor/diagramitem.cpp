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
    Arrow *arrow = 0;
    QList<Arrow*> arrows2del;
    for (int i = 0; i < m_arrows.count(); ++i) {
        arrow = m_arrows[i];
        if (arrow) {
//            arrow->startItem()->removeArrow(arrow);
//            arrow->endItem()->removeArrow(arrow);
            arrows2del << arrow;
//            scene()->removeItem(arrow);
//            delete arrow;
        }
        arrow = 0;
    }
    for (int i = 0; i < arrows2del.count(); ++i) {
        DiagramScene* dscene = static_cast<DiagramScene*>(scene());
        dscene->removeItem(arrows2del[i]);
        arrows2del[i]->startItem()->removeArrow(arrows2del[i]);
        arrows2del[i]->endItem()->removeArrow(arrows2del[i]);
        m_arrows.removeAll(arrows2del[i]);
        delete arrows2del[i];
    }
    arrows2del.clear();
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
    if (isSelected()) {
        painter->setBrush(QBrush(QColor(0, 0, 255, 40)));
        painter->drawPolygon(polygon());
    }
}

void DiagramItem::imitateMousePress()
{
    mousePressEvent(new QGraphicsSceneMouseEvent(QEvent::MouseButtonPress));
    mouseReleaseEvent(new QGraphicsSceneMouseEvent(QEvent::MouseButtonRelease));
}
