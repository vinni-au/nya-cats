#include "gameitem.h"
#include "view.h"
#include "gamefield.h"
#include "grid.h"

#include <QtGui>

GameField::GameField(QWidget *parent) :
    QWidget(parent)
{
    populateScene();

    View *view = new View("GameFieldView");
    view->view()->setScene(scene);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(view);
    setLayout(layout);
}

void GameField::populateScene()
{
    scene = new QGraphicsScene;

    // Grid
    QGraphicsItem *grid = new Grid(15, 10, 50, 50);
    grid->setPos(QPointF(0, 0));
    scene->addItem(grid);
}
