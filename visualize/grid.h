#ifndef GAMEGRID_H
#define GAMEGRID_H

#include <QtGui>
#include "cell.h"

// Grid
class Grid : public QObject
{
    Q_OBJECT
protected:
    int                 m_Count;
    QRectF              m_Rect;
    QColor              m_Color;
    QList<Cell*>        m_Cells;
    QGraphicsScene*     m_Scene;
    Cell*				m_contextMenuCell;

public:
    Grid(int count, QRectF &rect, QColor &color, QGraphicsScene *scene);

    QList<Cell*> GetCells();
    Cell* GetCell(int x, int y);
    Cell* GetCell(int index);
    int GetSideCount();
    int GetCount();

    GameItem* GetGameItem(int x, int y);
    GameItem* GetGameItem(int index);

	void MoveGameItemTo(int fromX, int fromY, int toX, int toY);
	Cell* FindCellByItemFrameId(int id);

	void Clear();

signals:
    void sigFindOutSituation(int x, int y);
	void sigDoVisualizerCell(Cell* cell);

private slots:
    void setContextMenuCell(Cell* cell);
    void findOutSituation();
	void slotDoVisualizerCell(Cell* cell);
};

#endif // GAMEGRID_H
