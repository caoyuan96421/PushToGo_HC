/*
 * FreeRTOS-openocd.c
 *
 *  Created on: Aug 3, 2019
 *      Author: caoyu
 */

#include "FreeRTOS.h"

#ifdef __GNUC__
#define USED __attribute__((used))
#else
#define USED
#endif

const int USED uxTopUsedPriority = configMAX_PRIORITIES - 1;
