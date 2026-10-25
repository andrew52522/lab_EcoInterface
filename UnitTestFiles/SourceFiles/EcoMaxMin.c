/* ACOM console UnitTest for the Eco.MaxMin component. */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoMaxMin.h"
#include "IdEcoCalculatorA.h"

/* All cases use the component through its interface (not direct calls). */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoMaxMin* pIMaxMin = 0;
    IEcoCalculatorX* pIX = 0;
    IEcoMaxMin* pIMaxMinAgain = 0;
    IEcoUnknown* pIUnkFromMax = 0;
    IEcoUnknown* pIUnkFromX = 0;
    int16_t result;
    int16_t status = 0;

    if (pIUnk == 0) { return 10; }
    result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void**)&pISys);
    if (result != 0 || pISys == 0) { return 11; }
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void**)&pIBus);
    if (result != 0 || pIBus == 0) { status = 12; goto Release; }

#ifdef ECO_LIB
    /* The external component owns an instance of CalculatorA. */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorA,
         (IEcoUnknown*)GetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E);
    if (result != 0) { status = 15; goto Release; }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoMaxMin,
         (IEcoUnknown*)GetIEcoComponentFactoryPtr_48749720D2434B819DCFE025D17DEB17);
    if (result != 0) { status = 13; goto Release; }
#endif

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMaxMin, 0, &IID_IEcoMaxMin, (void**)&pIMaxMin);
    if (result != 0 || pIMaxMin == 0) { status = 14; goto Release; }

    if (pIMaxMin->pVTbl->MaxInt(pIMaxMin, -5, 9) != 9) { status = 21; goto Release; }
    if (pIMaxMin->pVTbl->MinInt(pIMaxMin, -5, 9) != -5) { status = 22; goto Release; }
    if (pIMaxMin->pVTbl->MaxInt(pIMaxMin, 7, 7) != 7) { status = 23; goto Release; }
    if (pIMaxMin->pVTbl->MinInt(pIMaxMin, 7, 7) != 7) { status = 24; goto Release; }
    if (pIMaxMin->pVTbl->MaxLong(pIMaxMin, -1234567891L, 1234567890L) != 1234567890L) { status = 25; goto Release; }
    if (pIMaxMin->pVTbl->MinLong(pIMaxMin, -1234567891L, 1234567890L) != -1234567891L) { status = 26; goto Release; }
    if (pIMaxMin->pVTbl->MaxFloat(pIMaxMin, -1.25f, 5.5f) != 5.5f) { status = 27; goto Release; }
    if (pIMaxMin->pVTbl->MinFloat(pIMaxMin, -1.25f, 5.5f) != -1.25f) { status = 28; goto Release; }
    if (pIMaxMin->pVTbl->MaxDouble(pIMaxMin, 9.5, -7.125) != 9.5) { status = 29; goto Release; }
    if (pIMaxMin->pVTbl->MinDouble(pIMaxMin, 9.5, -7.125) != -7.125) { status = 30; goto Release; }
    if (pIMaxMin->pVTbl->MaxLongDouble(pIMaxMin, 2.75L, 2.75L) != 2.75L) { status = 31; goto Release; }
    if (pIMaxMin->pVTbl->MinLongDouble(pIMaxMin, 2.75L, 2.75L) != 2.75L) { status = 32; goto Release; }

    /* IEcoCalculatorX is provided by Eco.MaxMin (not by a direct query to A). */
    result = pIMaxMin->pVTbl->QueryInterface(pIMaxMin, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result != 0 || pIX == 0) { status = 33; goto Release; }
    if (pIX->pVTbl->Addition(pIX, 3, 5) != 8) { status = 34; goto Release; }
    if (pIX->pVTbl->Subtraction(pIX, 3, 5) != -2) { status = 35; goto Release; }

    /* QueryInterface must work in both directions. */
    result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoMaxMin, (void**)&pIMaxMinAgain);
    if (result != 0 || pIMaxMinAgain == 0) { status = 36; goto Release; }
    if (pIMaxMinAgain->pVTbl->MaxInt(pIMaxMinAgain, 4, 7) != 7) { status = 37; goto Release; }

    /* Both interfaces belong to one external ACOM object. */
    result = pIMaxMin->pVTbl->QueryInterface(pIMaxMin, &IID_IEcoUnknown, (void**)&pIUnkFromMax);
    if (result != 0 || pIUnkFromMax == 0) { status = 38; goto Release; }
    result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoUnknown, (void**)&pIUnkFromX);
    if (result != 0 || pIUnkFromX != pIUnkFromMax) { status = 39; goto Release; }

Release:
    if (pIUnkFromX != 0) { pIUnkFromX->pVTbl->Release(pIUnkFromX); }
    if (pIUnkFromMax != 0) { pIUnkFromMax->pVTbl->Release(pIUnkFromMax); }
    if (pIMaxMinAgain != 0) { pIMaxMinAgain->pVTbl->Release(pIMaxMinAgain); }
    if (pIX != 0) { pIX->pVTbl->Release(pIX); }
    if (pIMaxMin != 0) { pIMaxMin->pVTbl->Release(pIMaxMin); }
    if (pIBus != 0) { pIBus->pVTbl->Release(pIBus); }
    if (pISys != 0) { pISys->pVTbl->Release(pISys); }
    return status;
}
