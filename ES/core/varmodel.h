#ifndef VARMODEL_H
#define VARMODEL_H

#include <QAbstractItemModel>
#include <QVariant>
#include "variable.h"
#include "varnode.h"
#include "domain.h"
#include <QDomDocument>
#include <QStringList>
#include <QMimeData>
#include "validator.h"

class VarModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit VarModel(QObject *parent=0);
    virtual ~VarModel();

    void setRootNode(VarNode *node);

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
    void setVariables( QList<Variable*> *variables);

    QModelIndex addVariable();
    void editVariable(QModelIndex var,QString newName);
    void deleteVariable(QModelIndex var);
    bool variableExists(QString varName);

    QModelIndex indexByName(QString varName);
    QDomElement toXml(QDomDocument& doc);
    void fromXml(QDomElement &vars);

    bool isValid();

    void setItemsIsEditable(bool isEditable);
public:
    void moveBefore(QModelIndex sourceInx,QModelIndex destInx);
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
signals:
    void sigValueExists(QModelIndex index,QVariant value);
    void sigDataChanged();
    //сигналы для сохранения целостности ЭС
    void sigVarNameChanged(QString oldName,QString newName);            //сигнал об изменении имени переменной
    void sigVarDeleted(QString varName);                                //сигнал об удалении переменной
    void sigVarDomainChanged(QString varName,QString oldDomain,QString newDomain);  //сигнал об изменении домена переменной

    void sigErrorWhileValidating(QString errorText);
public slots:
    void onDomainNameChanged(QString oldDomain,QString newDomain);      //при изменении имени домена
    void onDomainDeleted(QString domainName);                           //при удалении домена
private:
    VarNode* nodeFromIndex(const QModelIndex& index) const;
    QString generateNewUniqueVarName();
private:
    QList<Variable*> *variables;
    VarNode *rootNode;
    bool itemsIsEditable;
};

#endif // VARMODEL_H
