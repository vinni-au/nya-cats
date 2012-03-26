#ifndef NFRAMENETMODE_H
#define NFRAMENETMODE_H

#include <QAbstractItemModel>

class NFramenetModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit NFramenetModel(QObject *parent = 0);

signals:

public slots:

};

#endif // NFRAMENETMODE_H
