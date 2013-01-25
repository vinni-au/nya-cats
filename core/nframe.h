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

/**
  @class NFrame
  Фрейм
*/

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

    Q_INVOKABLE int id();
    void setId(int id);

    bool isSystem();

    //сериализация
    QDomElement toXml(QDomDocument& doc);
    void fromXml(QDomElement &frame);

    int  slotCount();
    void addSlot(NSlot *slot);
    void insertSlot(int row,NSlot *slot);
    void removeSlot(int row);
    void removeSlot(NSlot *slot);
    void removeSlot(QString name);

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
    /// id фрейма
    int m_id;
    /// Тип фрейма
    FrameType::FRAME_TYPE m_type;

    /// Признак системности фрейма
    bool m_system;
public:
    NSlot name;
    NSlot is_a;

};

#endif // NFRAME_H
