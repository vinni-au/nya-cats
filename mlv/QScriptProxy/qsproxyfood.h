#ifndef QSPROXYFOOD_H
#define QSPROXYFOOD_H

#include "qsproxy.h"
#include "core/nframe.h"
#include <QScriptEngine>

class QSProxyFood : public QSProxy
{
    Q_OBJECT
public:
    explicit QSProxyFood(NFrame *food,QScriptEngine *engine,QObject *parent = 0);

signals:

public slots:

};

#endif // QSPROXYFOOD_H
