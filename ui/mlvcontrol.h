#ifndef MLVCONTROL_H
#define MLVCONTROL_H

#include <QFrame>
#include <QIcon>
#include "mlv/mlv.h"
#include "core/nframe.h"
#include "core/frame_model/nframenetmodel.h"
#include "core/frame_model/nframenetmodelhierarchical.h"

namespace Ui {
    class MLVControl;
}

class MLVControl : public QFrame
{
    Q_OBJECT

    NFramenetModel* m_frameModel;
	QTreeWidgetItem* m_currentItem;

public:
    explicit MLVControl(MLV* mlv, QWidget *parent = 0);
    ~MLVControl();

    void setWorkMemory(QList<NFrame*> *list);

private:
    Ui::MLVControl *ui;
    MLV* m_Mlv;

public slots:
    void AddMsg(QString msg);
    void ClearLog();

	// Для работы с LineOfReasoning
	void LOR_AddRoot(QString name);
	void LOR_AddNextLevel(QString name);
	void LOR_AddCurrLevel(QString name);
	void LOR_GotoPrevLevel();
	void LOR_SetCurrState(bool bOk);
	void LOR_Clear();
};

#endif // MLVCONTROL_H
