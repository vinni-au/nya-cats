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

public:
    Visualizer(QWidget *parent = 0);

signals:
    void AddItemTemplate(GameItem* item);

public slots:

};

#endif // VISUALIZER_H
