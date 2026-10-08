#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoMaxMin.h"
#include "CEcoMaxMinFactory.h"

extern CEcoMaxMin_D17DEB17 g_xCEcoMaxMin_D17DEB17;

static int16_t ECOCALLMETHOD CEcoMaxMin_Factory_QueryInterface(IEcoComponentFactory* me, const UGUID* riid, void** ppv) {
    if (me == 0 || riid == 0 || ppv == 0) { return ERR_ECO_POINTER; }
    *ppv = 0;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown) || IsEqualUGUID(riid, &IID_IEcoComponentFactory)) {
        *ppv = me;
        me->pVTbl->AddRef(me);
        return ERR_ECO_SUCCESS;
    }
    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD CEcoMaxMin_Factory_AddRef(IEcoComponentFactory* me) {
    CEcoMaxMin_D17DEB17Factory* pCMe = (CEcoMaxMin_D17DEB17Factory*)me;
    if (pCMe == 0) { return 0; }
    return ++pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoMaxMin_Factory_Release(IEcoComponentFactory* me) {
    CEcoMaxMin_D17DEB17Factory* pCMe = (CEcoMaxMin_D17DEB17Factory*)me;
    if (pCMe == 0 || pCMe->m_cRef == 0) { return 0; }
    return --pCMe->m_cRef;
}

static int16_t ECOCALLMETHOD CEcoMaxMin_Factory_Alloc(IEcoComponentFactory* me, IEcoUnknown* pIUnkSystem, IEcoUnknown* pIUnkOuter, const UGUID* riid, void** ppv) {
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoMaxMin_D17DEB17* pCObj = 0;
    const UGUID* pMemoryCID = &CID_EcoMemoryManager1;
    int16_t result;

    if (me == 0 || pIUnkSystem == 0 || riid == 0 || ppv == 0) { return ERR_ECO_POINTER; }
    *ppv = 0;
    if (pIUnkOuter != 0) { return ERR_ECO_NOAGGREGATION; }

    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem, (void**)&pISys);
    if (result != 0 || pISys == 0) { return ERR_ECO_NOSYSTEM; }

    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void**)&pIBus);
    if (result != 0 || pIBus == 0) {
        pISys->pVTbl->Release(pISys);
        return ERR_ECO_NOBUS;
    }

    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        pMemoryCID = pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
        if (pMemoryCID == 0) { pMemoryCID = &CID_EcoMemoryManager1; }
    }

    result = pIBus->pVTbl->QueryComponent(pIBus, pMemoryCID, 0, &IID_IEcoMemoryAllocator1, (void**)&pIMem);
    if (result != 0 || pIMem == 0) {
        pIBus->pVTbl->Release(pIBus);
        pISys->pVTbl->Release(pISys);
        return ERR_ECO_GET_MEMORY_ALLOCATOR;
    }

    pCObj = (CEcoMaxMin_D17DEB17*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoMaxMin_D17DEB17));
    if (pCObj == 0) {
        pIMem->pVTbl->Release(pIMem);
        pIBus->pVTbl->Release(pIBus);
        pISys->pVTbl->Release(pISys);
        return ERR_ECO_OUTOFMEMORY;
    }
    pCObj = (CEcoMaxMin_D17DEB17*)pIMem->pVTbl->Copy(pIMem, pCObj, &g_xCEcoMaxMin_D17DEB17, sizeof(CEcoMaxMin_D17DEB17));
    pCObj->m_pIMem = pIMem; /* Ownership moves into the component instance. */

    result = pCObj->Create(pCObj, pIUnkSystem, pIUnkOuter);
    if (result == 0) { result = pCObj->Init(pCObj, pIUnkSystem); }
    if (result == 0) {
        result = ((IEcoMaxMin*)pCObj)->pVTbl->QueryInterface((IEcoMaxMin*)pCObj, riid, ppv);
    }

    /* The initial reference belongs to the factory.  The caller owns the QI reference. */
    ((IEcoMaxMin*)pCObj)->pVTbl->Release((IEcoMaxMin*)pCObj);
    pIBus->pVTbl->Release(pIBus);
    pISys->pVTbl->Release(pISys);
    return result;
}

static int16_t ECOCALLMETHOD CEcoMaxMin_Factory_Init(IEcoComponentFactory* me, IEcoUnknown* pIUnkSystem, void* pv) {
    (void)pIUnkSystem; (void)pv;
    return me != 0 ? ERR_ECO_SUCCESS : ERR_ECO_POINTER;
}

static char_t* ECOCALLMETHOD CEcoMaxMin_Factory_get_Name(IEcoComponentFactory* me) {
    CEcoMaxMin_D17DEB17Factory* pCMe = (CEcoMaxMin_D17DEB17Factory*)me;
    return pCMe != 0 ? pCMe->m_Name : 0;
}
static char_t* ECOCALLMETHOD CEcoMaxMin_Factory_get_Version(IEcoComponentFactory* me) {
    CEcoMaxMin_D17DEB17Factory* pCMe = (CEcoMaxMin_D17DEB17Factory*)me;
    return pCMe != 0 ? pCMe->m_Version : 0;
}
static char_t* ECOCALLMETHOD CEcoMaxMin_Factory_get_Manufacturer(IEcoComponentFactory* me) {
    CEcoMaxMin_D17DEB17Factory* pCMe = (CEcoMaxMin_D17DEB17Factory*)me;
    return pCMe != 0 ? pCMe->m_Manufacturer : 0;
}

IEcoComponentFactoryVTbl g_x48749720D2434B819DCFE025D17DEB17FactoryVTbl = {
    CEcoMaxMin_Factory_QueryInterface,
    CEcoMaxMin_Factory_AddRef,
    CEcoMaxMin_Factory_Release,
    CEcoMaxMin_Factory_Alloc,
    CEcoMaxMin_Factory_Init,
    CEcoMaxMin_Factory_get_Name,
    CEcoMaxMin_Factory_get_Version,
    CEcoMaxMin_Factory_get_Manufacturer
};

CEcoMaxMin_D17DEB17Factory g_x48749720D2434B819DCFE025D17DEB17Factory = {
    &g_x48749720D2434B819DCFE025D17DEB17FactoryVTbl,
    0,
    "EcoMaxMin",
    "1.0.0.0",
    ""
};

#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr() {
    return (IEcoComponentFactory*)&g_x48749720D2434B819DCFE025D17DEB17Factory;
}
#elif defined(ECO_LIB)
IEcoComponentFactory* GetIEcoComponentFactoryPtr_48749720D2434B819DCFE025D17DEB17 = (IEcoComponentFactory*)&g_x48749720D2434B819DCFE025D17DEB17Factory;
#endif
