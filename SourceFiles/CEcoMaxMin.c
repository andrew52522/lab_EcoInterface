#include "IEcoSystem1.h"
#include "CEcoMaxMin.h"
#include "IEcoInterfaceBus1.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorC.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

/* Eco component implementation.  No standard C runtime functions are used. */
static int16_t ECOCALLMETHOD CEcoMaxMin_D17DEB17_QueryInterface(IEcoMaxMinPtr_t me, const UGUID* riid, void** ppv) {
    CEcoMaxMin_D17DEB17* pCMe;
    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    pCMe = (CEcoMaxMin_D17DEB17*)me;
    *ppv = 0;
    if (IsEqualUGUID(riid, &IID_IEcoMaxMin) || IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = (void*)&pCMe->m_pVTblIEcoMaxMin;
        ++pCMe->m_cRef;
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY) && pCMe->m_pInnerCalculatorY != 0) {
        *ppv = (void*)&pCMe->m_CalculatorY;
        ++pCMe->m_cRef;
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX) && pCMe->m_pInnerCalculatorX != 0) {
        *ppv = (void*)&pCMe->m_CalculatorX;
        ++pCMe->m_cRef;
        return ERR_ECO_SUCCESS;
    }
    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD CEcoMaxMin_D17DEB17_AddRef(IEcoMaxMinPtr_t me) {
    CEcoMaxMin_D17DEB17* pCMe;
    if (me == 0) { return 0; }
    pCMe = (CEcoMaxMin_D17DEB17*)me;
    return ++pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoMaxMin_D17DEB17_Release(IEcoMaxMinPtr_t me) {
    CEcoMaxMin_D17DEB17* pCMe;
    uint32_t nRef;
    if (me == 0) { return 0; }
    pCMe = (CEcoMaxMin_D17DEB17*)me;
    if (pCMe->m_cRef == 0) { return 0; }
    nRef = --pCMe->m_cRef;
    if (nRef == 0) { pCMe->Delete(pCMe); }
    return nRef;
}

/* The calculator interface belongs to the OUTER component.
 * Calls are forwarded to its contained Eco.CalculatorA instance. */
static int16_t ECOCALLMETHOD CEcoMaxMin_CalculatorX_QueryInterface(
    IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    CEcoMaxMinCalculatorXAdapter* pAdapter = (CEcoMaxMinCalculatorXAdapter*)me;
    if (pAdapter == 0 || pAdapter->m_pOwner == 0) { return ERR_ECO_POINTER; }
    return CEcoMaxMin_D17DEB17_QueryInterface(
        (IEcoMaxMin*)pAdapter->m_pOwner, riid, (void**)ppv);
}

static uint32_t ECOCALLMETHOD CEcoMaxMin_CalculatorX_AddRef(IEcoCalculatorXPtr_t me) {
    CEcoMaxMinCalculatorXAdapter* pAdapter = (CEcoMaxMinCalculatorXAdapter*)me;
    if (pAdapter == 0 || pAdapter->m_pOwner == 0) { return 0; }
    return CEcoMaxMin_D17DEB17_AddRef((IEcoMaxMin*)pAdapter->m_pOwner);
}

static uint32_t ECOCALLMETHOD CEcoMaxMin_CalculatorX_Release(IEcoCalculatorXPtr_t me) {
    CEcoMaxMinCalculatorXAdapter* pAdapter = (CEcoMaxMinCalculatorXAdapter*)me;
    if (pAdapter == 0 || pAdapter->m_pOwner == 0) { return 0; }
    return CEcoMaxMin_D17DEB17_Release((IEcoMaxMin*)pAdapter->m_pOwner);
}

static int32_t ECOCALLMETHOD CEcoMaxMin_CalculatorX_Addition(
    IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    CEcoMaxMinCalculatorXAdapter* pAdapter = (CEcoMaxMinCalculatorXAdapter*)me;
    IEcoCalculatorX* pInner;
    if (pAdapter == 0 || pAdapter->m_pOwner == 0) { return 0; }
    pInner = pAdapter->m_pOwner->m_pInnerCalculatorX;
    if (pInner == 0) { return 0; }
    return pInner->pVTbl->Addition(pInner, a, b);
}

static int16_t ECOCALLMETHOD CEcoMaxMin_CalculatorX_Subtraction(
    IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    CEcoMaxMinCalculatorXAdapter* pAdapter = (CEcoMaxMinCalculatorXAdapter*)me;
    IEcoCalculatorX* pInner;
    if (pAdapter == 0 || pAdapter->m_pOwner == 0) { return 0; }
    pInner = pAdapter->m_pOwner->m_pInnerCalculatorX;
    if (pInner == 0) { return 0; }
    return pInner->pVTbl->Subtraction(pInner, a, b);
}

IEcoCalculatorXVTbl g_xEcoMaxMinCalculatorXVTbl = {
    CEcoMaxMin_CalculatorX_QueryInterface,
    CEcoMaxMin_CalculatorX_AddRef,
    CEcoMaxMin_CalculatorX_Release,
    CEcoMaxMin_CalculatorX_Addition,
    CEcoMaxMin_CalculatorX_Subtraction
};


/* Second forwarding interface: multiplication/division from Eco.CalculatorE. */
static int16_t ECOCALLMETHOD CEcoMaxMin_CalculatorY_QueryInterface(
    IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    CEcoMaxMinCalculatorYAdapter* pAdapter = (CEcoMaxMinCalculatorYAdapter*)me;
    if (pAdapter == 0 || pAdapter->m_pOwner == 0) { return ERR_ECO_POINTER; }
    return CEcoMaxMin_D17DEB17_QueryInterface(
        (IEcoMaxMin*)pAdapter->m_pOwner, riid, (void**)ppv);
}
static uint32_t ECOCALLMETHOD CEcoMaxMin_CalculatorY_AddRef(IEcoCalculatorYPtr_t me) {
    CEcoMaxMinCalculatorYAdapter* pAdapter = (CEcoMaxMinCalculatorYAdapter*)me;
    if (pAdapter == 0 || pAdapter->m_pOwner == 0) { return 0; }
    return CEcoMaxMin_D17DEB17_AddRef((IEcoMaxMin*)pAdapter->m_pOwner);
}
static uint32_t ECOCALLMETHOD CEcoMaxMin_CalculatorY_Release(IEcoCalculatorYPtr_t me) {
    CEcoMaxMinCalculatorYAdapter* pAdapter = (CEcoMaxMinCalculatorYAdapter*)me;
    if (pAdapter == 0 || pAdapter->m_pOwner == 0) { return 0; }
    return CEcoMaxMin_D17DEB17_Release((IEcoMaxMin*)pAdapter->m_pOwner);
}
static int32_t ECOCALLMETHOD CEcoMaxMin_CalculatorY_Multiplication(
    IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    CEcoMaxMinCalculatorYAdapter* pAdapter = (CEcoMaxMinCalculatorYAdapter*)me;
    IEcoCalculatorY* pInner;
    if (pAdapter == 0 || pAdapter->m_pOwner == 0) { return 0; }
    pInner = pAdapter->m_pOwner->m_pInnerCalculatorY;
    if (pInner == 0) { return 0; }
    return pInner->pVTbl->Multiplication(pInner, a, b);
}
static int16_t ECOCALLMETHOD CEcoMaxMin_CalculatorY_Division(
    IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    CEcoMaxMinCalculatorYAdapter* pAdapter = (CEcoMaxMinCalculatorYAdapter*)me;
    IEcoCalculatorY* pInner;
    if (pAdapter == 0 || pAdapter->m_pOwner == 0) { return 0; }
    pInner = pAdapter->m_pOwner->m_pInnerCalculatorY;
    if (pInner == 0) { return 0; }
    return pInner->pVTbl->Division(pInner, a, b);
}
IEcoCalculatorYVTbl g_xEcoMaxMinCalculatorYVTbl = {
    CEcoMaxMin_CalculatorY_QueryInterface,
    CEcoMaxMin_CalculatorY_AddRef,
    CEcoMaxMin_CalculatorY_Release,
    CEcoMaxMin_CalculatorY_Multiplication,
    CEcoMaxMin_CalculatorY_Division
};

/* ACOM interface functions: these are intentionally independent of libc. */
static int ECOCALLMETHOD CEcoMaxMin_MaxInt(IEcoMaxMinPtr_t me, int a, int b) { (void)me; return a >= b ? a : b; }
static int ECOCALLMETHOD CEcoMaxMin_MinInt(IEcoMaxMinPtr_t me, int a, int b) { (void)me; return a <= b ? a : b; }
static long ECOCALLMETHOD CEcoMaxMin_MaxLong(IEcoMaxMinPtr_t me, long a, long b) { (void)me; return a >= b ? a : b; }
static long ECOCALLMETHOD CEcoMaxMin_MinLong(IEcoMaxMinPtr_t me, long a, long b) { (void)me; return a <= b ? a : b; }
static float ECOCALLMETHOD CEcoMaxMin_MaxFloat(IEcoMaxMinPtr_t me, float a, float b) { (void)me; return a >= b ? a : b; }
static float ECOCALLMETHOD CEcoMaxMin_MinFloat(IEcoMaxMinPtr_t me, float a, float b) { (void)me; return a <= b ? a : b; }
static double ECOCALLMETHOD CEcoMaxMin_MaxDouble(IEcoMaxMinPtr_t me, double a, double b) { (void)me; return a >= b ? a : b; }
static double ECOCALLMETHOD CEcoMaxMin_MinDouble(IEcoMaxMinPtr_t me, double a, double b) { (void)me; return a <= b ? a : b; }
static long double ECOCALLMETHOD CEcoMaxMin_MaxLongDouble(IEcoMaxMinPtr_t me, long double a, long double b) { (void)me; return a >= b ? a : b; }
static long double ECOCALLMETHOD CEcoMaxMin_MinLongDouble(IEcoMaxMinPtr_t me, long double a, long double b) { (void)me; return a <= b ? a : b; }

static int16_t ECOCALLMETHOD initCEcoMaxMin_D17DEB17(CEcoMaxMin_D17DEB17Ptr_t me, IEcoUnknownPtr_t pIUnkSystem) {
    CEcoMaxMin_D17DEB17* pCMe = (CEcoMaxMin_D17DEB17*)me;
    int16_t result;
    IEcoInterfaceBus1* pIBus = 0;
    if (pCMe == 0 || pIUnkSystem == 0) { return ERR_ECO_POINTER; }
    /* Keep our own reference until component deletion. */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem, (void**)&pCMe->m_pISys);
    if (result != 0 || pCMe->m_pISys == 0) { return ERR_ECO_NOSYSTEM; }

    pCMe->m_CalculatorX.pVTbl = &g_xEcoMaxMinCalculatorXVTbl;
    pCMe->m_CalculatorX.m_pOwner = pCMe;
    pCMe->m_CalculatorY.pVTbl = &g_xEcoMaxMinCalculatorYVTbl;
    pCMe->m_CalculatorY.m_pOwner = pCMe;
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys,
        &IID_IEcoInterfaceBus1, (void**)&pIBus);
    if (result != 0 || pIBus == 0) { return ERR_ECO_NOBUS; }

    /* Variant selected at compile time. Every option is a real ACOM component. */
