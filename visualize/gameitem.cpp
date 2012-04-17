#include "gameitem.h"

GameItem::GameItem(QPixmap *pic) :
    m_Pic(pic)
{
}

QPixmap* GameItem::GetPicture()
{
    return m_Pic;
}
