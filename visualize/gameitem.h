#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <QtGui>

enum GameItemType
{
    gitWarior,
    gitHealer,
    gitArcher,
    gitMeat
};

enum GameTeam
{
    gtRed,
    gtBlue,
    gtNull
};

class GameItem;

class GameMimeData : public QMimeData
{
private:
    GameItem* m_Item;

public:
    GameMimeData();
    GameItem* GetItem();
    void SetItem(GameItem* item);
};

class GameItem
{
protected:
    GameItemType    m_Type;
    QPixmap         m_Pic;
    GameTeam        m_Team;
    int				m_FrameId;

public:
    GameItem(GameItemType type, QPixmap& pic, GameTeam team, int id = 0);
    QPixmap& GetPic();
    GameTeam GetTeam();
    GameItemType GetType();
	int GetFrameId() {return m_FrameId;}
	void SetFrameId(int id) {m_FrameId = id;}
};

#endif // GAMEITEM_H
