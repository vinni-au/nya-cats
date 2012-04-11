#include "view.h"

#include <QtGui>

View::View(const QString &name, QWidget *parent) :
    QFrame(parent)
{
}

QGraphicsView *View::view() const
{
    return graphicsView;
}
