#ifndef QSPROXYCELL_H
#define QSPROXYCELL_H

#include <QObject>
#include "core/nframe.h"

#include <QScriptEngine>

//#include "mlv/mlv.h"
#include "qsproxy.h"
#include "qsproxyman.h"
#include "qsproxygameobject.h"
#include "mlv/mlv_define.h"

//class MLV;

/**
  @class QSProxyCell
  Класс обертка для фрейма Ячейка игрового поля для использования в скриптах
  */

class QSProxyCell : public QSProxy
{
    Q_OBJECT
    Q_PROPERTY(int X READ X WRITE setX SCRIPTABLE true)
    Q_PROPERTY(int Y READ Y WRITE setY SCRIPTABLE true)
public:
    explicit QSProxyCell(NFrame *cell,QScriptEngine *engine, MLV *mlv,QObject *parent = 0);

    Q_INVOKABLE bool hasGameObject(QString objName);

    Q_INVOKABLE QScriptValue getMan();
    Q_INVOKABLE QScriptValue getFood();
    Q_INVOKABLE QScriptValue getGameObject();

    Q_INVOKABLE void Clear();

    NFrame* getFrame();

    int     X();
    void    setX(int X);

    int     Y();
    void    setY(int Y);

signals:

public slots:

protected:


};

#endif // QSPROXYCELL_H
