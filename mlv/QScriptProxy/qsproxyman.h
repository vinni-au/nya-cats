#ifndef QSPROXYMAN_H
#define QSPROXYMAN_H

#include <QObject>
#include "core/nframe.h"

class QSProxyMan : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int X READ X WRITE setX SCRIPTABLE true)
    //Q_PROPERTY(int y READ y WRITE setY SCRIPTABLE true)

public:
    explicit QSProxyMan(NFrame *man,QObject *parent = 0);

    int     X();
    void    setX(int X);

signals:

public slots:

protected:
    NFrame  *m_frame;

};

#endif // QSPROXYMAN_H
