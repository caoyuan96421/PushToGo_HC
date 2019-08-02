/*
 * Debug.h
 *
 *  Created on: Jul 23, 2019
 *      Author: caoyu
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "Comm.h"

void debug_if(int condition, const char *format, ...);
void error(const char *format, ...);

#endif /* _DEBUG_H_ */
