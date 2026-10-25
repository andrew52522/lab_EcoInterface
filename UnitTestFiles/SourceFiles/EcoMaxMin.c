/* ACOM Lab2 UnitTest: one public component with X/Y adapters; multiple backends. */
#include "IEcoSystem1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoMaxMin.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorC.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

int16_t EcoMain(IEcoUnknown* pIUnk) {
    IEcoSystem1* sys = 0;
    IEcoInterfaceBus1* bus = 0;
    IEcoMaxMin* mm = 0;
    IEcoMaxMin* mm2 = 0;
    IEcoCalculatorX* x = 0;
    IEcoCalculatorX* xFromY = 0;
    IEcoCalculatorX* dx = 0;
    IEcoCalculatorX* ex = 0;
    IEcoCalculatorY* y = 0;
    IEcoCalculatorY* dy = 0;
    IEcoCalculatorY* ey = 0;
    IEcoCalculatorY* yFromX = 0;
    IEcoCalculatorY* eyFromX = 0;
    IEcoUnknown* unkMM = 0;
    IEcoUnknown* unkX = 0;
    IEcoUnknown* unkY = 0;
    int16_t result;
    int16_t status = 0;

    if (pIUnk == 0) { return 10; }
    result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void**)&sys);
    if (result != 0 || sys == 0) { return 11; }
    result = sys->pVTbl->QueryInterface(sys, &IID_IEcoInterfaceBus1, (void**)&bus);
    if (result != 0 || bus == 0) { status = 12; goto Cleanup; }
#ifdef ECO_LIB
    result = bus->pVTbl->RegisterComponent(bus, &CID_EcoCalculatorB,
         (IEcoUnknown*)GetIEcoComponentFactoryPtr_AE202E543CE54550899603BD70C62565);
    if (result != 0) { status = 13; goto Cleanup; }
    result = bus->pVTbl->RegisterComponent(bus, &CID_EcoCalculatorD,
         (IEcoUnknown*)GetIEcoComponentFactoryPtr_3A8E44677E82475CB4A3719ED8397E61);
    if (result != 0) { status = 14; goto Cleanup; }
    result = bus->pVTbl->RegisterComponent(bus, &CID_EcoCalculatorE,
         (IEcoUnknown*)GetIEcoComponentFactoryPtr_872FEF1DE3314B87AD44D1E7C232C2F0);
    if (result != 0) { status = 15; goto Cleanup; }
#if defined(LAB2_BACKEND_C)
    result = bus->pVTbl->RegisterComponent(bus, &CID_EcoCalculatorC,
         (IEcoUnknown*)GetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E);
    if (result != 0) { status = 16; goto Cleanup; }
#elif !defined(LAB2_BACKEND_D)
    result = bus->pVTbl->RegisterComponent(bus, &CID_EcoCalculatorA,
         (IEcoUnknown*)GetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E);
    if (result != 0) { status = 17; goto Cleanup; }
#endif
    result = bus->pVTbl->RegisterComponent(bus, &CID_EcoMaxMin,
         (IEcoUnknown*)GetIEcoComponentFactoryPtr_48749720D2434B819DCFE025D17DEB17);
    if (result != 0) { status = 18; goto Cleanup; }
