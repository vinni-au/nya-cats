#ifndef SAVER_H
#define SAVER_H

#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include "core/datamodels.h"

class Saver : public QObject
{
    Q_OBJECT
public:
    explicit Saver(QObject *parent = 0);

signals:

public slots:

public:
    static bool SaveAs();
    static bool mayBeSave();
    static bool areUsure(QString quest);

};

#endif // SAVER_H
