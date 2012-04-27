#ifndef DOMAINMODEL_H
#define DOMAINMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include "domainnode.h"
#include "domain.h"
#include <QVector>
#include <QList>
#include <QtXml>
#include <QDomDocument>
#include <QVariant>

#include "../validator.h"

class DomainModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DomainModel(QObject *parent=0);
    virtual ~DomainModel();

    void setRootNode(DomainNode *node);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;

    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual bool removeRow(int row, const QModelIndex &parent);
    virtual bool insertRow(int row, const QModelIndex &parent);

    virtual Qt::DropActions supportedDropActions() const;
    //мои
    void setDomains( QList<Domain*> *domains);

    QModelIndex addDomain();
    void editDomain(QModelIndex domain,QString newName);
    void deleteDomain(QModelIndex domain);
    bool domainExists(QString domainName);

    QModelIndex addValue(QModelIndex domainIndex);
    void deleteValue(QModelIndex valueIndex);
    bool valueExists(QString domainName,QString value);
    bool valueExists(Domain *domain,QString value);

    QModelIndex indexByName(QString domain);

    QDomElement toXml(QDomDocument& doc);
    void fromXml(QDomElement &domains);

    bool isValid();
    void setItemsIsEditable(bool isEditable);

    Domain* getDomainByName(QString name);
signals:
    void sigValueExists(QModelIndex index,QVariant value);
    void sigDataChanged();

    void sigErrorWhileValidating(QString errorText);
    //Сигналы для сохранения целостности
    void sigDomainDeleted(QString domainName);                                          //удаление домена
    void sigDomainNameChanged(QString oldName,QString newName);                         //при изменении имени домена
    void sigDomainValueChanged(QString domainName,QString oldValue,QString newValue);   //изменение значения домена
    void sigDomainValueDeleted(QString domainName,QString value);
public:
    void moveBefore(QModelIndex sourceInx,QModelIndex destInx);
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
public slots:

private:
    DomainNode* nodeFromIndex(const QModelIndex& index) const;
    QString generateNewUniqueDomainName();
    QString generateNewUniqueDomainValue(Domain* domain);
private:
    QList<Domain*> *domains;
    DomainNode *rootNode;

    bool itemsIsEditable;

};

#endif // DOMAINMODEL_H
