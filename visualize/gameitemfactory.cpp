#include "gameitemfactory.h"

// FactoryFrame
FactoryFrame::FactoryFrame(QGraphicsScene* scene) :
    m_Scene(scene)
{
}

void FactoryFrame::AddItem(GameItem* item, QRectF& rect)
{
    GameItemFactory* factory = new GameItemFactory(item, rect);
    m_FactoryList.append(factory);
    m_Scene->addItem(factory);
}

// GameItemFactory
GameItemFactory::GameItemFactory(GameItem* item, QRectF& rect) :
   QGraphicsPixmapItem()
  ,m_Item(item)
  ,m_Rect(rect)
{
    setShapeMode(QGraphicsPixmapItem::MaskShape);
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptsHoverEvents(true);
    setAcceptDrops(true);

    QPixmap pic = item->GetPic();
    setOffset(rect.x(), rect.y());
    setPixmap(pic.scaled(rect.width(), rect.height()));
}

QRectF GameItemFactory::boundingRect() const
{
    return m_Rect;
}

QPainterPath GameItemFactory::shape() const
{
    QPainterPath path;
    path.addRect(m_Rect);
    return path;
}

void GameItemFactory::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);

    QColor red(150, 0, 0, 100);
    QColor blue(0, 0, 150, 100);

    QColor fillColor = red;
    if (m_Item->GetTeam() == gtBlue)
        fillColor = blue;

    fillColor = (option->state & QStyle::State_MouseOver) ? fillColor.dark(255) : fillColor;
    QBrush b = painter->brush();
    painter->setBrush(fillColor);
    painter->drawRect(m_Rect);
    painter->setBrush(b);
}

void GameItemFactory::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void GameItemFactory::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsPixmapItem::hoverLeaveEvent(event);
}

void GameItemFactory::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsPixmapItem::mousePressEvent(event);
}

void GameItemFactory::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsPixmapItem::mouseMoveEvent(event);
}

void GameItemFactory::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPixmapItem::mouseReleaseEvent(event);
    update();
}
