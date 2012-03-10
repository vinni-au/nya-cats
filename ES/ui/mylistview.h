#ifndef MYLISTVIEW_H
#define MYLISTVIEW_H

#include <QListView>
#include <QMouseEvent>
#include <QKeyEvent>

class MyListView : public QListView
{
    Q_OBJECT
public:
    explicit MyListView(QWidget *parent = 0);

signals:
    void currentItemChanged(QModelIndex index);

public slots:

public:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // MYLISTVIEW_H
