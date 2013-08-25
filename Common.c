/*
 * Common.c
 *
 *  Created on: 16.10.2010
 *      Author: Patrick Glauner
 */

#include <stdlib.h>

#include "Common.h"

/*@out@*/ /*@null@*/
void* myMalloc(size_t size) {
	return malloc(size);
}
