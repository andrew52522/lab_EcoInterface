/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoMaxMin
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoMaxMin
 * </description>
 *
 * <reference>
 *
 * </reference>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __ID_ECOMAXMIN_H__
#define __ID_ECOMAXMIN_H__

#include "IEcoBase1.h"
#include "IEcoMaxMin.h"

/* EcoMaxMin CID = {48749720-D243-4B81-9DCF-E025D17DEB17} */
#ifndef __CID_EcoMaxMin
static const UGUID CID_EcoMaxMin = {0x01, 0x10, {0x48, 0x74, 0x97, 0x20, 0xD2, 0x43, 0x4B, 0x81, 0x9D, 0xCF, 0xE0, 0x25, 0xD1, 0x7D, 0xEB, 0x17}};
#endif /* __CID_EcoMaxMin */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_48749720D2434B819DCFE025D17DEB17;
#endif

#endif /* __ID_ECOMAXMIN_H__ */

