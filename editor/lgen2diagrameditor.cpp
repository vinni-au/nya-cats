#include "lgen2diagrameditor.hpp"

LGen2DiagramEditor::LGen2DiagramEditor(QWidget *parent) :
    QGraphicsView(parent)
{
    m_scene = new DiagramScene;

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void LGen2DiagramEditor::addNode(unsigned id, QString title, DiagramItem::DiagramType type)
{
    DiagramItem* node = new DiagramItem(id, type, title);
    m_scene->addItem(node);
}

void LGen2DiagramEditor::addLink(unsigned sid, unsigned did, QString title)
{
    DiagramItem* i1 = m_items[sid];
    DiagramItem* i2 = m_items[did];
    Arrow* a = new Arrow(i1, i2, title);
    i1->addArrow(a);
    i2->addArrow(a);
    m_scene->addItem(a);
}
