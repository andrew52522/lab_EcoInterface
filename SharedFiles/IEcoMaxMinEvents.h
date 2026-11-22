#ifndef IECO_MAXMIN_EVENTS_H
#define IECO_MAXMIN_EVENTS_H

#include "IEcoBase1.h"

static const UGUID IID_IEcoMaxMinEvents = {
    0x01, 0x10, {0xB4, 0x99, 0x42, 0x02, 0x4C, 0x85, 0x41, 0x97,
                 0x87, 0x25, 0x28, 0xC4, 0xCD, 0xEA, 0x43, 0x74}
};

enum EcoMaxMinOperation {
    ECO_MAXMIN_MAX = 1,
    ECO_MAXMIN_MIN
};

enum EcoMaxMinNumberType {
    ECO_MAXMIN_INT = 1,
    ECO_MAXMIN_LONG,
    ECO_MAXMIN_FLOAT,
    ECO_MAXMIN_DOUBLE,
    ECO_MAXMIN_LONG_DOUBLE
};

typedef struct IEcoMaxMinEvents* IEcoMaxMinEventsPtr_t;
typedef struct IEcoMaxMinEventsVTbl {
    int16_t (ECOCALLMETHOD *QueryInterface)(IEcoMaxMinEventsPtr_t me, const UGUID* iid, voidptr_t* out);
    uint32_t (ECOCALLMETHOD *AddRef)(IEcoMaxMinEventsPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(IEcoMaxMinEventsPtr_t me);
    int16_t (ECOCALLMETHOD *OnCompare)(IEcoMaxMinEventsPtr_t me, uint16_t operation, uint16_t type);
    int16_t (ECOCALLMETHOD *OnSelect)(IEcoMaxMinEventsPtr_t me, uint16_t operation, uint16_t type, uint16_t operand);
} IEcoMaxMinEventsVTbl;

interface IEcoMaxMinEvents {
    IEcoMaxMinEventsVTbl* pVTbl;
} IEcoMaxMinEvents;

#endif
