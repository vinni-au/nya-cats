#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QList>
#include <QGraphicsScene>

#include "gridcell.h"



class GameGrid : public QGraphicsScene
{
protected:
    int m_iXCount;
    int m_iYCount;
    int m_iXSize;
    int m_iYSize;

    bool m_bIsGridVisible;

    QList<GridCell*> m_Cells;

public:
    GameGrid(int xCount, int yCount, int xSize, int ySize, QObject * parent = 0);
    void Init();
};

#endif // GAMESCENE_H
