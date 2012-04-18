#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <QtGui>

class GameItem
{
protected:
    QPixmap m_Pic;

public:
    GameItem(QPixmap &pic);
    QPixmap& GetPic();
};

#endif // GAMEITEM_H
