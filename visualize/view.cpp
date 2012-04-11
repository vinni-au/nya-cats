#include "view.h"

#include <QtGui>

View::View(const QString &name, QWidget *parent) :
    QFrame(parent)
{
    graphicsView = new QGraphicsView;
    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(graphicsView);
    setLayout(layout);
}

QGraphicsView *View::view() const
{
    return graphicsView;
}
