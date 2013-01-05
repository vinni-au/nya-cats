#include "nframenetmodelhierarchical.h"

NFramenetModelHierarchical::NFramenetModelHierarchical(QObject *parent) :
    NFramenetModel(parent)
{
}

void
NFramenetModelHierarchical::setFrames( QList<NFrame *> *frames )
{
    qDebug()<<"NFramenetModelHierarchical::setFrames";
    //здесь делаем разбор списка фреймов
    //строим на основе списка дерево DomainNode

    //this->frames->clear();

    if(frames->count()>0)
    {
        m_fasetCount = frames->at(0)->getSlotByName("name")->fasetCount();
    }

    this->frames = frames;
    rootNode->children.clear();



    NFrame *frame;

    foreach(frame,*frames)
    {
        NFrameNode *frameNode = new NFrameNode(NFrameNode::FrameName,frame,true,rootNode);
        //слоты
        NSlot *slot;
        NFrameNode *slotNode;
        for(int i=0;i<frame->slotCount();i++)
        {
            slot = frame->getSlotByIndex(i);

            QString slotType = slot->getSlotType();
            if(slotType=="frame")
            {
                //субфрейм или is_a
                QString frameName = slot->defValue().toString();
                long long frameAddr = slot->getFasetByName("value")->value().toLongLong();
                //NFrame *subfr = this->getFrameByName(frameName);
                NFrame *subfr = (NFrame *)frameAddr;
                //NFrame *subfr = this->getFrameByName(frameName);
                if(subfr && frameName.trimmed()!="")
                {
                    AddSubFrameNode(frame,subfr,NFrameNode::Faset,frameNode);
                    //slotNode = new NFrameNode(NFrameNode::FrameName,subfr,true,frameNode);
                }
                else
                {
                    slotNode = new NFrameNode(NFrameNode::Faset,frame,true,frameNode);
                }
            }
            else
            {
                slotNode = new NFrameNode(NFrameNode::Faset,frame,true,frameNode);
            }
        }
    }
    reset();
    emit sigDataChanged();

}
void NFramenetModelHierarchical::AddSubFrameNode(NFrame* frame,NFrame* subframe,NFrameNode::Type type,NFrameNode *destFrameNode)
{
    NFrameNode *frameNode = new NFrameNode(type,frame,true,destFrameNode);
    //слоты
    NSlot *slot;
    NFrameNode *slotNode;
    for(int i=0;i<subframe->slotCount();i++)
    {
        slot = subframe->getSlotByIndex(i);

        QString slotType = slot->getSlotType();
        if(slotType=="frame")
        {
            //субфрейм или is_a
            QString frameName = slot->defValue().toString();
            long long frameAddr = slot->getFasetByName("value")->value().toLongLong();
            //NFrame *subfr = this->getFrameByName(frameName);
            NFrame *subfr = (NFrame *)frameAddr;
            if(subfr && frameName.trimmed()!="")
            {
                AddSubFrameNode(subframe,subfr,NFrameNode::Faset,frameNode);
                //slotNode = new NFrameNode(NFrameNode::FrameName,subfr,true,frameNode);
            }
            else
            {
                slotNode = new NFrameNode(NFrameNode::Faset,subframe,true,frameNode);
            }
        }
        else
        {
            slotNode = new NFrameNode(NFrameNode::Faset,subframe,true,frameNode);
        }


    }
}
