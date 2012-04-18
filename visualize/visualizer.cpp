#include <QtGui>

#include "view.h"
#include "visualizer.h"
#include "gameitem.h"

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
    QColor col(100, 100, 100, 50);
    QRectF rect(0, 0, 600, 600);
    QRectF rect1(-70, 0, 60,60);
    QRectF rect2(-70, 70, 60, 60);
    QRectF rect3(-70, 140, 60, 60);

    scene = new GameScene(this);
    scene->CreateGrid(10, rect, col);

    QPixmap pic1("..\\nya-cats\\visualize\\doc.gif");
    QPixmap pic2("..\\nya-cats\\visualize\\war.gif");
    QPixmap pic3("..\\nya-cats\\visualize\\hant.gif");

    GameItem* giWarior = new GameItem(gitWarior, pic2, 0);
    GameItem* giHealer = new GameItem(gitHealer, pic1, 0);
    GameItem* giArcher = new GameItem(gitArcher, pic3, 0);

    scene->CreateFactory(giWarior, rect1);
    scene->CreateFactory(giHealer, rect2);
    scene->CreateFactory(giArcher, rect3);
}
