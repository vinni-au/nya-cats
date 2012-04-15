#include "lgen2diagrameditor.hpp"

LGen2DiagramEditor::LGen2DiagramEditor(QWidget *parent) :
    QGraphicsView(parent)
{
    m_scene = new DiagramScene;

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QObject::connect(m_scene, SIGNAL(selectionChanged()),
                     SLOT(sceneSelectionChanged()));
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

void LGen2DiagramEditor::sceneSelectionChanged()
{
    QList<QGraphicsItem*> items = m_scene->selectedItems();
    if (items.count() == 1) {
        DiagramItem* item = qgraphicsitem_cast<DiagramItem*>(items[0]);
        if (item) {
            if (item->type() == DiagramItem::Node)
                emit frameSelected(item->id());
        } else {
            Arrow* arrow = qgraphicsitem_cast<Arrow*>(items[0]);
            if (arrow)
                emit linkSelected(arrow->startItem()->id(), arrow->endItem()->id());
        }
    }
    if (!items.count())
        emit selectionCleared();
}
