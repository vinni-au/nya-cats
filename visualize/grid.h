#ifndef GAMEGRID_H
#define GAMEGRID_H

#include <QtGui>
#include "cell.h"

// Grid
class Grid
{
protected:
    int                 m_Count;
    QRectF              m_Rect;
    QColor              m_Color;
    QList<Cell*>        m_Cells;
    QGraphicsScene*     m_Scene;

public:
    Grid(int count, QRectF &rect, QColor &color, QGraphicsScene *scene);
};

#endif // GAMEGRID_H
