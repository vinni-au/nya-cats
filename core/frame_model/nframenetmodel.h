#ifndef NFRAMENETMODE_H
#define NFRAMENETMODE_H

#include <QAbstractItemModel>
#include "nframenode.h"
#include "core/nframe.h"

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

    //virtual Qt::DropActions supportedDropActions() const;
    //мои
    void setFrames( QList<NFrame*> *frames);

    int getIdByIndex(QModelIndex index);

    void update();

    bool addSlot(QModelIndex& frameIndex);
    bool deleteSlot(QModelIndex& slotIndex);
    //QModelIndex addDomain();
    //void editDomain(QModelIndex domain,QString newName);
    //void deleteDomain(QModelIndex domain);
   // bool domainExists(QString domainName);

    //QModelIndex addValue(QModelIndex domainIndex);
    //void deleteValue(QModelIndex valueIndex);
    //bool valueExists(QString domainName,QString value);
    //bool valueExists(Domain *domain,QString value);

    //QModelIndex indexByName(QString domain);

    //QDomElement toXml(QDomDocument& doc);
    //void fromXml(QDomElement &domains);

    //bool isValid();
    //void setItemsIsEditable(bool isEditable);
private:
    int getFreeId();
signals:
    void sigValueExists(QModelIndex index,QVariant value);
    void sigDataChanged();

    void sigErrorWhileValidating(QString errorText);
    //Сигналы для сохранения целостности
    void sigFrameDeleted(QString domainName);                                          //удаление домена
public:
    //void moveBefore(QModelIndex sourceInx,QModelIndex destInx);
    //QStringList mimeTypes() const;
    //QMimeData *mimeData(const QModelIndexList &indexes) const;
    //bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
public slots:

private:
    NFrameNode* nodeFromIndex(const QModelIndex& index) const;
    //QString generateNewUniqueDomainName();
    //QString generateNewUniqueDomainValue(Domain* domain);
private:
    QList<NFrame*> *frames;
    NFrameNode *rootNode;

    bool itemsIsEditable;

    bool m_simpleView;

};

#endif // NFRAMENETMODE_H
