#ifndef LGEN2DIAGRAMEDITOR_HPP
#define LGEN2DIAGRAMEDITOR_HPP

#include <QMap>
#include <QGraphicsView>
#include "diagramscene.hpp"

class LGen2DiagramEditor : public QGraphicsView
{
    Q_OBJECT

    DiagramScene* m_scene;

    QMap<unsigned, DiagramItem*> m_items;

public:
    explicit LGen2DiagramEditor(QWidget *parent = 0);

signals:
    void frameSelected(unsigned id);
    void linkSelected(unsigned sid, unsigned eid);
    void selectionCleared();

protected slots:
    void sceneSelectionChanged();

public slots:
    //Добавить вершину с внутренним идентификатором id, надписью title, и типом type
    void addNode(unsigned id, QString title, DiagramItem::DiagramType type = DiagramItem::Node);
    //Добавить связь между от вершины с идентификатором sid до вершины с идентификатором did
    //и надписью title
    void addLink(unsigned sid, unsigned did, QString title);

};

#endif // LGEN2DIAGRAMEDITOR_HPP