#if defined(LAB2_BACKEND_C)
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorC,
        0, &IID_IEcoCalculatorX, (void**)&pCMe->m_pInnerCalculatorX);
#elif defined(LAB2_BACKEND_D)
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD,
        0, &IID_IEcoCalculatorX, (void**)&pCMe->m_pInnerCalculatorX);
#else
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorA,
        0, &IID_IEcoCalculatorX, (void**)&pCMe->m_pInnerCalculatorX);
#endif
    if (result != 0 || pCMe->m_pInnerCalculatorX == 0) {
        pIBus->pVTbl->Release(pIBus);
        return ERR_ECO_NOINTERFACE;
    }
    /* E aggregates B. Expose E's IEcoCalculatorY through our adapter. */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE,
        0, &IID_IEcoCalculatorY, (void**)&pCMe->m_pInnerCalculatorY);
    pIBus->pVTbl->Release(pIBus);
    if (result != 0 || pCMe->m_pInnerCalculatorY == 0) {
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESS;
}

static int16_t ECOCALLMETHOD createCEcoMaxMin_D17DEB17(CEcoMaxMin_D17DEB17Ptr_t me, IEcoUnknownPtr_t pIUnkSystem, IEcoUnknownPtr_t pIUnkOuter) {
    (void)pIUnkSystem;
    (void)pIUnkOuter;
    return me != 0 ? ERR_ECO_SUCCESS : ERR_ECO_POINTER;
}

