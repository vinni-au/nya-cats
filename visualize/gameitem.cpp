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

GameItem::GameItem(QString type, QPixmap& pic, QString team, QColor color, int id) :
	m_Type(type)
	,m_Pic(pic)
	,m_Color(color)
	,m_Team(team)
	,m_FrameId(id)
{
}
