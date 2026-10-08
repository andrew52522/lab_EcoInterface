#include "IEcoSystem1.h"
#include "CEcoMaxMin.h"

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
    if (pCMe == 0 || pIUnkSystem == 0) { return ERR_ECO_POINTER; }
    /* Keep our own reference until component deletion. */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem, (void**)&pCMe->m_pISys);
    if (result != 0 || pCMe->m_pISys == 0) { return ERR_ECO_NOSYSTEM; }
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
    initCEcoMaxMin_D17DEB17,
    createCEcoMaxMin_D17DEB17,
    deleteCEcoMaxMin_D17DEB17,
    1,
    0,
    0
};
