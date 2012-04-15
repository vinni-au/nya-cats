#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QtGui/qwidget.h>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "gamegrid.h"

class Visualizer : public QWidget
{
    Q_OBJECT
public:
    Visualizer(QWidget *parent = 0);

private:
    void populateScene();

   GameGrid *scene;

signals:

public slots:

};

#endif // VISUALIZER_H
