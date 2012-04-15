#include <QtGui>

#include "view.h"
#include "visualizer.h"

Visualizer::Visualizer(QWidget *parent) :
    QWidget(parent)
{
    populateScene();

    View *view = new View("GameFieldView");
    view->view()->setScene(scene);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(view);
    setLayout(layout);
}

void Visualizer::populateScene()
{
    scene = new GameGrid(10, 10, 50, 50, this);
    scene->Init();
}
