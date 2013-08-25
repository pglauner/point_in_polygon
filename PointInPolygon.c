/*
 * PointInPolygon.c
 *
 *  Created on: 15.10.2010
 *      Author: Patrick Glauner
 */
#include <stdio.h>
#include <limits.h>

#include "PointInPolygon.h"

/*
Travelling from p0 to p1 to p2.
@return -1 for counter-clockwise or if p0 is on the line segment between p1 and p2
        1 for clockwise or if p1 is on the line segment between p0 and p2
		0 if p2 in on the line segment between p0 and p1
*/
static int ccw(Point p0, Point p1, Point p2) {
	long long dx1 = p1.x - p0.x;
	long long dy1 = p1.y - p0.y;
	long long dx2 = p2.x - p0.x;
	long long dy2 = p2.y - p0.y;

	// second slope is greater than the first one --> counter-clockwise
	if (dx1 * dy2 > dx2 * dy1) {
		return 1;
	}
	// first slope is greater than the second one --> clockwise
	else if (dx1 * dy2 < dx2 * dy1) {
		return -1;
	}
	// both slopes are equal --> collinear line segments
	else {
		// p0 is between p1 and p2
		if (dx1 * dx2 < 0 || dy1 * dy2 < 0) {
			return -1;
		}
		// p2 is between p0 and p1, as the length is compared
		// square roots are avoided to increase performance
		else if (dx1 * dx1 + dy1 * dy1 >= dx2 * dx2 + dy2 * dy2) {
			return 0;
		}
		// p1 is between p0 and p2
		else {
			return 1;
		}
	}
}

/*
Checks if the line segments intersect.
@return 1 if there is an intersection
        0 otherwise
*/
static int intersect(Line line1, Line line2) {
	// ccw returns 0 if two points are identical, except from the situation
	// when p0 and p1 are identical and different from p2
	int ccw11 = ccw(line1.p1, line1.p2, line2.p1);
	int ccw12 = ccw(line1.p1, line1.p2, line2.p2);
	int ccw21 = ccw(line2.p1, line2.p2, line1.p1);
	int ccw22 = ccw(line2.p1, line2.p2, line1.p2);

	return (((ccw11 * ccw12 < 0) && (ccw21 * ccw22 < 0))
			// one ccw value is zero to detect an intersection
			|| (ccw11 * ccw12 * ccw21 * ccw22 == 0)) ? 1 : 0;
}

/*
@return next valid index (current + 1 or start index)
		for an array with n entries
@param n entries count
@param current current index
 */
static int getNextIndex(int n, int current) {
	return current == n - 1 ? 0 : current + 1;
}


int isInPolygon(Point testPoint, Point* polygon, int n) {
	Line xAxis;
	Line xAxisPositive;

	Point startPoint;
	Point endPoint;
	Line edge;
	Line testPointLine;

	int i;
	int startNodePosition;
	int count;
	int seenPoints;

	// Initial start point
	startPoint.x = 0;
	startPoint.y = 0;

	// Create axes
	xAxis.p1.x = 0;
	xAxis.p1.y = 0;
	xAxis.p2.x = 0;
	xAxis.p2.y = 0;
	xAxisPositive.p1.x = 0;
	xAxisPositive.p1.y = 0;
	xAxisPositive.p2.x = 0;
	xAxisPositive.p2.y = 0;

	startNodePosition = -1;

	// Is testPoint on a node?
	// Move polygon to 0|0
	// Enlarge axes
	for (i = 0; i < n; i++) {
		if (testPoint.x == polygon[i].x && testPoint.y == polygon[i].y) {
			return 1;
		}

		// Move polygon to 0|0
		polygon[i].x -= testPoint.x;
		polygon[i].y -= testPoint.y;

		// Find start point which is not on the x axis
		if (polygon[i].y != 0) {
			startPoint.x = polygon[i].x;
			startPoint.y = polygon[i].y;
			startNodePosition = i;
		}

		// Enlarge axes
		if (polygon[i].x > xAxis.p2.x) {
			xAxis.p2.x = polygon[i].x;
			xAxisPositive.p2.x = polygon[i].x;
		}
		if (polygon[i].x < xAxis.p1.x) {
			xAxis.p1.x = polygon[i].x;
		}
	}

	// Move testPoint to 0|0
	testPoint.x = 0;
	testPoint.y = 0;
	testPointLine.p1 = testPoint;
	testPointLine.p2 = testPoint;

	// Is testPoint on an edge?
	for (i = 0; i < n; i++) {
		edge.p1 = polygon[i];
		// Get correct index of successor edge
		edge.p2 = polygon[getNextIndex(n, i)];
		if (intersect(testPointLine, edge) == 1) {
			return 1;
		}
	}

	// No start point found and point is not on an edge or node
	// --> point is outside
	if (startNodePosition == -1) {
		return 0;
	}

	count = 0;
	seenPoints = 0;
	i = startNodePosition;

	// Consider all edges
	while (seenPoints < n) {

		int savedX = polygon[getNextIndex(n, i)].x;
		int savedIndex = getNextIndex(n, i);

		// Move to next point which is not on the x-axis
		do {
			i = getNextIndex(n, i);
			seenPoints++;
		} while (polygon[i].y == 0);
		// Found end point
		endPoint.x = polygon[i].x;
		endPoint.y = polygon[i].y;

		// Only intersect lines that cross the x-axis
		if (startPoint.y * endPoint.y < 0) {
			edge.p1 = startPoint;
			edge.p2 = endPoint;

			// No nodes have been skipped and the successor node
			// has been chosen as the end point
			if (savedIndex == i) {
				count += intersect(edge, xAxisPositive);
			}
			// If at least one node on the right side has been skipped,
			// the original edge would have been intersected
			// --> intersect with full x-axis
			else if (savedX > 0) {
				count += intersect(edge, xAxis);
			}
		}
		// End point is the next start point
		startPoint = endPoint;
	}

	// Odd count --> in the polygon (1)
	// Even count --> outside (0)
	return count % 2;
}
