#ifndef MLV_H
#define MLV_H

#include "nkbmanager.h"

class MLV
{
protected:
    NKBManager* m_KBManager;

public:
    MLV(NKBManager* manager);
};

#endif // MLV_H
