#include "itemtemplatesframe.h"

// ItemTemplate
ItemTemplate::ItemTemplate(GameItem* item, QRectF &rect, QColor &color) :
    Cell(-1, -1, rect, color),
    m_Item(item)
{

}

// ItemTemplatesFrame
ItemTemplatesFrame::ItemTemplatesFrame(QRectF &rect, QColor &color, QGraphicsScene* scene) :
    GameItemBase(rect, color),
    m_Scene(scene)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptsHoverEvents(true);
}

QRectF ItemTemplatesFrame::boundingRect() const
{
    return m_Rect;
}

QPainterPath ItemTemplatesFrame::shape() const
{
    QPainterPath path;
    path.addRect(m_Rect);
    return path;
}

void ItemTemplatesFrame::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor fillColor = (option->state & QStyle::State_MouseOver) ? m_Color.dark(255) : m_Color;
    QBrush b = painter->brush();
    QPen p = painter->pen();

    painter->setBrush(fillColor);
    painter->setPen(m_Color);
    painter->drawRect(m_Rect);
    painter->setBrush(b);
    painter->setPen(p);

    if (stuff.size() > 1)
    {
        QPen p = painter->pen();
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->setBrush(Qt::NoBrush);
        QPainterPath path;
        path.moveTo(stuff.first());
        for (int i = 1; i < stuff.size(); ++i)
            path.lineTo(stuff.at(i));
        painter->drawPath(path);
        painter->setPen(p);
    }
}

void ItemTemplatesFrame::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void ItemTemplatesFrame::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void ItemTemplatesFrame::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void ItemTemplatesFrame::AddItemTemplate(GameItem* item)
{
//    float rect_h = m_Rect.height();
//    float rect_w = m_Rect.width();
//    float pic_h = item->GetPic().height();
//    float pic_w = item->GetPic().width();


//    m_Rect.setHeight(rect_h + pic_h + 20);
//    float sub = rect_w - pic_w;
//    if (sub < 0)
//        m_Rect.setWidth(rect_w + sub + 20);

//    QRectF rect(m_Rect.x() + 10, rect_h + 10, pic_h, pic_w);
//    ItemTemplate* templ = new ItemTemplate(item, rect, m_Color);
//    m_ItemTemplates.append(templ);
//    m_Scene->addItem(templ);
}
