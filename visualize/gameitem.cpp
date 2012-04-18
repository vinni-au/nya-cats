#include "gameitem.h"

GameItem::GameItem(QPixmap &pic) :
    m_Pic(pic)
{
}

QPixmap& GameItem::GetPic()
{
    return m_Pic;
}
