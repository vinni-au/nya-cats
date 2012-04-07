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

signals:
    //Для диаграмм
    void frameAdded(unsigned id);

    void sigDataLoaded();
    void sigErrorWhileValidating(QString errorText);

public slots:
    ///От диаграмм
    void selectFrame(unsigned id);
    //
    void addFrame(QString name);
    void deleteFrame(unsigned id);
    //source id, destination id
    void addIsa(unsigned sid, unsigned did);
    void addApo(unsigned sid, unsigned did);

    void deleteLink(unsigned sid, unsigned did);
    void onDataChanged();
private:
    QList<NFrame*> m_frames;              //список фреймов
    QList<NProduction*> m_productions;    //список продукционных программок
    QList<NProc*> m_procs;                //спимок процедур на qscript
    DomainModel m_domainModel;            //модель доменов

    bool        dirty;                  //Признак того, что есть несохраненные изменения. true - если произведены изменения
    QFile       *file;                  //ссылка на файл БЗ
};

#endif // NKBMANAGER_H
