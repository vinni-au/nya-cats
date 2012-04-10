#ifndef DIAGRAMSCENE_HPP
#define DIAGRAMSCENE_HPP

#include <QGraphicsScene>

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

    QColor m_textColor;
    QColor m_itemColor;
    QColor m_lineColor;

public:
    explicit DiagramScene(QObject *parent = 0);

signals:

public slots:

};

#endif // DIAGRAMSCENE_HPP
