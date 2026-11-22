#include <stddef.h>
#include "CEcoMaxMin.h"
#include "IEcoEnumConnectionPoints.h"
#include "IEcoEnumConnections.h"
#include "IEcoMaxMinEvents.h"

/* Each enumerator owns its item (if present) and the allocator. */
typedef struct PointEnum {
    IEcoEnumConnectionPoints iface;
    IEcoMemoryAllocator1* allocator;
    uint32_t refs;
    int consumed;
    IEcoConnectionPoint* item;
} PointEnum;

typedef struct ConnectionEnum {
    IEcoEnumConnections iface;
    IEcoMemoryAllocator1* allocator;
    uint32_t refs;
    int consumed;
    EcoConnectionData item;
} ConnectionEnum;

/* A partial fetch or skip consumes the available item. */
static int TakeItem(int available, int* consumed, uint32_t count) {
    if (count == 0 || *consumed || !available) { return 0; }
    *consumed = 1;
    return 1;
}

static int16_t NewPointEnum(IEcoMemoryAllocator1* allocator, IEcoConnectionPoint* item,
                            int consumed, IEcoEnumConnectionPoints** out);

static int16_t ECOCALLMETHOD PointEnum_QueryInterface(IEcoEnumConnectionPoints* me, const UGUID* iid, voidptr_t* out) {
    if (out == 0) { return ERR_ECO_POINTER; }
    *out = 0;
    if (me == 0 || iid == 0) { return ERR_ECO_POINTER; }
    if (!IsEqualUGUID(iid, &IID_IEcoEnumConnectionPoints) && !IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        return ERR_ECO_NOINTERFACE;
    }
    me->pVTbl->AddRef(me);
    *out = me;
    return ERR_ECO_SUCCESS;
}

static uint32_t ECOCALLMETHOD PointEnum_AddRef(IEcoEnumConnectionPoints* me) {
    return ++((PointEnum*)me)->refs;
}

static uint32_t ECOCALLMETHOD PointEnum_Release(IEcoEnumConnectionPoints* me) {
    PointEnum* self = (PointEnum*)me;
    IEcoMemoryAllocator1* allocator = self->allocator;
    uint32_t refs = --self->refs;
    if (refs == 0) {
        if (self->item != 0) { self->item->pVTbl->Release(self->item); }
        allocator->pVTbl->Free(allocator, self);
        allocator->pVTbl->Release(allocator);
    }
    return refs;
}

static int16_t ECOCALLMETHOD PointEnum_Next(IEcoEnumConnectionPoints* me, uint32_t count,
                                      IEcoConnectionPoint** out, uint32_t* fetched) {
    PointEnum* self = (PointEnum*)me;
    uint32_t taken;
    if (fetched != 0) { *fetched = 0; }
    if (count == 0) { return ERR_ECO_SUCCESS; }
    if (out == 0 || (count > 1 && fetched == 0)) { return ERR_ECO_POINTER; }
    taken = TakeItem(self->item != 0, &self->consumed, count);
    if (taken != 0) {
        self->item->pVTbl->AddRef(self->item);
        *out = self->item;
    }
    if (fetched != 0) { *fetched = taken; }
    return taken == count ? ERR_ECO_SUCCESS : ERR_ECO_FAIL;
}

static int16_t ECOCALLMETHOD PointEnum_Skip(IEcoEnumConnectionPoints* me, uint32_t count) {
    PointEnum* self = (PointEnum*)me;
    uint32_t taken = TakeItem(self->item != 0, &self->consumed, count);
    return taken == count ? ERR_ECO_SUCCESS : ERR_ECO_FAIL;
}

static int16_t ECOCALLMETHOD PointEnum_Reset(IEcoEnumConnectionPoints* me) {
    ((PointEnum*)me)->consumed = 0;
    return ERR_ECO_SUCCESS;
}

static int16_t ECOCALLMETHOD PointEnum_Clone(IEcoEnumConnectionPoints* me, IEcoEnumConnectionPoints** out) {
    PointEnum* self = (PointEnum*)me;
    return NewPointEnum(self->allocator, self->item, self->consumed, out);
}

static IEcoEnumConnectionPointsVTbl PointEnum_VTbl = {
    PointEnum_QueryInterface, PointEnum_AddRef, PointEnum_Release,
    PointEnum_Next, PointEnum_Skip, PointEnum_Reset, PointEnum_Clone
};

