#include "gamescene.h"


GameScene::GameScene(QObject* parent) :
    QGraphicsScene(parent)
{
}

void GameScene::CreateGrid(int count, QRectF &rect, QColor &color)
{
    m_Grid = new Grid(count, rect, color, this);
}

void GameScene::CreateItemsFrame(QRectF &rect, QColor &color)
{
    m_ItemsFrame = new ItemTemplatesFrame(rect, color, this);
    this->addItem(m_ItemsFrame);
}

void GameScene::AddItemTemplate(GameItem* item)
{
    m_ItemsFrame->AddItemTemplate(item);
}
