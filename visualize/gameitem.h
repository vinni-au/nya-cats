#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <QtGui>

enum GameItemType
{
    gitWarior,
    gitHealer,
    gitArcher
};

class GameItem
{
protected:
    GameItemType    m_Type;
    QPixmap         m_Pic;
    int             m_Team;

public:
    GameItem(GameItemType type, QPixmap& pic, int team);
    QPixmap& GetPic();
};

#endif // GAMEITEM_H
