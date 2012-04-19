#ifndef LGEN2DIAGRAMEDITOR_HPP
#define LGEN2DIAGRAMEDITOR_HPP

#include <QMap>
#include <QGraphicsView>
#include <QDomElement>
#include "diagramscene.hpp"

class LGen2DiagramEditor : public QGraphicsView
{
    Q_OBJECT

    DiagramScene* m_scene;

    QMap<unsigned, DiagramItem*> m_items;

public:
    explicit LGen2DiagramEditor(QWidget *parent = 0);

    DiagramScene* scene() const
    {   return m_scene; }

    unsigned selectedFrameId();

    QDomElement toXML(QDomDocument &doc);

signals:
    void frameSelected(unsigned id);
    void linkSelected(unsigned sid, unsigned eid);
    void selectionCleared();

protected slots:
    void sceneSelectionChanged();

public slots:
    void zoomIn();
    void zoonOut();
    //Добавить вершину с внутренним идентификатором id, надписью title, и типом type
    void addNode(unsigned id, QString title);
    //Удалить вершин с внутренним идентификатором id
    void deleteNode(unsigned id);
    //Добавить связь между от вершины с идентификатором sid до вершины с идентификатором did
    //и надписью title
    void addLink(unsigned sid, unsigned did, QString title);
    void deleteLink(unsigned sid, unsigned did);

};

#endif // LGEN2DIAGRAMEDITOR_HPP
