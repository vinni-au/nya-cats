#include "lgen2diagrameditor.hpp"

LGen2DiagramEditor::LGen2DiagramEditor(QWidget *parent) :
    QGraphicsView(parent)
{
    m_scene = new DiagramScene;

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::TextAntialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    setScene(m_scene);

    QObject::connect(m_scene, SIGNAL(selectionChanged()),
                     SLOT(sceneSelectionChanged()));
}

void LGen2DiagramEditor::addNode(unsigned id, QString title)
{
    QMenu* menu = new QMenu;
    QAction* act = menu->addAction("Удалить фрейм");
    QObject::connect(act, SIGNAL(triggered()),
                     SLOT(deleteSelectedItem()));
    DiagramItem* node = new DiagramItem(id, DiagramItem::Node, title, menu);
    m_items.insert(id, node);
    m_scene->addItem(node);
}

void LGen2DiagramEditor::deleteNode(unsigned id)
{
    DiagramItem* item = m_items[id];
    if (item) {
        item->removeArrows();
        m_items.remove(id);
        m_scene->removeItem(item);
        delete item;
    }
}

void LGen2DiagramEditor::addLink(unsigned sid, unsigned did, QString title)
{
    DiagramItem* i1 = m_items[sid];
    DiagramItem* i2 = m_items[did];
    Arrow* a = new Arrow(i1, i2, title);
    m_links << a;
    i1->addArrow(a);
    i2->addArrow(a);
    m_scene->addItem(a);
}

void LGen2DiagramEditor::deleteLink(unsigned sid, unsigned did)
{
    Arrow* a = 0;
    foreach (Arrow* arrow, m_links) {
        if (arrow->startItem()->id() == sid &&
                arrow->endItem()->id() == did)
            a = arrow;
    }
    m_links.removeAll(a);
    DiagramItem* si = m_items[sid];
    DiagramItem* di = m_items[did];
    si->removeArrowTo(di);
    di->removeArrowFrom(si);
}

void LGen2DiagramEditor::sceneSelectionChanged()
{
    QList<QGraphicsItem*> items = m_scene->selectedItems();
    if (items.count() == 1) {
        DiagramItem* item = qgraphicsitem_cast<DiagramItem*>(items[0]);
        if (item) {
            if (item->diagramType() == DiagramItem::Node)
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

unsigned LGen2DiagramEditor::selectedFrameId()
{
    QList<QGraphicsItem*> items = m_scene->selectedItems();
    if (items.count() == 1) {
        DiagramItem* item = qgraphicsitem_cast<DiagramItem*>(items[0]);
        if (item)
            if (item->diagramType() == DiagramItem::Node)
                return item->id();
    }
    return -1;
}

void LGen2DiagramEditor::selectNode(unsigned id)
{
    blockSignals(true);
    QList<QGraphicsItem*> items = m_scene->items();
    foreach (QGraphicsItem* item, items) {
        DiagramItem *ditem = qgraphicsitem_cast<DiagramItem*>(item);
        if (ditem)
            if (ditem->id() == id)
                ditem->imitateMousePress();
    }
    blockSignals(false);
}

void LGen2DiagramEditor::deleteSelectedItem()
{
    unsigned id = selectedFrameId();
    if (id != -1)
        emit nodeDeleted(id);
}

void LGen2DiagramEditor::selectLink(unsigned sid, unsigned did)
{
    //Вроде как нафиг не нужно
}

void LGen2DiagramEditor::zoomIn()
{
    scale(1.3, 1.3);
}

void LGen2DiagramEditor::zoonOut()
{
    scale(1 / 1.3, 1 / 1.3);
}

QDomElement LGen2DiagramEditor::toXML(QDomDocument &doc)
{
    QDomElement dElem = doc.createElement("diagram");

    foreach (QGraphicsItem* item, m_items) {
        DiagramItem* ditem = qgraphicsitem_cast<DiagramItem*>(item);
        if (ditem) {
            QDomElement ielem = doc.createElement("item");
            ielem.setAttribute("x", ditem->x());
            ielem.setAttribute("y", ditem->y());
            ielem.setAttribute("title", ditem->title());
            ielem.setAttribute("id", ditem->id());
            dElem.appendChild(ielem);
        }
    }

    foreach (Arrow* arrow, m_links) {
        QDomElement aelem = doc.createElement("link");
        aelem.setAttribute("text", arrow->text());
        aelem.setAttribute("sid", arrow->startItem()->id());
        aelem.setAttribute("did", arrow->endItem()->id());
        dElem.appendChild(aelem);
    }

    return dElem;
}

void LGen2DiagramEditor::fromXML(QDomElement &elem)
{
    if (elem.tagName() == "diagram") {
        QDomNodeList nodes = elem.childNodes();
        int count = nodes.count();
        for (int i = 0; i < count; ++i) {
            QDomElement e = nodes.at(i).toElement();
            if (e.tagName() == "item") {
                unsigned id = e.attribute("id").toUInt();
                QString title = e.attribute("title");
                qreal x = e.attribute("x").toDouble();
                qreal y = e.attribute("y").toDouble();
                DiagramItem* item = new DiagramItem(id, DiagramItem::Node, title);
                item->setX(x);
                item->setY(y);
                m_items.insert(id, item);
                m_scene->addItem(item);
            } else if (e.tagName() == "link") {
                QString text = e.attribute("text");
                unsigned sid = e.attribute("sid").toUInt();
                unsigned did = e.attribute("did").toUInt();
                DiagramItem* start = m_items.value(sid, 0);
                DiagramItem* end = m_items.value(did, 0);
                Arrow* a = new Arrow(start, end, text);
                m_links << a;
                m_scene->addItem(a);
            }
        }
    }
}
