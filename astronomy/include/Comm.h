/*
 * Comm.h
 *
 *  Created on: Jul 23, 2019
 *      Author: caoyu
 */

#ifndef _COMM_H_
#define _COMM_H_

#include <cstdio>

class Comm {
public:
	static void init();
	static int read(char *buf, size_t len);
	static int write(char *buf, size_t len);
	static int printf(const char *fmt, ...);

private:
	Comm();
	~Comm();
};

#endif /* _COMM_H_ */
