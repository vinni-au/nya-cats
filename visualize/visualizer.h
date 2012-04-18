#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QtGui/qwidget.h>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "gamescene.h"

class Visualizer : public QWidget
{
    Q_OBJECT
private:
    void populateScene();
    GameScene *scene;

public:
    Visualizer(QWidget *parent = 0);

signals:

public slots:

};

#endif // VISUALIZER_H
