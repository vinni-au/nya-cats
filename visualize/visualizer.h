#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QtGui/qwidget.h>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "gamescene.h"
#include "gameitem.h"
#include "grid.h"
#include "core/nkbmanager.h"
#include "core/nframe.h"
#include "core/nslot.h"
#include "core/nfaset.h"
#include "core/nproc.h"
#include "mlv/mlv_define.h"

class Visualizer : public QWidget
{
    Q_OBJECT
private:
    GameScene*	m_Scene;
    int			m_PosCount;
	int			m_ColCount;
	QRectF		m_FactoryRect;
	NKBManager* m_KBManager;

	QMap<QString, QColor*> m_ColorTeamMap;

private:
	void populateScene();
	QRectF& GetNextPos();
	QColor* GetNextColor();

public:
    Visualizer(NKBManager* kbManager, QWidget *parent = 0);

    void Update();
    Grid* GetGrid();
	void RedrawItems();
	QColor* GetColorByTeam(QString team);

signals:

public slots:

};

#endif // VISUALIZER_H
