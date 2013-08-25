/*
 * Main.c
 *
 *  Created on: 15.10.2010
 *      Author: Patrick Glauner
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "Common.h"
#include "PointInPolygon.h"

int main(int argc, char* argv[]) {
	int argumentCount;
	char* correctUsage = "Usage: px py x1 y1 x2 y2 [x3 y3 .... xn yn]";

	Point testPoint;
	Point* polygon;
	int polygonPointCount;

	int i;

	// Argument count must be even and >= 6 as a polygon
	// has at least two points
	argumentCount = argc - 1;
	if (argumentCount % 2 == 1) {
		printf("Odd number of arguments\n%s\n", correctUsage);
		return 0;
	}
	if (argumentCount < 6) {
		printf("Too few arguments\n%s\n", correctUsage);
		return 0;
	}

	// Get test point coordinates
	testPoint.x = atoi(argv[1]);
	testPoint.y = atoi(argv[2]);

	// Allocate memory and copy input
	polygonPointCount = (argumentCount - 2) / 2;
	polygon = myMalloc(polygonPointCount * sizeof(Point));
	//  Check for memory allocation failure
	if (polygon == NULL) {
		printf("Memory allocation failure!\n");
		exit(EXIT_FAILURE);
	}

	/*@+forloopexec@*/
	for (i = 0; i < polygonPointCount; i += 1) {
		Point p;
		p.x = atoi(argv[i * 2 + 3]);
		p.y = atoi(argv[i * 2 + 4]);
		polygon[i] = p;
	}
	/*@=forloopexec@*/

	// Show result
	if (isInPolygon(testPoint, polygon, polygonPointCount) == 1) {
		printf("INSIDE\n");
	} else {
		printf("OUTSIDE\n");
	}

	// Clean up
	if (polygon != 0) {
		free(polygon);
	}

	return 0;
}
