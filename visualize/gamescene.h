#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QtGui>
#include <QList>
#include <QGraphicsScene>
#include "grid.h"
#include "gameitemfactory.h"

class GameScene : public QGraphicsScene
{
protected:
    Grid*           m_Grid;
    FactoryFrame*   m_FactoryFrame;

public:
    GameScene(QObject* parent = 0);
    void CreateGrid(int count, QRectF &rect, QColor& color);
    void CreateFactory(GameItem* item, QRectF& rect);

    Grid* GetGrid();
};

#endif // GAMESCENE_H
