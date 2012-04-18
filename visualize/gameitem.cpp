#include "gameitem.h"

GameItem::GameItem(GameItemType type, QPixmap& pic, int team) :
   m_Type(type)
  ,m_Pic(pic)
  ,m_Team(team)
{
}

QPixmap& GameItem::GetPic()
{
    return m_Pic;
}
