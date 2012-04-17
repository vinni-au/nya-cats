#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QList>
#include <QGraphicsScene>

#include "gridcell.h"
#include "gameitemframe.h"


class GameGrid : public QGraphicsScene
{
protected:
    int m_iXCellCount;
    int m_iYCellCount;
    int m_iXCellSize;
    int m_iYCellSize;

    bool m_bIsGridVisible;

    QList<GridCell*> m_Cells;

//    class ItemsFrame : public QGraphicsItem
//    {
//    protected:
//        QRectF m_Rect;

//        QList<GameItemFrame*> m_Items;

//    public:
//        ItemsFrame(QRectF rect);
//        void AddItem(GameItemFrame* item);

//    protected:
//        QRectF boundingRect() const;
//        QPainterPath shape() const;
//        void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
//    };

//    ItemsFrame* m_ItemsFrame;

public:
    GameGrid(int xCellCount, int yCellCount, int xCellSize, int yCellSize, QObject * parent = 0);
    void Init();
};

#endif // GAMESCENE_H
