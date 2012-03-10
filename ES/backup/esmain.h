#ifndef ESMAIN_H
#define ESMAIN_H

#include <QMainWindow>
#include <QMdiArea>

#include "domainwnd.h"
#include "varswnd.h"
#include "ruleswnd.h"

namespace Ui {
    class ESmain;
}

class ESmain : public QMainWindow
{
    Q_OBJECT

public:
    explicit ESmain(QWidget *parent = 0);
    ~ESmain();

private:
    QMdiArea *mdiArea;

};

#endif // ESMAIN_H
