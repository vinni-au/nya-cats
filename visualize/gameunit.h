#ifndef GAMEUNIT_H
#define GAMEUNIT_H

#include <QGraphicsPixmapItem>

enum gcGameClasses
{
    gcWarior,
    gcHealer,
    gcArcher
};

class gameunit : public QGraphicsPixmapItem
{
public:
    gameunit(gcGameClasses gc, int teamNumb);

protected:
    gcGameClasses m_gcClass;
    int m_iTeamNumb;
};

#endif // GAMEUNIT_H
