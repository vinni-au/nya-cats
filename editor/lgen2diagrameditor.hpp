#ifndef LGEN2DIAGRAMEDITOR_HPP
#define LGEN2DIAGRAMEDITOR_HPP

#include <QMap>
#include <QGraphicsView>
#include <QDomElement>
#include "diagramscene.hpp"
#include <QPointF>
#include <QScrollBar>

class LGen2DiagramEditor : public QGraphicsView
{
    Q_OBJECT

    DiagramScene* m_scene;

    QMap<unsigned, DiagramItem*> m_items;
    QList<Arrow*> m_links;

    QMenu* m_contextMenu;

    //  Центр видимой области

    QPointF center;



    //  Точка фиксации (для прокрутки)

    QPoint fixedPoint;


public:
    explicit LGen2DiagramEditor(QWidget *parent = 0, QMenu* contextMenu = 0);

    DiagramScene* scene() const
    {   return m_scene; }

    unsigned selectedFrameId();

    QDomElement toXML(QDomDocument &doc);
    void fromXML(QDomElement &doc);

    void setContextMenu(QMenu* menu)
    {   m_contextMenu = menu;   }

signals:
    void frameSelected(unsigned id);
    void nodeDeleted(unsigned id);
    void selectionCleared();
    void linkSelected(unsigned, unsigned);
    void isaDeleted(unsigned sid, unsigned did);
    void apoDeleted(unsigned sid, unsigned did);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void mousePressEvent(QMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QMouseEvent *mouseEvent);
    void setCenter(const QPointF& centerPoint);
    QPointF getCenter();
protected slots:
    void sceneSelectionChanged();
    void deleteSelectedItem();
    void deleteSelectedLink();

    void onScrollBar(int i);
public slots:
    void zoomIn();
    void zoonOut();
    //Добавить вершину с внутренним идентификатором id, надписью title, и типом type
    void addNode(unsigned id, QString title);
    void changeNodeTitle(unsigned id, QString newtitle);
    //Удалить вершин с внутренним идентификатором id
    void deleteNode(unsigned id);
    //Добавить связь между от вершины с идентификатором sid до вершины с идентификатором did
    //и надписью title
    void addLink(unsigned sid, unsigned did, QString title);
    void addArrow(Arrow* arrow);
    //Удалить связь
    void deleteLink(unsigned sid, unsigned did);

    void selectNode(unsigned id);
    void selectLink(unsigned sid, unsigned did);

};

#endif // LGEN2DIAGRAMEDITOR_HPP
