#ifndef VIEW_H
#define VIEW_H

#include <QFrame>
#include <QGraphicsView>

class View : public QFrame
{
    Q_OBJECT
public:
    View(const QString &name, QWidget *parent = 0);
    QGraphicsView *view() const;

private:
    QGraphicsView *graphicsView;

signals:

public slots:

};

#endif // VIEW_H
