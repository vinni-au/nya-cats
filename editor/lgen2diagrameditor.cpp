#include "lgen2diagrameditor.hpp"

LGen2DiagramEditor::LGen2DiagramEditor(QWidget *parent) :
    QGraphicsView(parent)
{
    m_scene = new DiagramScene;
    DiagramItem* i1 = new DiagramItem(0, DiagramItem::Node, "Hello");
    DiagramItem* i2 = new DiagramItem(0, DiagramItem::Node, "Goodbye");
    Arrow* a = new Arrow(i1, i2, "and");
    m_scene->addItem(i1);
    m_scene->addItem(i2);

    i1->addArrow(a);
    i2->addArrow(a);

    m_scene->addItem(a);

    setScene(m_scene);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}
