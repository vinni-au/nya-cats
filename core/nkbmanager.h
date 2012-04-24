#ifndef NKBMANAGER_H
#define NKBMANAGER_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QMessageBox>
#include <qfiledialog.h>
#include <QDebug>

#include "nframe.h"
#include "nproc.h"
#include "domains/domainmodel.h"
#include "production/nproduction.h"
#include "./frame_model/nframenetmodel.h"

//Менеджер базы знаний
//Управляет базой знаний через апи. Создание фреймов, слотов. Сохранение. Загрузка БЗ.
class NKBManager : public QObject
{
    Q_OBJECT
public:
    explicit NKBManager(QObject *parent = 0);

    QMap<unsigned, QString> frameNames();

    bool SaveAs();
    bool mayBeSave();
    bool areUsure(QString quest);

    void saveToXml(QTextStream &stream);
    void readFromXml(QFile &file);
    bool saved();
    void save();
    bool saveAs(QString filePath);
    bool Open(QString filePath);
    void Clear();

    bool isValid();
    bool reload();

    bool frameExists(QString name);

    NFramenetModel  *getFrameNetModel();
    DomainModel     *getDomainModel();

    unsigned getIdByIndex(QModelIndex);

    QDomElement diagramNode();
    void        setDiagramNode(QDomElement node);

    bool productionExists(QString name);
    NProduction *getProduction(QString name);

    void reloadProduction(NProduction *production);
    void addProduction(NProduction *prod);
    QStringList getProductionNames();
    QStringList getProceduresNames();
private:
    NFrame  *getFrameById(int id);
    int     getFreeId();

    void deleteLinks(int id);


signals:
    //Для диаграмм
    void frameAdded(unsigned id, QString name);
    void frameDeleted(unsigned id);

    void sigDataLoaded();
    void sigErrorWhileValidating(QString errorText);

public slots:
    ///От диаграмм
    void selectFrame(unsigned id);
    //
    bool addFrame(QString name);
    bool deleteFrame(unsigned id);
    //source id, destination id
    bool addIsa(unsigned sid, unsigned did);
    bool addApo(unsigned sid, unsigned did);

    bool deleteIsa(unsigned sid, unsigned did);
    bool deleteApo(unsigned sid, unsigned did);

    void onDataChanged();

    void setDirty(bool dirty);

    QFile* file();
    void setFile(QFile *file);
private:
    QList<NFrame*>      m_frames;               //список фреймов
    NFramenetModel      *m_framenetModel;
    QList<NProduction*> m_productions;          //список продукционных программок
    QList<NProc*>       m_procs;                //спимок процедур на qscript
    DomainModel         m_domainModel;          //модель доменов

    QDomElement         m_diagramElement;       //данные для диаграммы

    bool        m_dirty;                        //Признак того, что есть несохраненные изменения. true - если произведены изменения
    QFile       *m_file;                        //ссылка на файл БЗ

public:
    QDomDocument doc;
};

#endif // NKBMANAGER_H
