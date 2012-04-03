#ifndef NSLOT_H
#define NSLOT_H

#include <QObject>
#include <QList>

#include "nfaset.h"


#define INHERIT_OVERRIDE    "override"
#define INHERIT_SAME        "same"

//Слот фрейма
//Имеет список фасетов, признак системности.
//
class NSlot : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool system READ isSystem WRITE setSystem SCRIPTABLE true)//системность
public:
    explicit NSlot(QObject *parent = 0);

    NFaset *getFasetByName(QString name);
    QString name();

    //системность
    bool isSystem();
    void setSystem(bool isSystem);
signals:

public slots:

private:
    bool m_system;
    QList<NFaset*> fasets;//список фасетов для поиска
public:
    NFaset slotName;
    NFaset slotType;
    NFaset slotValue;
    NFaset slotDefaultValue;
    NFaset slotMarkerType;
    NFaset slotMarker;
    NFaset slotInheritanceType;
};

#endif // NSLOT_H
