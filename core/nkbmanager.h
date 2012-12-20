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

    bool frameIsSystem(int id);
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


    //процедуры
    void        addProc(NProc* procedure);
    NProc*      getProc(QString name);
    bool        procExists(QString name);
    QStringList getProceduresNames();


    QStringList getVars(QString frameName);
    QStringList getVarsWithParents(QString frameName);

    QString getDomainByString( QString frameName, QString str  );
    NSlot * getSlotByString( QString frameName, QString str  );

    // Для МЛВ
    NFrame*         GetFrameByName      (QString name);
    NFrame*         GetFrameParent      (NFrame* frame);
    NFrame*         GetFrameInstance    (NFrame* frame);
    NFrame*         GetFrameInstance    (QString name);
    NFrame*         GetFrameInstanceWithParents(NFrame* frame);
    NFrame*         GetFrameInstanceWithParents(QString name);
    QList<NFrame*>  GetFrameChildren    (NFrame* frame);
    QList<NSlot*>   GetFrameSlots       (NFrame* frame);
    NSlot*          GetFrameSlot        (NFrame* frame, QString slotName);

    Domain*         GetSlotDomain       (NSlot* slot);
    QList<NFaset*>  GetSlotFasets       (NSlot* slot);
	bool			HasParentWithName	(NFrame* frame, QString parentName);

    QStringList getFilteredFrameList(QString frameName,QString slotName);
    bool slotExists(QString framename,QString slotName);

    void clearExemplarIds();

	QList<NFrame*> getAllChildren(NFrame* frame);//получает потомков и потомков потомков ... 
	bool hasChildren(NFrame* frame);

private:
    NFrame  *getFrameById(int id);
    NFrame  *getFrameByName(QString name);
    int      getFreeId();

    void deleteLinks(int id);
    int getFreeExemplarId();
signals:
    //Для диаграмм
    void frameAdded(unsigned id, QString name);
    void frameDeleted(unsigned id);
    void isaAdded(unsigned sid, unsigned did);
    void isaDeleted(unsigned sid, unsigned did);
    void apoAdded(unsigned sid, unsigned did);
    void apoDeleted(unsigned sid, unsigned did);

    void sigDataLoaded();
    void sigErrorWhileValidating(QString errorText);

    void sigFrameNameChanged(unsigned id,QString newName);
public slots:
    ///От диаграмм
    void selectFrame(unsigned id);
    //
    bool addFrame(QString name);
    bool addFrameSituation(QString name);
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

private slots:
    void addIsa(QString source, QString dest);
    void changeIsa(QString source, QString olddest, QString newDest);
    void deleteIsa(QString source, QString dest);

    void addApo(QString source, QString dest);
    void changeApo(QString oldSource, QString newSource, QString dest);
    void deleteApo(QString source,QString dest);

    void onFrameNetModelFrameNameChanged(QString oldName,QString newName);
private:
    QList<NFrame*>      m_frames;               //список фреймов
    NFramenetModel      *m_framenetModel;
    QList<NProduction*> m_productions;          //список продукционных программок
    QList<NProc*>       m_procs;                //спимок процедур на qscript
    DomainModel         m_domainModel;          //модель доменов

    QDomElement         m_diagramElement;       //данные для диаграммы

    bool        m_dirty;                        //Признак того, что есть несохраненные изменения. true - если произведены изменения
    QFile       *m_file;                        //ссылка на файл БЗ

    int m_frameExemplarMaxId;
public:
    QDomDocument doc;
};

#endif // NKBMANAGER_H
