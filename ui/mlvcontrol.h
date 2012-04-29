#ifndef MLVCONTROL_H
#define MLVCONTROL_H

#include <QFrame>
#include "mlv/mlv.h"

namespace Ui {
    class MLVControl;
}

class MLVControl : public QFrame
{
    Q_OBJECT

public:
    explicit MLVControl(MLV* mlv, QWidget *parent = 0);
    ~MLVControl();

private:
    Ui::MLVControl *ui;
    MLV* m_Mlv;

public slots:
    void AddMsg(QString msg);
    void ClearLog();
};

#endif // MLVCONTROL_H
