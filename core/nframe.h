#ifndef NFRAME_H
#define NFRAME_H

#include <QObject>
#include <QList>

#include "nslot.h"

namespace FrameType {
enum FRAME_TYPE
{
    prototype,
    exemplar
};
}

//Фрейм
//
class NFrame : public QObject
{
    Q_OBJECT
public:
    explicit NFrame(unsigned id,QObject *parent = 0);

    NSlot* getSlotByName(QString name);     //получить слот по имени
    NSlot* getSlotByIndex(int inx);

    QString parentFrame();                  //получить имя родительского фрейма

    int id();

    //сериализация
    QDomElement toXml(QDomDocument& doc);
    void fromXml(QDomElement &frame);

    int slotCount();
    void addSlot(NSlot *slot);
    void insertSlot(int row,NSlot *slot);
    void removeSlot(int row);
    void removeSlot(NSlot *slot);
signals:

public slots:

private:
    QList<NSlot*> m_slots;
    int m_id;
    FrameType::FRAME_TYPE m_type;
public:
    NSlot name;
    NSlot is_a;

};

#endif // NFRAME_H
