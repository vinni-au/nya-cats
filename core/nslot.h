#ifndef NSLOT_H
#define NSLOT_H

#include <QObject>
#include <QList>
#include <QVariant>

#include "nfaset.h"

//вариант наследования
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
    NFaset *getFasetByIndex(int inx);

    QString name();//имя слота
    void setName(QString name);

    //QVariant value();//значение слота
    //void setValue(QVariant value);

    QVariant defValue();//задание отсутствия
    void setDefValue(QVariant value);


    //системность
    bool isSystem();
    void setSystem(bool isSystem);

    //сериализация
    QDomElement toXml(QDomDocument& doc);
    void fromXml(QDomElement &slot);

    int fasetCount();
    int getFasetIndexByName(QString name);

    void setHasFasetValue(bool b);
    void initFromSlot(NSlot *slot);

    QString getSlotType();
    QString getSlotMarkerType();
    QString getSlotMarker();
    QString getSlotInheritanceType();
signals:

public slots:

private:
    bool m_system;
    QList<NFaset*> m_fasets;//список фасетов для поиска
public:
    NFaset slotName;
    NFaset slotType;
    //NFaset slotValue;
    NFaset slotDefaultValue;
    NFaset slotMarkerType;
    NFaset slotMarker;
    NFaset slotInheritanceType;
};

#endif // NSLOT_H
