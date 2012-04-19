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
    setFlags(ItemIsSelectable);
    setAcceptsHoverEvents(true);
    setAcceptDrops(true);

    QPixmap pic = item->GetPic();
    setPixmap(pic.scaled(rect.width(), rect.height()));
    setPos(rect.x(), rect.y());
}

//QRectF GameItemFactory::boundingRect() const
//{
//    return m_Rect;
//}

//QPainterPath GameItemFactory::shape() const
//{
//    QPainterPath path;
//    path.addRect(m_Rect);
//    return path;
//}

void GameItemFactory::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);
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
    QGraphicsPixmapItem::mousePressEvent(event);
}

void GameItemFactory::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPixmapItem::mouseMoveEvent(event);
}

void GameItemFactory::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}
