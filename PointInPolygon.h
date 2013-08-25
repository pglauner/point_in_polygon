/*
 * PointInPolygon.c
 *
 *  Created on: 15.10.2010
 *      Author: Patrick Glauner
 */

#ifndef POINTINPOLYGON_C_
#define POINTINPOLYGON_C_

#include "Common.h"

/*
Checks if a point is in a polygon. The point is also in the polygon
if it is on a node or on an edge.
@return 1 if the point is in the polygon
        0 otherwise
*/
int isInPolygon(Point testPoint, Point* polygon, int n);

#endif /* POINTINPOLYGON_C_ */
