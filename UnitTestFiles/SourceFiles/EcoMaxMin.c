/* ACOM console UnitTest for the Eco.MaxMin component. */
#include <stdio.h>
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoMaxMin.h"
#include "IEcoConnectionPointContainer.h"
#include "IEcoEnumConnectionPoints.h"
#include "IEcoEnumConnections.h"
#include "IEcoMaxMinEvents.h"

typedef enum EventKind {
    EVENT_COMPARE,
    EVENT_SELECT
} EventKind;

typedef struct EventRecord {
    EventKind kind;
    uint16_t operation;
    uint16_t type;
    uint16_t operand;
} EventRecord;

typedef struct TestSink {
    IEcoMaxMinEvents iface;
    uint32_t refs;
    unsigned count;
    EventRecord records[2];
} TestSink;

static int16_t ECOCALLMETHOD Sink_QueryInterface(IEcoMaxMinEvents* me, const UGUID* iid, voidptr_t* out) {
    if (out == 0) { return ERR_ECO_POINTER; }
    *out = 0;
    if (iid == 0) { return ERR_ECO_POINTER; }
    if (!IsEqualUGUID(iid, &IID_IEcoMaxMinEvents) && !IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        return ERR_ECO_NOINTERFACE;
    }
    me->pVTbl->AddRef(me);
    *out = me;
    return ERR_ECO_SUCCESS;
}

static uint32_t ECOCALLMETHOD Sink_AddRef(IEcoMaxMinEvents* me) {
    return ++((TestSink*)me)->refs;
}

static uint32_t ECOCALLMETHOD Sink_Release(IEcoMaxMinEvents* me) {
    return --((TestSink*)me)->refs;
}

static void RecordEvent(TestSink* sink, EventKind kind, uint16_t operation, uint16_t type, uint16_t operand) {
    if (sink->count < 2) {
        EventRecord* record = &sink->records[sink->count];
        record->kind = kind;
        record->operation = operation;
        record->type = type;
        record->operand = operand;
    }
    ++sink->count;
}

static int16_t ECOCALLMETHOD Sink_OnCompare(IEcoMaxMinEvents* me, uint16_t operation, uint16_t type) {
    RecordEvent((TestSink*)me, EVENT_COMPARE, operation, type, 0);
    return ERR_ECO_SUCCESS;
}

static int16_t ECOCALLMETHOD Sink_OnSelect(IEcoMaxMinEvents* me, uint16_t operation, uint16_t type, uint16_t operand) {
    RecordEvent((TestSink*)me, EVENT_SELECT, operation, type, operand);
    /* Exercise delivery errors without changing the numeric result. */
    return ERR_ECO_FAIL;
}

static IEcoMaxMinEventsVTbl Sink_VTbl = {
    Sink_QueryInterface, Sink_AddRef, Sink_Release, Sink_OnCompare, Sink_OnSelect
};

static int CheckEvents(const TestSink* sink, const char* scenario,
                       uint16_t operation, uint16_t type, uint16_t operand) {
    const char* problem;
    if (sink->count != 2) {
        problem = "event count";
    } else if (sink->records[0].kind != EVENT_COMPARE || sink->records[1].kind != EVENT_SELECT) {
        problem = "event order";
    } else if (sink->records[0].operation != operation || sink->records[1].operation != operation) {
        problem = "event operation";
    } else if (sink->records[0].type != type || sink->records[1].type != type) {
        problem = "event type";
    } else if (sink->records[1].operand != operand) {
        problem = "selected operand";
    } else {
        return 1;
    }
    fprintf(stderr, "FAIL Lab 3: %s: %s\n", scenario, problem);
    return 0;
}

#define CHECK_LAB3(condition, message) \
    do { \
        if (!(condition)) { fprintf(stderr, "FAIL Lab 3: %s\n", message); goto Cleanup; } \
    } while (0)

