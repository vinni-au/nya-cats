#include "gameitem.h"

GameMimeData::GameMimeData() :
    QMimeData()
{
}

GameItem* GameMimeData::GetItem()
{
    return m_Item;
}

void GameMimeData::SetItem(GameItem* item)
{
    m_Item = item;
}

GameItem::GameItem(GameItemType type, QPixmap& pic, GameTeam team, int index) :
   m_Type(type)
  ,m_Pic(pic)
  ,m_Team(team)
  ,m_Index(index)
{
}

QPixmap& GameItem::GetPic()
{
    return m_Pic;
}

GameTeam GameItem::GetTeam()
{
    return m_Team;
}

GameItemType GameItem::GetType()
{
    return m_Type;
}
