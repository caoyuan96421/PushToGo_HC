/*
 * Comm.h
 *
 *  Created on: Jul 23, 2019
 *      Author: caoyu
 */

#ifndef _COMM_H_
#define _COMM_H_

#include <cstdio>

// Communication class
// Note: this class is NOT thread safe. Make sure all calls of read are from the same thread, for write as well. Or use other lock methods
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
