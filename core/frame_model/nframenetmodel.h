#ifndef NFRAMENETMODE_H
#define NFRAMENETMODE_H

#include <QAbstractItemModel>
#include "nframenode.h"
#include "core/nframe.h"

/**
  @class NFramenetModel
  Модель фреймов
  */

class NFramenetModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit NFramenetModel(QObject *parent=0);
    virtual ~NFramenetModel();

    void setRootNode(NFrameNode *node);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;

    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual bool removeRow(int row, const QModelIndex &parent);
    virtual bool insertRow(int row, const QModelIndex &parent);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    //мои
    virtual void setFrames( QList<NFrame*> *frames);

    int getIdByIndex(QModelIndex index);

    void update();

    QModelIndex addSlot(QModelIndex& frameIndex);
    bool deleteSlot(QModelIndex& slotIndex);

    bool setSlotFasetValue(QModelIndex slotIndex,QString fasetName,QVariant value);
    QVariant getSlotFasetValue(QModelIndex slotIndex,QString fasetName);

    bool isSlot(QModelIndex slotIndex);

    QModelIndex getFrameIndexById(int id);
    QModelIndex getSlotFasetIndex(QModelIndex frameIndex,QString slotName,QString fasetName);
    QModelIndex getSlotFasetIndex(QModelIndex slotIndex,QString fasetName);
    //QDomElement toXml(QDomDocument& doc);
    //void fromXml(QDomElement &domains);

    QString getFrameNameByIndex(QModelIndex index);

    int getFasetCount() const;

    bool frameExists(QString frameName);
protected:
    int getFreeId();
signals:
    void sigValueExists(QModelIndex index,QVariant value);
    void sigDataChanged();

    void sigErrorWhileValidating(QString errorText);

    void sigIsaAdded(QString source,QString dest);
    void sigIsaChanged(QString source,QString oldDest,QString newDest);
    void sigIsaDeleted(QString source,QString dest);

    void sigApoAdded(QString source,QString dest);
    void sigApoChanged(QString oldSource,QString newSource,QString dest);
    void sigApoDeleted(QString source,QString dest);
    //Сигналы для сохранения целостности
    void sigFrameDeleted(QString domainName);

    void sigFrameNameChanged(QString oldName,QString newName);
public slots:

protected:
    NFrameNode* nodeFromIndex(const QModelIndex& index) const;

    NFrame* getFrameByName(QString name);
protected:
    QList<NFrame*> *frames;
    NFrameNode *rootNode;

    bool itemsIsEditable;
    /// Режим отображения, при котором сокращено количество столбцов
    bool m_simpleView;
    /// Текущий последний занятый id
    int m_freeId;

    int m_fasetCount;
};

#endif // NFRAMENETMODE_H
