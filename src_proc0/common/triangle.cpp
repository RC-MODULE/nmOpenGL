#include <cmath>
#include "service.h"

nm32f Triangle::edgeSize(int i) const
{
	Point p1 = points[edges[i].p1];
	Point p2 = points[edges[i].p2];

	nm32f dx = fabs(p1.x - p2.x);
	nm32f dy = fabs(p1.y - p2.y);

	nm32f size = pow(dx, 2) + pow(dy, 2);
	return size;
}

edgeProjection Triangle::edgeGetProjection(int i) const
{
	Point p1 = points[edges[i].p1];
	Point p2 = points[edges[i].p2];
	struct edgeProjection res;

	res.dx = fabs(p1.x - p2.x);
	res.dy = fabs(p1.y - p2.y);

	return res;
}

bool Triangle::isTooBig(nm32f xMax, nm32f yMax) const
{
	// If the projection of at least one edge is more than allowed value
	// then split the triangle
	edgeProjection edge0Projection = this->edgeGetProjection(0);
	edgeProjection edge1Projection = this->edgeGetProjection(1);
	edgeProjection edge2Projection = this->edgeGetProjection(2);
	if (edge0Projection.dx > xMax || edge0Projection.dy > yMax ||
		edge1Projection.dx > xMax || edge1Projection.dy > yMax ||
		edge2Projection.dx > xMax || edge2Projection.dy > yMax ) {
			return true;
	} else {
			return false;
	}
}

