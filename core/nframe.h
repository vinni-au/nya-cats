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
    explicit NFrame(QObject *parent = 0);

    NSlot* getSlotByName(QString name);


    QString parentFrame();

    int id();

    //сериализация
    QDomElement toXml(QDomDocument& doc);
    void fromXml(QDomElement &frame);
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
