#include "mylistview.h"
#include <QDebug>

MyListView::MyListView(QWidget *parent) :
    QListView(parent)
{
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setDragEnabled(true);
    this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    this->setDragDropMode(QAbstractItemView::InternalMove);
    this->setDragDropOverwriteMode(false);
}
void
MyListView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    this->scrollTo(current);
    emit currentItemChanged(current);
}

void
MyListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();
}
void
MyListView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F2)
    {
        event->accept();
    }
    else
    QListView::keyPressEvent(event);
}
