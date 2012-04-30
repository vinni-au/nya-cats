#ifndef NFRAME_H
#define NFRAME_H

#include <QObject>
#include <QList>
#include <QStringList>

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
    explicit NFrame(unsigned id,FrameType::FRAME_TYPE frameType = FrameType::prototype,QObject *parent = 0);

    NSlot* getSlotByName(QString name);     //получить слот по имени
    NSlot* getSlotByIndex(int inx);
    NFaset* GetSlotFaset(QString slotName, QString fasetName);

    int getSlotIndexByName(QString name);

    QString     parentFrame();                  //получить имя родительского фрейма
    void        setParentName(QString name);       //установить родителя

    int id();

    //сериализация
    QDomElement toXml(QDomDocument& doc);
    void fromXml(QDomElement &frame);

    int  slotCount();
    void addSlot(NSlot *slot);
    void insertSlot(int row,NSlot *slot);
    void removeSlot(int row);
    void removeSlot(NSlot *slot);

    QString frameName();
    FrameType::FRAME_TYPE frameType();

    QStringList getSimpleSlotNames();
    QStringList getSubframesSlotNames(QStringList& frames);

    NFrame* createInstance();
    void    merge(NFrame *frame);
    bool    hasSlot(QString slotName);

    QList<NSlot*> getSlotsByDefValue(QString defVal);
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
