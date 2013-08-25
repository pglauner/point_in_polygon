/*
 * Common.h
 *
 *  Created on: 15.10.2010
 *      Author: Patrick Glauner
 */

#ifndef COMMON_H_
#define COMMON_H_

/*
2 dimensional point.
*/
typedef struct  {
	int x;
	int y;
} Point;

/*
2 dimensional line segment from p1 to p2.
*/
typedef struct {
	Point p1;
	Point p2;
} Line;

/*
Allocates memory.
@param size in bytes
*/
/*@out@*/ /*@null@*/
void* myMalloc(size_t size);

#endif /* COMMON_H_ */
