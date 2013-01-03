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

#define SYSSTR_FRAMENAME_IMAGE		"�����������"
#define SYSSTR_SLOTNAME_PIC			"��������"
#define SYSSTR_SLOTNAME_ITEMTYPE	"���"
#define SYSSTR_DOMAINNAME_TEAMS		"�������"

class Visualizer : public QWidget
{
    Q_OBJECT
private:
    GameScene*	m_Scene;
    int			m_PosCount;
	int			m_ColCount;
	QRectF		m_FactoryRect;
	NKBManager* m_KBManager;

private:
	void populateScene();
	QRectF& GetNextPos();
	QColor& GetNextColor();

public:
    Visualizer(NKBManager* kbManager, QWidget *parent = 0);

    void Update();
    Grid* GetGrid();
	void RedrawItems();

signals:

public slots:

};

#endif // VISUALIZER_H