static int16_t NewPointEnum(IEcoMemoryAllocator1* allocator, IEcoConnectionPoint* item,
                            int consumed, IEcoEnumConnectionPoints** out) {
    PointEnum* self;
    if (out == 0) { return ERR_ECO_POINTER; }
    *out = 0;
    self = allocator->pVTbl->Alloc(allocator, sizeof(*self));
    if (self == 0) { return ERR_ECO_OUTOFMEMORY; }
    self->iface.pVTbl = &PointEnum_VTbl;
    self->allocator = allocator;
    self->refs = 1;
    self->consumed = consumed;
    self->item = item;
    allocator->pVTbl->AddRef(allocator);
    if (item != 0) { item->pVTbl->AddRef(item); }
    *out = &self->iface;
    return ERR_ECO_SUCCESS;
}

static int16_t NewConnectionEnum(IEcoMemoryAllocator1* allocator, EcoConnectionData item,
                            int consumed, IEcoEnumConnections** out);

static int16_t ECOCALLMETHOD ConnectionEnum_QueryInterface(IEcoEnumConnections* me, const UGUID* iid, voidptr_t* out) {
    if (out == 0) { return ERR_ECO_POINTER; }
    *out = 0;
    if (me == 0 || iid == 0) { return ERR_ECO_POINTER; }
    if (!IsEqualUGUID(iid, &IID_IEcoEnumConnections) && !IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        return ERR_ECO_NOINTERFACE;
    }
    me->pVTbl->AddRef(me);
    *out = me;
    return ERR_ECO_SUCCESS;
}

static uint32_t ECOCALLMETHOD ConnectionEnum_AddRef(IEcoEnumConnections* me) {
    return ++((ConnectionEnum*)me)->refs;
}

static uint32_t ECOCALLMETHOD ConnectionEnum_Release(IEcoEnumConnections* me) {
    ConnectionEnum* self = (ConnectionEnum*)me;
    IEcoMemoryAllocator1* allocator = self->allocator;
    uint32_t refs = --self->refs;
    if (refs == 0) {
        if (self->item.pUnk != 0) { self->item.pUnk->pVTbl->Release(self->item.pUnk); }
        allocator->pVTbl->Free(allocator, self);
        allocator->pVTbl->Release(allocator);
    }
    return refs;
}

static int16_t ECOCALLMETHOD ConnectionEnum_Next(IEcoEnumConnections* me, uint32_t count,
                                      EcoConnectionData* out, uint32_t* fetched) {
    ConnectionEnum* self = (ConnectionEnum*)me;
    uint32_t taken;
    if (fetched != 0) { *fetched = 0; }
    if (count == 0) { return ERR_ECO_SUCCESS; }
    if (out == 0 || (count > 1 && fetched == 0)) { return ERR_ECO_POINTER; }
    taken = TakeItem(self->item.pUnk != 0, &self->consumed, count);
    if (taken != 0) {
        self->item.pUnk->pVTbl->AddRef(self->item.pUnk);
        *out = self->item;
    }
    if (fetched != 0) { *fetched = taken; }
    return taken == count ? ERR_ECO_SUCCESS : ERR_ECO_FAIL;
}

static int16_t ECOCALLMETHOD ConnectionEnum_Skip(IEcoEnumConnections* me, uint32_t count) {
    ConnectionEnum* self = (ConnectionEnum*)me;
    uint32_t taken = TakeItem(self->item.pUnk != 0, &self->consumed, count);
    return taken == count ? ERR_ECO_SUCCESS : ERR_ECO_FAIL;
}

static int16_t ECOCALLMETHOD ConnectionEnum_Reset(IEcoEnumConnections* me) {
    ((ConnectionEnum*)me)->consumed = 0;
    return ERR_ECO_SUCCESS;
}

static int16_t ECOCALLMETHOD ConnectionEnum_Clone(IEcoEnumConnections* me, IEcoEnumConnections** out) {
    ConnectionEnum* self = (ConnectionEnum*)me;
    return NewConnectionEnum(self->allocator, self->item, self->consumed, out);
}

static IEcoEnumConnectionsVTbl ConnectionEnum_VTbl = {
    ConnectionEnum_QueryInterface, ConnectionEnum_AddRef, ConnectionEnum_Release,
    ConnectionEnum_Next, ConnectionEnum_Skip, ConnectionEnum_Reset, ConnectionEnum_Clone
};

