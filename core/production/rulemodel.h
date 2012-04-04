#ifndef RULEMODEL_H
#define RULEMODEL_H

#include <QAbstractItemModel>
#include "rulenode.h"
#include "rule.h"
#include "expr.h"
#include <QVariant>
#include <QDomDocument>
#include <QStringList>
#include "../validator.h"
#include <QMimeData>


class RuleModel : public QAbstractItemModel
{
    Q_OBJECT
public:
enum ExprPart {ExprVar,ExprSign,ExprVal};
public:
    explicit RuleModel(QObject *parent=0);
    virtual ~RuleModel();

    void setRootNode(RuleNode *node);

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

    /////////////////////////
    void setRules( QList<Rule*> *rules);

    bool ruleExists(QString ruleName);

    QModelIndex addRule();
    QModelIndex insertRule(QModelIndex afterInddex);
    void deleteRule(QModelIndex index);

    QModelIndex addIf(QModelIndex ruleIndex);
    void deleteIf(QModelIndex index);

    QModelIndex addThen(QModelIndex ruleIndex);
    void deleteThen(QModelIndex index);

    QString getRuleName(QModelIndex index);

    QString getExprPart(QModelIndex index,RuleModel::ExprPart exprPart);

    QDomElement toXml(QDomDocument& doc);
    void fromXml(QDomElement &rules);
    QModelIndex indexByName(QString ruleName);

    QList<Rule*>* getRules();

    bool isValid();
public:
    void moveBefore(QModelIndex sourceInx,QModelIndex destInx);
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
signals:
    void sigValueExists(QModelIndex index,QVariant value);
    void sigDataChanged();
    void sigErrorWhileValidating(QString errorText);
public slots:
    //реакция на сигналы изменения данных
    void onVarDeleted(QString varName);
    void onVarNameChanged(QString oldName,QString newName);
    void onVarDomainChanged(QString varName,QString oldDomain,QString newDomain);
    void onDomainValueDeleted(QString domainName,QString value);
    void onDomainValueChanged(QString domainName,QString oldValue,QString newValue);
private:
    RuleNode* nodeFromIndex(const QModelIndex& index) const;
    QString generateNewUniqueRuleName();
private:
    QList<Rule*> *rules;
    RuleNode *rootNode;


};

#endif // RULEMODEL_H
