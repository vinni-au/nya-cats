#include <QtGui>

#include "view.h"
#include "visualizer.h"
#include "gameitem.h"

Visualizer::Visualizer(QWidget *parent) :
    QWidget(parent)
  ,m_ItemsCount(0)
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
    // быдлобыдлокод

    QColor col(100, 100, 100, 50);
    QRectF rect(0, 0, 600, 600);

    scene = new GameScene(this);
    scene->CreateGrid(10, rect, col);

//    AddGameItem(gitWarior, gtRed);
//    AddGameItem(gitWarior, gtBlue);
//    AddGameItem(gitHealer, gtRed);
//    AddGameItem(gitHealer, gtBlue);
//    AddGameItem(gitArcher, gtRed);
//    AddGameItem(gitArcher, gtBlue);

    QRectF rect1(-70, 0, 60, 60);
    QRectF rect2(-70, 70, 60, 60);
    QRectF rect3(-70, 140, 60, 60);
    QRectF rect4(-70, 210, 60, 60);
    QRectF rect5(-70, 280, 60, 60);
    QRectF rect6(-70, 350, 60, 60);

    QPixmap pic1("..\\nya-cats\\visualize\\war.gif");
    QPixmap pic2("..\\nya-cats\\visualize\\doc.gif");
    QPixmap pic3("..\\nya-cats\\visualize\\hant.gif");

    scene->CreateFactory(new GameItem(gitWarior, pic1, gtRed, -1), rect1);
    scene->CreateFactory(new GameItem(gitHealer, pic2, gtRed, -1), rect2);
    scene->CreateFactory(new GameItem(gitArcher, pic3, gtRed, -1), rect3);

    scene->CreateFactory(new GameItem(gitWarior, pic1, gtBlue, -1), rect4);
    scene->CreateFactory(new GameItem(gitHealer, pic2, gtBlue, -1), rect5);
    scene->CreateFactory(new GameItem(gitArcher, pic3, gtBlue, -1), rect6);

}

void Visualizer::Update()
{

}

Grid* Visualizer::GetGrid()
{
    return scene->GetGrid();
}

void Visualizer::AddGameItem(GameItemType type, GameTeam team)
{
    // быдлобыдлокод

    QString path = "..\\nya-cats\\visualize\\";
    QRectF rect;

    if (type == gitWarior)
    {
        path += "war.gif";
        rect.setRect(-70, 0, 60, 60);
    }
    else if (type == gitHealer)
    {
        path += "doc.gif";
        rect.setRect(-70, 70, 60, 60);
    }
    else
    {
        path += "hant.gif";
        rect.setRect(-70, 140, 60, 60);
    }

    QPixmap pic(path);
    GameItem* item = new GameItem(type, pic, team, m_ItemsCount++);
    scene->CreateFactory(item, rect);
}
