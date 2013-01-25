#ifndef NFRAMENETMODELHIERARCHICAL_H
#define NFRAMENETMODELHIERARCHICAL_H

#include "nframenetmodel.h"
#include <QDebug>

/**
  @class NFramenetModelHierarchical
  Иерархическая модель фреймов для КО
  */
class NFramenetModelHierarchical : public NFramenetModel
{
    Q_OBJECT
public:
    explicit NFramenetModelHierarchical(QObject *parent = 0);
    ~NFramenetModelHierarchical();

    virtual void setFrames( QList<NFrame*> *frames);
    void AddSubFrameNode(NFrame* frame,NFrame* subframe,NFrameNode::Type type,NFrameNode *destFrameNode);

signals:

public slots:

};

#endif // NFRAMENETMODELHIERARCHICAL_H
