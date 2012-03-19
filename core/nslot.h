#ifndef NSLOT_H
#define NSLOT_H

#include <QObject>

class NSlot : public QObject
{
    Q_OBJECT
public:
    explicit NSlot(QObject *parent = 0);

signals:

public slots:

};

#endif // NSLOT_H
