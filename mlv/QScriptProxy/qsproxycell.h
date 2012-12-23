#ifndef QSPROXYCELL_H
#define QSPROXYCELL_H

#include <QObject>
#include "core/nframe.h"
#include "qsproxy.h"
#include <QScriptEngine>
#include "qsproxyman.h"


class QSProxyCell : public QSProxy
{
    Q_OBJECT
    Q_PROPERTY(int X READ X WRITE setX SCRIPTABLE true)
    Q_PROPERTY(int Y READ Y WRITE setY SCRIPTABLE true)
public:
    explicit QSProxyCell(NFrame *cell,QScriptEngine *engine,QObject *parent = 0);

    Q_INVOKABLE QScriptValue getMan();

    int     X();
    void    setX(int X);

    int     Y();
    void    setY(int Y);

signals:

public slots:

protected:


};

#endif // QSPROXYCELL_H