static int16_t NewConnectionEnum(IEcoMemoryAllocator1* allocator, EcoConnectionData item,
                            int consumed, IEcoEnumConnections** out) {
    ConnectionEnum* self;
    if (out == 0) { return ERR_ECO_POINTER; }
    *out = 0;
    self = allocator->pVTbl->Alloc(allocator, sizeof(*self));
    if (self == 0) { return ERR_ECO_OUTOFMEMORY; }
    self->iface.pVTbl = &ConnectionEnum_VTbl;
    self->allocator = allocator;
    self->refs = 1;
    self->consumed = consumed;
    self->item = item;
    allocator->pVTbl->AddRef(allocator);
    if (item.pUnk != 0) { item.pUnk->pVTbl->AddRef(item.pUnk); }
    *out = &self->iface;
    return ERR_ECO_SUCCESS;
}


static CEcoMaxMin_D17DEB17* ContainerOwner(IEcoConnectionPointContainer* me) {
    return (CEcoMaxMin_D17DEB17*)((char*)me - offsetof(CEcoMaxMin_D17DEB17, container));
}

static CEcoMaxMin_D17DEB17* PointOwner(IEcoConnectionPoint* me) {
    return (CEcoMaxMin_D17DEB17*)((char*)me - offsetof(CEcoMaxMin_D17DEB17, point));
}

static int16_t ECOCALLMETHOD Container_QueryInterface(IEcoConnectionPointContainer* me,
                                                      const UGUID* iid, voidptr_t* out) {
    IEcoMaxMin* owner = (IEcoMaxMin*)ContainerOwner(me);
    return owner->pVTbl->QueryInterface(owner, iid, out);
}

static uint32_t ECOCALLMETHOD Container_AddRef(IEcoConnectionPointContainer* me) {
    IEcoMaxMin* owner = (IEcoMaxMin*)ContainerOwner(me);
    return owner->pVTbl->AddRef(owner);
}

static uint32_t ECOCALLMETHOD Container_Release(IEcoConnectionPointContainer* me) {
    IEcoMaxMin* owner = (IEcoMaxMin*)ContainerOwner(me);
    return owner->pVTbl->Release(owner);
}

static int16_t ECOCALLMETHOD Container_EnumConnectionPoints(IEcoConnectionPointContainer* me,
                                                           IEcoEnumConnectionPoints** out) {
    CEcoMaxMin_D17DEB17* owner = ContainerOwner(me);
    return NewPointEnum(owner->m_pIMem, &owner->point, 0, out);
}

static int16_t ECOCALLMETHOD Container_FindConnectionPoint(IEcoConnectionPointContainer* me,
                                                           const UGUID* iid, IEcoConnectionPoint** out) {
    if (out == 0) { return ERR_ECO_POINTER; }
    *out = 0;
    if (iid == 0) { return ERR_ECO_POINTER; }
    if (!IsEqualUGUID(iid, &IID_IEcoMaxMinEvents)) { return ERR_ECO_NOINTERFACE; }
    me->pVTbl->AddRef(me);
    *out = &ContainerOwner(me)->point;
    return ERR_ECO_SUCCESS;
}

IEcoConnectionPointContainerVTbl CEcoMaxMin_ContainerVTbl = {
    Container_QueryInterface, Container_AddRef, Container_Release,
    Container_EnumConnectionPoints, Container_FindConnectionPoint
};

static int16_t ECOCALLMETHOD Point_QueryInterface(IEcoConnectionPoint* me, const UGUID* iid, voidptr_t* out) {
    if (out == 0) { return ERR_ECO_POINTER; }
    *out = 0;
    if (iid == 0) { return ERR_ECO_POINTER; }
    if (!IsEqualUGUID(iid, &IID_IEcoConnectionPoint) && !IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        return ERR_ECO_NOINTERFACE;
    }
    me->pVTbl->AddRef(me);
    *out = me;
    return ERR_ECO_SUCCESS;
}

static uint32_t ECOCALLMETHOD Point_AddRef(IEcoConnectionPoint* me) {
    IEcoMaxMin* owner = (IEcoMaxMin*)PointOwner(me);
    return owner->pVTbl->AddRef(owner);
}

