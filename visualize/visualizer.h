#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QtGui/qwidget.h>

QT_FORWARD_DECLARE_CLASS(QGraphicsScene)
QT_FORWARD_DECLARE_CLASS(QGraphicsView)

class Visualizer : public QWidget
{
    Q_OBJECT
public:
    Visualizer(QWidget *parent = 0);

private:
    void populateScene();

    QGraphicsScene *scene;

signals:

public slots:

};

#endif // VISUALIZER_H
