#ifndef __C_ECOMAXMIN_H__
#define __C_ECOMAXMIN_H__

#include "IEcoMaxMin.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoMaxMin_D17DEB17* CEcoMaxMin_D17DEB17Ptr_t;

/* Second interface: its vtable is at offset zero in the adapter. */
typedef struct CEcoMaxMinCalculatorXAdapter {
    IEcoCalculatorXVTbl* pVTbl;
    CEcoMaxMin_D17DEB17Ptr_t m_pOwner;
} CEcoMaxMinCalculatorXAdapter;

typedef struct CEcoMaxMinCalculatorYAdapter {
    IEcoCalculatorYVTbl* pVTbl;
    CEcoMaxMin_D17DEB17Ptr_t m_pOwner;
} CEcoMaxMinCalculatorYAdapter;

typedef struct CEcoMaxMin_D17DEB17 {
    IEcoMaxMinVTbl* m_pVTblIEcoMaxMin;
    CEcoMaxMinCalculatorXAdapter m_CalculatorX;
    CEcoMaxMinCalculatorYAdapter m_CalculatorY;
    int16_t (ECOCALLMETHOD *Init)(CEcoMaxMin_D17DEB17Ptr_t me, IEcoUnknownPtr_t pIUnkSystem);
    int16_t (ECOCALLMETHOD *Create)(CEcoMaxMin_D17DEB17Ptr_t me, IEcoUnknownPtr_t pIUnkSystem, IEcoUnknownPtr_t pIUnkOuter);
    void (ECOCALLMETHOD *Delete)(CEcoMaxMin_D17DEB17Ptr_t me);
    uint32_t m_cRef;
    IEcoMemoryAllocator1* m_pIMem;
    IEcoSystem1* m_pISys;
    /* Contained CalculatorA, owned until Delete(). */
    IEcoCalculatorX* m_pInnerCalculatorX;
    /* Eco.CalculatorE is an inner component aggregating Eco.CalculatorB. */
    IEcoCalculatorY* m_pInnerCalculatorY;
} CEcoMaxMin_D17DEB17;

#endif
