#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QtGui/qwidget.h>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "gamescene.h"
#include "gameitem.h"

class Visualizer : public QWidget
{
    Q_OBJECT
private:
    void populateScene();
    GameScene *scene;
    int m_ItemsCount;

public:
    Visualizer(QWidget *parent = 0);
    void AddGameItem(GameItemType type, GameTeam team);

signals:

public slots:

};

#endif // VISUALIZER_H
