#ifndef NFRAMENODE_H
#define NFRAMENODE_H

#include <QObject>
#include "../nframe.h"

class NFrameNode : public QObject
{
    Q_OBJECT
public:
    //тип вершины
    enum Type {Root,FrameName,Faset};
    explicit NFrameNode(Type type,NFrame *frame,bool createBackLink,NFrameNode *parent = 0);
    ~NFrameNode();

signals:
public slots:
public:
    Type type;
    NFrame *frame;

    NFrameNode *parent;
    QList<NFrameNode*> children;
private:
    bool backLinkCreated;
};

#endif // NFRAMENODE_H
