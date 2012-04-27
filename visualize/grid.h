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

    QList<Cell*> GetCells();
    Cell* GetCell(int x, int y);
    Cell* GetCell(int index);
    int GetSideCount();
    int GetCount();

    GameItem* GetGameItem(int x, int y);
    GameItem* GetGameItem(int index);
};

#endif // GAMEGRID_H