static void ECOCALLMETHOD deleteCEcoMaxMin_D17DEB17(CEcoMaxMin_D17DEB17Ptr_t me) {
    IEcoMemoryAllocator1* pIMem;
    if (me == 0) { return; }
    if (me->m_pInnerCalculatorY != 0) {
        me->m_pInnerCalculatorY->pVTbl->Release(me->m_pInnerCalculatorY);
    }
    if (me->m_pInnerCalculatorX != 0) {
        me->m_pInnerCalculatorX->pVTbl->Release(me->m_pInnerCalculatorX);
    }
    if (me->m_pISys != 0) {
        me->m_pISys->pVTbl->Release(me->m_pISys);
    }
    pIMem = me->m_pIMem;
    if (pIMem != 0) {
        pIMem->pVTbl->Free(pIMem, me);
        pIMem->pVTbl->Release(pIMem);
    }
}

IEcoMaxMinVTbl g_x6547418E7924437D9216E54B952E0555VTbl_D17DEB17 = {
    CEcoMaxMin_D17DEB17_QueryInterface,
    CEcoMaxMin_D17DEB17_AddRef,
    CEcoMaxMin_D17DEB17_Release,
    CEcoMaxMin_MaxInt,
    CEcoMaxMin_MinInt,
    CEcoMaxMin_MaxLong,
    CEcoMaxMin_MinLong,
    CEcoMaxMin_MaxFloat,
    CEcoMaxMin_MinFloat,
    CEcoMaxMin_MaxDouble,
    CEcoMaxMin_MinDouble,
    CEcoMaxMin_MaxLongDouble,
    CEcoMaxMin_MinLongDouble
};

CEcoMaxMin_D17DEB17 g_xCEcoMaxMin_D17DEB17 = {
    &g_x6547418E7924437D9216E54B952E0555VTbl_D17DEB17,
    { &g_xEcoMaxMinCalculatorXVTbl, 0 },
    { &g_xEcoMaxMinCalculatorYVTbl, 0 },
    initCEcoMaxMin_D17DEB17,
    createCEcoMaxMin_D17DEB17,
    deleteCEcoMaxMin_D17DEB17,
    1,
    0,
    0,
    0,
    0
};
