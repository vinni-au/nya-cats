#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QList>
#include <QGraphicsScene>
#include "grid.h"
#include "itemtemplatesframe.h"
#include "gameitem.h"

class GameScene : public QGraphicsScene
{
protected:
    Grid *m_Grid;
    ItemTemplatesFrame *m_ItemsFrame;

public:
    GameScene(QObject* parent = 0);
    void CreateGrid(int count, QRectF &rect, QColor& color);
    void CreateItemsFrame(QRectF &rect, QColor& color);
    void AddItemTemplate(GameItem* item);
};

#endif // GAMESCENE_H
