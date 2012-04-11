#include "gameitem.h"
#include "view.h"
#include "gamefield.h"

#include <QtGui>

GameField::GameField(QWidget *parent) :
    QWidget(parent)
{
    populateScene();

    View *view = new View("");
    view->view()->setScene(scene);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(view);
    setLayout(layout);
}

void GameField::populateScene()
{
    scene = new QGraphicsScene;

    QColor color(100, 100, 100);
    QGraphicsItem *item = new GameItem(color, 100, 100);
    item->setPos(QPointF(0, 0));
    scene->addItem(item);
}
