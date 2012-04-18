#ifndef GAMEITEMBASE_H
#define GAMEITEMBASE_H

#include <QtGui>

class GameItemBase
{
protected:
    QRectF m_Rect;
    QColor m_Color;

public:
    GameItemBase(QRectF rect, QColor color);
};

#endif // GAMEITEMBASE_H