#endif

    result = bus->pVTbl->QueryComponent(bus, &CID_EcoMaxMin, 0, &IID_IEcoMaxMin, (void**)&mm);
    if (result != 0 || mm == 0) { status = 20; goto Cleanup; }
    if (mm->pVTbl->MaxInt(mm,-5,9) != 9 || mm->pVTbl->MinInt(mm,-5,9) != -5) { status = 21; goto Cleanup; }
    if (mm->pVTbl->MaxDouble(mm, 7.0,2.0) != 7.0 || mm->pVTbl->MinDouble(mm,7.0,2.0) != 2.0) { status = 22; goto Cleanup; }

    result = mm->pVTbl->QueryInterface(mm, &IID_IEcoCalculatorX, (void**)&x);
    if (result != 0 || x == 0) { status = 23; goto Cleanup; }
    result = mm->pVTbl->QueryInterface(mm, &IID_IEcoCalculatorY, (void**)&y);
    if (result != 0 || y == 0) { status = 24; goto Cleanup; }
    if (x->pVTbl->Addition(x,3,5) != 8 || x->pVTbl->Subtraction(x,3,5) != -2) { status = 25; goto Cleanup; }
    if (y->pVTbl->Multiplication(y,3,5) != 15 || y->pVTbl->Division(y,15,3) != 5) { status = 26; goto Cleanup; }

    result = x->pVTbl->QueryInterface(x, &IID_IEcoCalculatorY, (void**)&yFromX);
    if (result != 0 || yFromX == 0 || yFromX->pVTbl->Multiplication(yFromX,4,3) != 12) { status = 27; goto Cleanup; }
    result = y->pVTbl->QueryInterface(y, &IID_IEcoCalculatorX, (void**)&xFromY);
    if (result != 0 || xFromY == 0 || xFromY->pVTbl->Addition(xFromY,1,4) != 5) { status = 28; goto Cleanup; }
    result = y->pVTbl->QueryInterface(y, &IID_IEcoMaxMin, (void**)&mm2);
    if (result != 0 || mm2 == 0 || mm2->pVTbl->MinInt(mm2,1,4) != 1) { status = 29; goto Cleanup; }
    result = mm->pVTbl->QueryInterface(mm, &IID_IEcoUnknown, (void**)&unkMM);
    if (result != 0 || unkMM == 0) { status = 30; goto Cleanup; }
    result = x->pVTbl->QueryInterface(x, &IID_IEcoUnknown, (void**)&unkX);
    if (result != 0 || unkX != unkMM) { status = 31; goto Cleanup; }
    result = y->pVTbl->QueryInterface(y, &IID_IEcoUnknown, (void**)&unkY);
    if (result != 0 || unkY != unkMM) { status = 32; goto Cleanup; }

    /* D includes B for Addition/Subtraction and implements Multiplication/Division. */
    result = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorX, (void**)&dx);
    if (result != 0 || dx == 0) { status = 33; goto Cleanup; }
    if (dx->pVTbl->Addition(dx,10,7) != 17 || dx->pVTbl->Subtraction(dx,10,7) != 3) { status = 34; goto Cleanup; }
    result = dx->pVTbl->QueryInterface(dx, &IID_IEcoCalculatorY, (void**)&dy);
    if (result != 0 || dy == 0 || dy->pVTbl->Multiplication(dy,6,3) != 18) { status = 35; goto Cleanup; }

    /* E aggregates B. Request X from Y, then Y again from B's delegated X. */
    result = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (void**)&ey);
    if (result != 0 || ey == 0) { status = 36; goto Cleanup; }
    result = ey->pVTbl->QueryInterface(ey, &IID_IEcoCalculatorX, (void**)&ex);
    if (result != 0 || ex == 0 || ex->pVTbl->Addition(ex,4,6) != 10) { status = 37; goto Cleanup; }
    result = ex->pVTbl->QueryInterface(ex, &IID_IEcoCalculatorY, (void**)&eyFromX);
    if (result != 0 || eyFromX == 0 || eyFromX->pVTbl->Division(eyFromX,20,4) != 5) { status = 38; goto Cleanup; }

#if defined(LAB2_BACKEND_C)
    /* C provides X and Y through the same component. */
    { IEcoCalculatorX* cx = 0; IEcoCalculatorY* cy = 0;
      result = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorC, 0, &IID_IEcoCalculatorX, (void**)&cx);
      if (result != 0 || cx == 0) { status = 39; goto Cleanup; }
      result = cx->pVTbl->QueryInterface(cx, &IID_IEcoCalculatorY, (void**)&cy);
      if (result != 0 || cy == 0 || cy->pVTbl->Multiplication(cy,3,4) != 12) { status = 40; }
      if (cy != 0) { cy->pVTbl->Release(cy); }
      cx->pVTbl->Release(cx);
      if (status != 0) { goto Cleanup; }
    }
#endif
Cleanup:
    if (eyFromX != 0) { eyFromX->pVTbl->Release(eyFromX); }
    if (ex != 0) { ex->pVTbl->Release(ex); }
    if (ey != 0) { ey->pVTbl->Release(ey); }
    if (dy != 0) { dy->pVTbl->Release(dy); }
    if (dx != 0) { dx->pVTbl->Release(dx); }
    if (unkY != 0) { unkY->pVTbl->Release(unkY); }
    if (unkX != 0) { unkX->pVTbl->Release(unkX); }
    if (unkMM != 0) { unkMM->pVTbl->Release(unkMM); }
    if (mm2 != 0) { mm2->pVTbl->Release(mm2); }
    if (xFromY != 0) { xFromY->pVTbl->Release(xFromY); }
    if (yFromX != 0) { yFromX->pVTbl->Release(yFromX); }
    if (y != 0) { y->pVTbl->Release(y); }
    if (x != 0) { x->pVTbl->Release(x); }
    if (mm != 0) { mm->pVTbl->Release(mm); }
    if (bus != 0) { bus->pVTbl->Release(bus); }
    if (sys != 0) { sys->pVTbl->Release(sys); }
    return status;
}
