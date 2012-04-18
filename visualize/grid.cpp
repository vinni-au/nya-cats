#include "grid.h"
#include <QtGui>

// Cell
Cell::Cell(int xIndex, int yIndex, QRectF &rect, QColor &color) :
    GameItemBase(rect, color),
    m_xIndex(xIndex),
    m_yIndex(yIndex)
{
    setAcceptsHoverEvents(true);
}

QRectF Cell::boundingRect() const
{
    return m_Rect;
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor fillColor = (option->state & QStyle::State_MouseOver) ? m_Color.dark(255) : m_Color;
    QBrush b = painter->brush();
    QPen p = painter->pen();

    painter->setBrush(fillColor);
    painter->setPen(m_Color);
    painter->drawRect(m_Rect);
    painter->setBrush(b);
    painter->setPen(p);
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

// Grid
Grid::Grid(int count, QRectF &rect, QColor &color, QGraphicsScene *scene) :
    GameItemBase(rect, color),
    m_Count(count),
    m_Scene(scene)
{
    Init();
}

void Grid::Init()
{
    float xSize = m_Rect.width() / m_Count;
    float ySize = m_Rect.height() / m_Count;

    for (int i = 0; i < m_Count; i++)
        for (int j = 0; j < m_Count; j++)
        {
            QRectF rect(i * xSize, j * ySize, xSize, ySize);
            Cell *cell = new Cell(i, j, rect, m_Color);
            m_Cells.append(cell);
            m_Scene->addItem(cell);
        }
}
