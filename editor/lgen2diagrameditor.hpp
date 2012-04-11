#ifndef LGEN2DIAGRAMEDITOR_HPP
#define LGEN2DIAGRAMEDITOR_HPP

#include <QGraphicsView>
#include "diagramscene.hpp"
#include "diagramitem.hpp"

class LGen2DiagramEditor : public QGraphicsView
{
    Q_OBJECT

    DiagramScene* m_scene;

public:
    explicit LGen2DiagramEditor(QWidget *parent = 0);


signals:

public slots:

};

#endif // LGEN2DIAGRAMEDITOR_HPP
