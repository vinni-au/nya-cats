#include "cell.h"

Cell::Cell(int xIndex, int yIndex, QRectF &rect, QColor &color) :
     m_xIndex(xIndex)
    ,m_yIndex(yIndex)
    ,m_Rect(rect)
    ,m_Color(color)
    ,m_Item(NULL)
{
    setAcceptsHoverEvents(true);
    setAcceptDrops(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void Cell::SetGameItem(GameItem* item)
{
    m_Item = item;
    update();
}

QRectF Cell::boundingRect() const
{
    return m_Rect;
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor fillColor;

    if (m_Item)
    {
        painter->drawImage(m_Rect, m_Item->GetPic().toImage());

        QColor red(150, 0, 0, 100);
        QColor blue(0, 0, 150, 100);

        fillColor = red;
        if (m_Item->GetTeam() == gtBlue)
            fillColor = blue;

        fillColor =
                (option->state & QStyle::State_MouseOver) ? fillColor.dark(255) : fillColor;

        QBrush b = painter->brush();
        QPen p = painter->pen();

        painter->setBrush(fillColor);
        painter->setPen(m_Color);
        painter->drawRect(m_Rect);
        painter->setBrush(b);
        painter->setPen(p);
    }
    else
    {
        fillColor =
                (option->state & QStyle::State_MouseOver) ? m_Color.dark(255) : m_Color;
        QBrush b = painter->brush();
        QPen p = painter->pen();

        painter->setBrush(fillColor);
        painter->setPen(m_Color);
        painter->drawRect(m_Rect);
        painter->setBrush(b);
        painter->setPen(p);
    }
}

void Cell::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void Cell::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

void Cell::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    bool b = false;

    GameMimeData *myData =
            (GameMimeData*)(event->mimeData());

    if (myData && m_Item == NULL)
        b = true;

    update();
    event->setAccepted(b);
}

void Cell::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
    update();
}

void Cell::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    GameMimeData *myData =
            (GameMimeData*)(event->mimeData());

    if (myData)
    {
        SetGameItem(myData->GetItem());
    }
    update();
}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_Item)
        setCursor(Qt::ClosedHandCursor);
}

void Cell::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
        .length() < QApplication::startDragDistance() || m_Item == NULL) {
        return;
    }

    QDrag *drag = new QDrag(event->widget());
    GameMimeData *mime = new GameMimeData;
    drag->setMimeData(mime);

    mime->SetItem(m_Item);

    QPixmap pic = m_Item->GetPic();
    drag->setPixmap(pic.scaled(m_Rect.height(), m_Rect.width()));
    drag->setHotSpot(QPoint(15, 20));

    drag->exec();
    setCursor(Qt::OpenHandCursor);
    m_Item = NULL;
    update();
}

void Cell::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_Item)
        setCursor(Qt::OpenHandCursor);
}