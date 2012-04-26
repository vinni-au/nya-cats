#ifndef MLV_H
#define MLV_H

#include "core/nkbmanager.h"
#include "core/nframe.h"
#include "core/nslot.h"
#include "core/nfaset.h"
#include "core/nproc.h"
#include "visualize/visualizer.h"
#include "visualize/grid.h"

class MLV
{
protected:
    NKBManager* m_KBManager;
    NFrame* m_CurrFrame;
    Grid* m_Grid;

public:
    MLV(NKBManager* manager, Grid* grid);
    void Start(NFrame* frame);
    bool BindFrame(NFrame* frame);
    bool BindSlot(NSlot* slot);

protected:

};

#endif // MLV_H
