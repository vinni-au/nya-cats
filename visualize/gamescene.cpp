#include "gamescene.h"


GameScene::GameScene(QObject* parent) :
    QGraphicsScene(parent), m_FactoryFrame(0)
{
}

void GameScene::CreateGrid(int count, QRectF &rect, QColor &color)
{
    m_Grid = new Grid(count, rect, color, this);
}

void GameScene::CreateFactory(GameItem* item, QRectF& rect)
{
    if (!m_FactoryFrame)
        m_FactoryFrame = new FactoryFrame(this);

    m_FactoryFrame->AddItem(item, rect);
}
