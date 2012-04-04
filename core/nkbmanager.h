#ifndef NKBMANAGER_H
#define NKBMANAGER_H

#include <QObject>
#include <QMap>

#include "nframe.h"
#include "nproc.h"
#include "domains/domainmodel.h"

//Менеджер базы знаний
//Управляет базой знаний через апи. Создание фреймов, слотов. Сохранение. Загрузка БЗ.
class NKBManager : public QObject
{
    Q_OBJECT
public:
    explicit NKBManager(QObject *parent = 0);

    QMap<unsigned, QString> frameNames();

    static bool SaveAs();
    static bool mayBeSave();
    static bool areUsure(QString quest);


signals:
    //Для диаграмм
    void frameAdded(unsigned id);

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

private:
    QList<NFrame*> frames;
    //QList<продукционная программа
    QList<NProc*> procs;
    DomainModel domainModel;
};

#endif // NKBMANAGER_H
