#ifndef MLVCONTROL_H
#define MLVCONTROL_H

#include <QFrame>
#include "mlv/mlv.h"
#include "core/nframe.h"
#include "core/frame_model/nframenetmodel.h"

namespace Ui {
    class MLVControl;
}

class MLVControl : public QFrame
{
    Q_OBJECT

    NFramenetModel* m_frameModel;

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
};

#endif // MLVCONTROL_H
