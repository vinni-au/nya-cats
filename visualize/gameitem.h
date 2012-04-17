#ifndef GAMEITEM_H
#define GAMEITEM_H

#include<QPixmap>

class GameItem
{
protected:
    QPixmap *m_Pic;

public:
    GameItem(QPixmap *pic);

    QPixmap* GetPicture();
};

#endif // GAMEITEM_H
