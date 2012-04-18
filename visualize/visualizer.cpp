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
    QColor col(100, 100, 100, 50);
    QRectF rect(0, 0, 500, 500);
    QRectF rect1(-70, 0, 60, 100);

    scene = new GameScene(this);
    scene->CreateGrid(10, rect, col);
    scene->CreateItemsFrame(rect1, col);

    QPixmap pic1("D:\\_develop\\QtProjects\\nya-cats\\visualize\\doc.gif");
    QPixmap pic2("D:\\_develop\\QtProjects\\nya-cats\\visualize\\war.gif");
    QPixmap pic3("D:\\_develop\\QtProjects\\nya-cats\\visualize\\hant.gif");

    GameItem* item = new GameItem(pic1);
    scene->AddItemTemplate(item);

    item = new GameItem(pic2);
    scene->AddItemTemplate(item);

    item = new GameItem(pic3);
    scene->AddItemTemplate(item);
}