/* Keep each result comparison in the method's native numeric type. */
#define CHECK_CALL(method, operation, type, a, b, expected, operand, scenario) \
    do { \
        sink.count = 0; \
        CHECK_LAB3(maxMin->pVTbl->method(maxMin, a, b) == (expected), #method " " scenario ": result"); \
        if (!CheckEvents(&sink, #method " " scenario, operation, type, operand)) { goto Cleanup; } \
        ++numericCases; \
    } while (0)

#define CHECK_METHOD(method, operation, type, winner, other) \
    do { \
        CHECK_CALL(method, operation, type, winner, other, winner, 1, "first operand"); \
        CHECK_CALL(method, operation, type, other, winner, winner, 2, "second operand"); \
        CHECK_CALL(method, operation, type, winner, winner, winner, 1, "equal operands"); \
    } while (0)

/* Own the component reference until cleanup, while the test sink is still alive. */
static int16_t CheckLab3(IEcoMaxMin* maxMin) {
    TestSink sink = {{&Sink_VTbl}, 1, 0, {{EVENT_COMPARE, 0, 0, 0}, {EVENT_COMPARE, 0, 0, 0}}};
    IEcoUnknown* sinkUnknown = 0;
    IEcoUnknown* componentUnknown = 0;
    IEcoUnknown* containerUnknown = 0;
    IEcoConnectionPointContainer* container = 0;
    IEcoConnectionPointContainer* returnedContainer = 0;
    IEcoConnectionPoint* point = 0;
    IEcoConnectionPoint* returnedPoint = 0;
    IEcoConnectionPoint* wrongPoint = 0;
    IEcoEnumConnectionPoints* points = 0;
    IEcoEnumConnectionPoints* pointClone = 0;
    IEcoEnumConnections* connections = 0;
    IEcoEnumConnections* connectionClone = 0;
    IEcoEnumConnections* empty = 0;
    EcoConnectionData entry = {0, 0};
    UGUID outgoing;
    uint32_t fetched;
    uint32_t cookie = 0;
    uint32_t rejectedCookie;
    uint32_t previousCookie;
    unsigned numericCases = 0;
    unsigned cycle = 0;
    const unsigned cycleCount = 100;
    int result;
    int16_t status = 40;

    CHECK_LAB3(maxMin->pVTbl->QueryInterface(maxMin, &IID_IEcoConnectionPointContainer,
                                           (void**)&container) == 0 && container != 0, "query container");
    CHECK_LAB3(maxMin->pVTbl->QueryInterface(maxMin, &IID_IEcoUnknown, (void**)&componentUnknown) == 0 &&
               componentUnknown != 0, "query component identity");
    CHECK_LAB3(container->pVTbl->QueryInterface(container, &IID_IEcoUnknown, (void**)&containerUnknown) == 0 &&
               containerUnknown == componentUnknown, "container identity");
    CHECK_LAB3(container->pVTbl->FindConnectionPoint(container, &IID_IEcoMaxMinEvents, &point) == 0 &&
               point != 0, "find point");
    wrongPoint = point;
    CHECK_LAB3(container->pVTbl->FindConnectionPoint(container, &IID_IEcoMaxMin, &wrongPoint) != 0 &&
               wrongPoint == 0, "reject foreign IID");
    CHECK_LAB3(point->pVTbl->GetConnectionInterface(point, &outgoing) == 0 &&
               IsEqualUGUID(&outgoing, &IID_IEcoMaxMinEvents), "outgoing IID");
    CHECK_LAB3(point->pVTbl->GetConnectionPointContainer(point, &returnedContainer) == 0 &&
               returnedContainer == container, "point container");

    CHECK_LAB3(container->pVTbl->EnumConnectionPoints(container, &points) == 0 && points != 0, "enumerate points");
    CHECK_LAB3(points->pVTbl->Next(points, 1, &returnedPoint, &fetched) == 0 &&
               fetched == 1 && returnedPoint == point, "enumerated point");
    returnedPoint->pVTbl->Release(returnedPoint);
    returnedPoint = 0;
    CHECK_LAB3(points->pVTbl->Next(points, 1, &returnedPoint, &fetched) != 0 &&
               fetched == 0, "point enumeration exhausted");
    CHECK_LAB3(points->pVTbl->Clone(points, &pointClone) == 0 && pointClone != 0, "clone point position");
    CHECK_LAB3(points->pVTbl->Reset(points) == 0 &&
               points->pVTbl->Skip(points, 1) == 0 &&
               points->pVTbl->Next(points, 1, &returnedPoint, &fetched) != 0 && fetched == 0 &&
               points->pVTbl->Skip(points, 1) != 0, "reset and skip point");
    CHECK_LAB3(points->pVTbl->Reset(points) == 0 &&
               pointClone->pVTbl->Next(pointClone, 1, &returnedPoint, &fetched) != 0 &&
               fetched == 0, "point clone retains consumed position");
    CHECK_LAB3(points->pVTbl->Next(points, 1, &returnedPoint, 0) == 0 &&
               returnedPoint == point, "point clone position is independent");
    returnedPoint->pVTbl->Release(returnedPoint);
    returnedPoint = 0;
    pointClone->pVTbl->Release(pointClone);
    pointClone = 0;
    points->pVTbl->Release(points);
    points = 0;

    fetched = 1;
    CHECK_LAB3(point->pVTbl->EnumConnections(point, &empty) == 0 && empty != 0 &&
               empty->pVTbl->Next(empty, 1, &entry, &fetched) != 0 &&
               fetched == 0, "no subscription before Advise");
    rejectedCookie = 42;
    CHECK_LAB3(point->pVTbl->Advise(point, componentUnknown, &rejectedCookie) == ERR_ECO_NOINTERFACE &&
               rejectedCookie == 0, "reject sink without events");
    CHECK_LAB3(sink.iface.pVTbl->QueryInterface(&sink.iface, &IID_IEcoUnknown, (void**)&sinkUnknown) == 0,
               "query sink identity");
    CHECK_LAB3(point->pVTbl->Advise(point, sinkUnknown, &cookie) == 0 && cookie != 0 && sink.refs == 3,
               "subscribe");
    rejectedCookie = 42;
    CHECK_LAB3(point->pVTbl->Advise(point, sinkUnknown, &rejectedCookie) != 0 &&
               rejectedCookie == 0 && sink.refs == 3, "reject second subscription");
    CHECK_LAB3(empty->pVTbl->Next(empty, 1, &entry, &fetched) != 0 &&
               fetched == 0, "empty snapshot stays empty");
    empty->pVTbl->Release(empty);
    empty = 0;

    CHECK_LAB3(point->pVTbl->EnumConnections(point, &connections) == 0 && connections != 0, "enumerate subscriptions");
    CHECK_LAB3(connections->pVTbl->Next(connections, 1, &entry, &fetched) == 0 &&
               fetched == 1 && entry.pUnk == sinkUnknown && entry.cCookie == cookie, "enumerated subscription");
    entry.pUnk->pVTbl->Release(entry.pUnk);
    entry.pUnk = 0;
    CHECK_LAB3(connections->pVTbl->Next(connections, 1, &entry, &fetched) != 0 &&
               fetched == 0, "subscription enumeration exhausted");
    CHECK_LAB3(connections->pVTbl->Clone(connections, &connectionClone) == 0 && connectionClone != 0,
               "clone subscription position");
    CHECK_LAB3(connections->pVTbl->Reset(connections) == 0 &&
               connections->pVTbl->Skip(connections, 1) == 0 &&
               connections->pVTbl->Next(connections, 1, &entry, &fetched) != 0 && fetched == 0 &&
               connections->pVTbl->Skip(connections, 1) != 0, "reset and skip subscription");
    CHECK_LAB3(connections->pVTbl->Reset(connections) == 0 &&
               connectionClone->pVTbl->Next(connectionClone, 1, &entry, &fetched) != 0 &&
               fetched == 0, "subscription clone retains consumed position");
    CHECK_LAB3(connections->pVTbl->Next(connections, 1, &entry, 0) == 0 &&
               entry.pUnk == sinkUnknown && entry.cCookie == cookie, "subscription clone position is independent");
    entry.pUnk->pVTbl->Release(entry.pUnk);
    entry.pUnk = 0;
    connections->pVTbl->Release(connections);
    connections = 0;

    CHECK_LAB3(point->pVTbl->Unadvise(point, cookie + 1) != 0, "reject wrong cookie");
    CHECK_CALL(MaxInt, ECO_MAXMIN_MAX, ECO_MAXMIN_INT, -5, 9, 9, 2, "second operand");
    printf("MaxInt(-5, 9): OnCompare -> OnSelect(%u) -> 9\n", (unsigned)sink.records[1].operand);
    CHECK_CALL(MinInt, ECO_MAXMIN_MIN, ECO_MAXMIN_INT, -5, 9, -5, 1, "first operand");
    printf("MinInt(-5, 9): OnCompare -> OnSelect(%u) -> -5\n", (unsigned)sink.records[1].operand);

    CHECK_CALL(MaxInt, ECO_MAXMIN_MAX, ECO_MAXMIN_INT, 9, -5, 9, 1, "first operand");
    CHECK_CALL(MaxInt, ECO_MAXMIN_MAX, ECO_MAXMIN_INT, 9, 9, 9, 1, "equal operands");
    CHECK_CALL(MinInt, ECO_MAXMIN_MIN, ECO_MAXMIN_INT, 9, -5, -5, 2, "second operand");
    CHECK_CALL(MinInt, ECO_MAXMIN_MIN, ECO_MAXMIN_INT, -5, -5, -5, 1, "equal operands");
    CHECK_METHOD(MaxLong, ECO_MAXMIN_MAX, ECO_MAXMIN_LONG, 1234567890L, -1234567891L);
    CHECK_METHOD(MinLong, ECO_MAXMIN_MIN, ECO_MAXMIN_LONG, -1234567891L, 1234567890L);
    CHECK_METHOD(MaxFloat, ECO_MAXMIN_MAX, ECO_MAXMIN_FLOAT, 5.5f, -1.25f);
    CHECK_METHOD(MinFloat, ECO_MAXMIN_MIN, ECO_MAXMIN_FLOAT, -1.25f, 5.5f);
    CHECK_METHOD(MaxDouble, ECO_MAXMIN_MAX, ECO_MAXMIN_DOUBLE, 9.5, -7.125);
    CHECK_METHOD(MinDouble, ECO_MAXMIN_MIN, ECO_MAXMIN_DOUBLE, -7.125, 9.5);
    CHECK_METHOD(MaxLongDouble, ECO_MAXMIN_MAX, ECO_MAXMIN_LONG_DOUBLE, 2.75L, -4.5L);
    CHECK_METHOD(MinLongDouble, ECO_MAXMIN_MIN, ECO_MAXMIN_LONG_DOUBLE, -4.5L, 2.75L);
    CHECK_LAB3(sink.refs == 4, "temporary references after delivery");
    printf("Lab 3: %u numeric and event cases passed\n", numericCases);

    previousCookie = cookie;
    CHECK_LAB3(point->pVTbl->Unadvise(point, cookie) == 0, "unsubscribe");
    cookie = 0;
    fetched = 1;
    CHECK_LAB3(point->pVTbl->EnumConnections(point, &empty) == 0 && empty != 0 &&
               empty->pVTbl->Next(empty, 1, &entry, &fetched) != 0 &&
               fetched == 0, "no subscription after Unadvise");
    CHECK_LAB3(connectionClone->pVTbl->Reset(connectionClone) == 0 &&
               connectionClone->pVTbl->Next(connectionClone, 1, &entry, 0) == 0 &&
               entry.pUnk == sinkUnknown && entry.cCookie == previousCookie, "occupied snapshot survives Unadvise");
    entry.pUnk->pVTbl->Release(entry.pUnk);
    entry.pUnk = 0;
    connectionClone->pVTbl->Release(connectionClone);
    connectionClone = 0;
    CHECK_LAB3(sink.refs == 2, "temporary references released");
    sinkUnknown->pVTbl->Release(sinkUnknown);
    sinkUnknown = 0;
    CHECK_LAB3(sink.refs == 1, "sink reference count restored");

    sink.count = 0;
    result = maxMin->pVTbl->MaxInt(maxMin, -5, 9);
    CHECK_LAB3(result == 9 && sink.count == 0, "no delivery after Unadvise");
    printf("After Unadvise: MaxInt(-5, 9) = %d, events = %u\n", result, sink.count);

    CHECK_LAB3(sink.iface.pVTbl->QueryInterface(&sink.iface, &IID_IEcoUnknown, (void**)&sinkUnknown) == 0,
               "query sink identity again");
    CHECK_LAB3(point->pVTbl->Advise(point, sinkUnknown, &cookie) == 0 &&
               cookie != 0 && cookie != previousCookie && sink.refs == 3, "subscribe again with new cookie");
    CHECK_LAB3(point->pVTbl->Unadvise(point, previousCookie) != 0, "reject previous cookie");
    sink.count = 0;
    CHECK_LAB3(maxMin->pVTbl->MaxInt(maxMin, -5, 9) == 9, "result after resubscribe");
    if (!CheckEvents(&sink, "MaxInt after resubscribe", ECO_MAXMIN_MAX, ECO_MAXMIN_INT, 2)) { goto Cleanup; }

    previousCookie = cookie;
    CHECK_LAB3(point->pVTbl->Unadvise(point, cookie) == 0 && sink.refs == 2, "unsubscribe after resubscribe");
    cookie = 0;
    for (cycle = 1; cycle <= cycleCount; ++cycle) {
        CHECK_LAB3(point->pVTbl->Advise(point, sinkUnknown, &cookie) == 0 &&
                   cookie != 0 && cookie != previousCookie && sink.refs == 3, "cycle Advise");
        sink.count = 0;
        CHECK_LAB3(maxMin->pVTbl->MaxInt(maxMin, -5, 9) == 9, "cycle MaxInt result");
        if (!CheckEvents(&sink, "cycle MaxInt", ECO_MAXMIN_MAX, ECO_MAXMIN_INT, 2)) { goto Cleanup; }
        CHECK_LAB3(sink.refs == 3, "cycle references after delivery");
        previousCookie = cookie;
        CHECK_LAB3(point->pVTbl->Unadvise(point, cookie) == 0 && sink.refs == 2, "cycle Unadvise");
        cookie = 0;
        sink.count = 0;
        CHECK_LAB3(maxMin->pVTbl->MaxInt(maxMin, -5, 9) == 9 && sink.count == 0,
                   "cycle result without events after Unadvise");
    }
    printf("Lab 3: %u subscription cycles passed\n", cycle - 1);
    cycle = 0;

    CHECK_LAB3(point->pVTbl->Advise(point, sinkUnknown, &cookie) == 0 &&
               cookie != 0 && cookie != previousCookie && sink.refs == 3, "subscribe before destruction");
    /* Leave only the point holding the component. Its final Release clears the slot. */
    returnedContainer->pVTbl->Release(returnedContainer);
    returnedContainer = 0;
    container->pVTbl->Release(container);
    container = 0;
    componentUnknown->pVTbl->Release(componentUnknown);
    componentUnknown = 0;
    containerUnknown->pVTbl->Release(containerUnknown);
    containerUnknown = 0;
    maxMin->pVTbl->Release(maxMin);
    maxMin = 0;
    CHECK_LAB3(point->pVTbl->GetConnectionInterface(point, &outgoing) == 0 &&
               IsEqualUGUID(&outgoing, &IID_IEcoMaxMinEvents), "point retains component");
    CHECK_LAB3(point->pVTbl->GetConnectionPointContainer(point, &returnedContainer) == 0 &&
               returnedContainer != 0, "recover container from last point reference");
    returnedContainer->pVTbl->Release(returnedContainer);
    returnedContainer = 0;
    fetched = point->pVTbl->Release(point);
    point = 0;
    CHECK_LAB3(fetched == 0 && sink.refs == 2, "component destruction releases active subscription");
    status = 0;

Cleanup:
    if (cycle != 0) { fprintf(stderr, "FAIL Lab 3: subscription cycle %u\n", cycle); }
    if (entry.pUnk != 0) { entry.pUnk->pVTbl->Release(entry.pUnk); }
    if (empty != 0) { empty->pVTbl->Release(empty); }
    if (connectionClone != 0) { connectionClone->pVTbl->Release(connectionClone); }
    if (connections != 0) { connections->pVTbl->Release(connections); }
    if (returnedPoint != 0) { returnedPoint->pVTbl->Release(returnedPoint); }
    if (pointClone != 0) { pointClone->pVTbl->Release(pointClone); }
    if (points != 0) { points->pVTbl->Release(points); }
    if (returnedContainer != 0) { returnedContainer->pVTbl->Release(returnedContainer); }
    if (containerUnknown != 0) { containerUnknown->pVTbl->Release(containerUnknown); }
    if (componentUnknown != 0) { componentUnknown->pVTbl->Release(componentUnknown); }
    if (container != 0) { container->pVTbl->Release(container); }
    if (point != 0) {
        if (cookie != 0) { point->pVTbl->Unadvise(point, cookie); }
        point->pVTbl->Release(point);
    }
    if (maxMin != 0) { maxMin->pVTbl->Release(maxMin); }
    if (sinkUnknown != 0) { sinkUnknown->pVTbl->Release(sinkUnknown); }
    if (sink.refs != 1) {
        fprintf(stderr, "FAIL Lab 3: sink reference count after cleanup\n");
        return 41;
    }
    return status;
}

#undef CHECK_METHOD
#undef CHECK_CALL
#undef CHECK_LAB3


/* All cases use the component through its interface (not direct calls). */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoMaxMin* pIMaxMin = 0;
    int16_t result;
    int16_t status = 0;

    if (pIUnk == 0) { return 10; }
    result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void**)&pISys);
    if (result != 0 || pISys == 0) { return 11; }
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void**)&pIBus);
    if (result != 0 || pIBus == 0) { status = 12; goto Release; }

#ifdef ECO_LIB
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

    puts("PASS: 12 Eco.MaxMin UnitTest checks");
    status = CheckLab3(pIMaxMin);
    pIMaxMin = 0;
    if (status == 0) { puts("PASS: Lab 3 connection point and event checks"); }

Release:
    if (status != 0 && status < 40) { fprintf(stderr, "FAIL: EcoMain exit code %d\n", (int)status); }
    if (pIMaxMin != 0) { pIMaxMin->pVTbl->Release(pIMaxMin); }
    if (pIBus != 0) { pIBus->pVTbl->Release(pIBus); }
    if (pISys != 0) { pISys->pVTbl->Release(pISys); }
    return status;
}
