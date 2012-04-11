#include "visualizer.h"
#include "view.h"

#include <QtGui>

Visualizer::Visualizer(QWidget *parent) :
    QWidget(parent)
{
    populateScene();

}

void Visualizer::populateScene()
{
    scene = new QGraphicsScene;

    View *view = new View("���� ���");
    view->view()->setScene(scene);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(view);
    setLayout(layout);
}
