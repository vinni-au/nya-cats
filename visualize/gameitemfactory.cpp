#include "gameitemfactory.h"

// FactoryFrame
FactoryFrame::FactoryFrame(QGraphicsScene* scene) :
    m_Scene(scene)
{
	m_FactoryList.append(NULL);
}

void FactoryFrame::AddItem(GameItem* item, QRectF& rect)
{
    GameItemFactory* factory = new GameItemFactory(item, rect);
    m_FactoryList.append(factory);
    m_Scene->addItem(factory);
}

void FactoryFrame::Clear()
{
	for (int i = 0; i < m_FactoryList.size(); i++)
	{
		m_Scene->removeItem(m_FactoryList[i]);
		delete m_FactoryList[i];
	}
	m_FactoryList.clear();
	m_FactoryList.append(NULL);
}

// GameItemFactory
GameItemFactory::GameItemFactory(GameItem* item, QRectF& rect) :
   QGraphicsPixmapItem()
  ,m_Item(item)
  ,m_Rect(rect)
{
    setFlags(ItemIsMovable);
    setAcceptsHoverEvents(true);

    QPixmap pic = item->GetPic();
    setOffset(rect.x(), rect.y());
    setPixmap(pic.scaled(rect.width(), rect.height()));

    setCursor(Qt::OpenHandCursor);
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

    if (m_Item->GetTeam().isEmpty())
        return;

    QColor fillColor = m_Item->GetColor();

    fillColor = (option->state & QStyle::State_MouseOver) ? fillColor.dark(255) : fillColor;
    QBrush b = painter->brush();
    painter->setBrush(fillColor);
    painter->drawRect(m_Rect);
    painter->setBrush(b);
}

void GameItemFactory::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsPixmapItem::hoverEnterEvent(event);
}

void GameItemFactory::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsPixmapItem::hoverLeaveEvent(event);
}

void GameItemFactory::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
}

void GameItemFactory::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QDrag *drag = new QDrag(event->widget());
    GameMimeData *mime = new GameMimeData;
    drag->setMimeData(mime);

    mime->SetItem(new GameItem
		(
			m_Item->GetType(), 
			m_Item->GetPic(), 
			m_Item->GetTeam(), 
			m_Item->GetColor(), 
			m_Item->GetFrameId()
		));

    QPixmap pic = m_Item->GetPic();
    drag->setPixmap(pic.scaled(m_Rect.height(), m_Rect.width()));
    drag->setHotSpot(QPoint(15, 20));

    drag->exec();
    setCursor(Qt::OpenHandCursor);
}

void GameItemFactory::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);
}
