#ifndef DATAMODELS_H
#define DATAMODELS_H

#include "domainmodel.h"
#include "varmodel.h"
#include "rulemodel.h"
#include <QTextStream>
#include <QDomDocument>

class RuleModel;

class DataModels: public QObject
{
    Q_OBJECT
private:
    static DataModels *theSingleInstance;//=NULL
public:
    explicit DataModels(QObject *parent = 0);
    ~DataModels();

    static DataModels* instance()
    {
        if(!theSingleInstance)
            theSingleInstance=new DataModels();
        return theSingleInstance;
    }

    void saveToXml(QTextStream &stream);
    void readFromXml(QFile &file);
    bool saved();
    void save();
    bool saveAs(QString filePath);
    bool Open(QString filePath);
    void Clear();

    bool isValid();
    bool reload();
public slots:
    void onDataChanged();

signals:
    void sigDataLoaded();
    void sigErrorWhileValidating(QString errorText);
public:
    DomainModel *domainModel;
    VarModel    *varModel;
    RuleModel   *ruleModel;

    bool        dirty;          //true - если произведены изменения
    QFile       *file;
};

#endif // DATAMODELS_H
