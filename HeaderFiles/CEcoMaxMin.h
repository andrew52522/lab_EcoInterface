#ifndef __C_ECOMAXMIN_H__
#define __C_ECOMAXMIN_H__

#include "IEcoMaxMin.h"
#include "IEcoConnectionPointContainer.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoMaxMin_D17DEB17* CEcoMaxMin_D17DEB17Ptr_t;

typedef struct CEcoMaxMin_D17DEB17 {
    IEcoMaxMinVTbl* m_pVTblIEcoMaxMin;
    int16_t (ECOCALLMETHOD *Init)(CEcoMaxMin_D17DEB17Ptr_t me, IEcoUnknownPtr_t pIUnkSystem);
    int16_t (ECOCALLMETHOD *Create)(CEcoMaxMin_D17DEB17Ptr_t me, IEcoUnknownPtr_t pIUnkSystem, IEcoUnknownPtr_t pIUnkOuter);
    void (ECOCALLMETHOD *Delete)(CEcoMaxMin_D17DEB17Ptr_t me);
    uint32_t m_cRef;
    IEcoMemoryAllocator1* m_pIMem;
    IEcoSystem1* m_pISys;
    IEcoConnectionPointContainer container;
    IEcoConnectionPoint point;
    IEcoUnknown* sink;
    uint32_t cookie;
} CEcoMaxMin_D17DEB17;

extern IEcoConnectionPointContainerVTbl CEcoMaxMin_ContainerVTbl;
extern IEcoConnectionPointVTbl CEcoMaxMin_PointVTbl;

/* Operand zero denotes OnCompare, operands one and two denote OnSelect. */
void CEcoMaxMin_Notify(CEcoMaxMin_D17DEB17* owner, uint16_t operation, uint16_t type, uint16_t operand);

#endif
