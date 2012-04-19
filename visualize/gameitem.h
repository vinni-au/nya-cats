#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <QtGui>

enum GameItemType
{
    gitWarior,
    gitHealer,
    gitArcher
};

enum GameTeam
{
    gtRed,
    gtBlue
};

class GameItem
{
protected:
    GameItemType    m_Type;
    QPixmap         m_Pic;
    GameTeam        m_Team;
    int             m_Index;

public:
    GameItem(GameItemType type, QPixmap& pic, GameTeam team, int index);
    QPixmap& GetPic();
    GameTeam GetTeam();
};

#endif // GAMEITEM_H
