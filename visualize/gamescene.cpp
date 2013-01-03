#include "gamescene.h"


GameScene::GameScene(QObject* parent) :
    QGraphicsScene(parent)
  ,m_FactoryFrame(NULL)
{
	m_FactoryFrame = new FactoryFrame(this);
}

void GameScene::CreateGrid(int count, QRectF &rect, QColor &color)
{
    m_Grid = new Grid(count, rect, color, this);
}

void GameScene::CreateFactory(GameItem* item, QRectF& rect)
{
    m_FactoryFrame->AddItem(item, rect);
}

Grid* GameScene::GetGrid()
{
    return m_Grid;
}

void GameScene::Clear()
{
	m_FactoryFrame->Clear();
	m_Grid->Clear();
}