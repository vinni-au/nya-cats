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
	,m_Bright(100)
{
}

void GameItem::UpdatePic(QString path)
{
	QPixmap *pic = new QPixmap(path);
	m_Pic = *pic;
}

void GameItem::UpdateBrigth(int bright)
{
	if (bright <= 0 || bright >= 300)
		return;

	m_Bright = bright;
}