static uint32_t ECOCALLMETHOD Point_Release(IEcoConnectionPoint* me) {
    IEcoMaxMin* owner = (IEcoMaxMin*)PointOwner(me);
    return owner->pVTbl->Release(owner);
}

static int16_t ECOCALLMETHOD Point_GetConnectionInterface(IEcoConnectionPoint* me, UGUID* out) {
    if (out == 0) { return ERR_ECO_POINTER; }
    *out = IID_IEcoMaxMinEvents;
    return ERR_ECO_SUCCESS;
}

static int16_t ECOCALLMETHOD Point_GetConnectionPointContainer(IEcoConnectionPoint* me,
                                                               IEcoConnectionPointContainer** out) {
    if (out == 0) { return ERR_ECO_POINTER; }
    me->pVTbl->AddRef(me);
    *out = &PointOwner(me)->container;
    return ERR_ECO_SUCCESS;
}

static int16_t ECOCALLMETHOD Point_Advise(IEcoConnectionPoint* me, IEcoUnknown* sink, uint32_t* cookie) {
    CEcoMaxMin_D17DEB17* owner = PointOwner(me);
    IEcoMaxMinEvents* events = 0;
    if (cookie == 0) { return ERR_ECO_POINTER; }
    *cookie = 0;
    if (sink == 0) { return ERR_ECO_POINTER; }
    if (owner->sink != 0) { return ERR_ECO_FAIL; }
    if (sink->pVTbl->QueryInterface(sink, &IID_IEcoMaxMinEvents, (void**)&events) != ERR_ECO_SUCCESS ||
        events == 0) {
        return ERR_ECO_NOINTERFACE;
    }
    /* QueryInterface can call back into the point. Recheck before committing. */
    if (owner->sink != 0) {
        events->pVTbl->Release(events);
        return ERR_ECO_FAIL;
    }
    sink->pVTbl->AddRef(sink);
    owner->sink = sink;
    if (++owner->cookie == 0) { ++owner->cookie; }
    *cookie = owner->cookie;
    events->pVTbl->Release(events);
    return ERR_ECO_SUCCESS;
}

static int16_t ECOCALLMETHOD Point_Unadvise(IEcoConnectionPoint* me, uint32_t cookie) {
    CEcoMaxMin_D17DEB17* owner = PointOwner(me);
    IEcoUnknown* sink = owner->sink;
    if (sink == 0 || cookie != owner->cookie) { return ERR_ECO_VALUE_NOT_FOUND; }
    owner->sink = 0;
    sink->pVTbl->Release(sink);
    return ERR_ECO_SUCCESS;
}

static int16_t ECOCALLMETHOD Point_EnumConnections(IEcoConnectionPoint* me, IEcoEnumConnections** out) {
    CEcoMaxMin_D17DEB17* owner = PointOwner(me);
    EcoConnectionData item;
    item.pUnk = owner->sink;
    item.cCookie = owner->sink != 0 ? owner->cookie : 0;
    return NewConnectionEnum(owner->m_pIMem, item, 0, out);
}

IEcoConnectionPointVTbl CEcoMaxMin_PointVTbl = {
    Point_QueryInterface, Point_AddRef, Point_Release,
    Point_GetConnectionInterface, Point_GetConnectionPointContainer,
    Point_Advise, Point_Unadvise, Point_EnumConnections
};

void CEcoMaxMin_Notify(CEcoMaxMin_D17DEB17* owner, uint16_t operation, uint16_t type, uint16_t operand) {
    IEcoEnumConnections* connections;
    IEcoMaxMinEvents* events = 0;
    EcoConnectionData item;
    if (owner->sink == 0) { return; }
    if (owner->point.pVTbl->EnumConnections(&owner->point, &connections) != ERR_ECO_SUCCESS) { return; }

    if (connections->pVTbl->Next(connections, 1, &item, 0) == ERR_ECO_SUCCESS) {
        if (item.pUnk->pVTbl->QueryInterface(item.pUnk, &IID_IEcoMaxMinEvents, (void**)&events) == ERR_ECO_SUCCESS) {
            if (operand == 0) {
                events->pVTbl->OnCompare(events, operation, type);
            } else {
                events->pVTbl->OnSelect(events, operation, type, operand);
            }
            events->pVTbl->Release(events);
        }
        item.pUnk->pVTbl->Release(item.pUnk);
    }
    connections->pVTbl->Release(connections);
}
