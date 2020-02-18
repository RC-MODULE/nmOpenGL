#include <stack>
#include <iostream>
#include <cstdlib>
#include <cmath> // pow for NMC-SDK
#include <cerrno>

#include "service.h"

void Triangle::print()
{
	for (auto &p :points) {
		std::cout << p.x << "; " << p.y << "; " << p.z << std::endl;
	}
}

nm32f Triangle::edgeSize(int i) const
{
	Point p1 = points[edges[i].p1];
	Point p2 = points[edges[i].p2];

	nm32f dx = fabs(p1.x - p2.x);
	nm32f dy = fabs(p1.y - p2.y);

	nm32f size = sqrt(pow(dx, 2) + pow(dy, 2));
	return size;
}

int checkAndSplitFirstLargeEdge(const Triangle& tr, nm32f xMax, nm32f yMax, Triangle &trOut1, Triangle& trOut2);
int checkAndSplitLargestEdge(const Triangle& tr, nm32f xMax, nm32f yMax, Triangle &trOut1, Triangle& trOut2);

int triangulate(const nm32f *srcVertex,
	const v4nm32f *srcColor,
	int srcCount,
	int maxWidth,
	int maxHeight,
	int maxDstSize,
	nm32f *dstVertex,
	v4nm32f *dstColor,
	int *srcTreatedCount)
{

	if ((NULL == srcVertex)	|| 
		(NULL == srcColor)	||
		(NULL == dstVertex) ||
		(NULL == dstColor)	||
		(NULL == srcTreatedCount)) {

		errno = EINVAL;
		return -1;
	}

	int currentDstSize = 0;

	int i = 0; // make it local to assign it to srcTreatedCount
	for (i = 0; i < srcCount; ++i) {
		// Get the triangle from the source
		Point a;
		Point b;
		Point c;
		a.x = srcVertex[i];
		a.y = srcVertex[i + srcCount];
		a.z = srcVertex[i + 2 * srcCount];
		b.x = srcVertex[3 * srcCount + i];
		b.y = srcVertex[3 * srcCount + i + srcCount];
		b.z = srcVertex[3 * srcCount + i + 2 * srcCount];
		c.x = srcVertex[6 * srcCount + i];
		c.y = srcVertex[6 * srcCount + i + srcCount];
		c.z = srcVertex[6 * srcCount + i + 2 * srcCount];
		a.color = srcColor[3 * i];
		b.color = srcColor[3 * i + 1];
		c.color = srcColor[3 * i + 2];
		Triangle tr{a, b, c};

		// Try to triangulate the triangle
		std::vector<Triangle> trVec;
		int spaceLeft = maxDstSize - currentDstSize;
		int res = triangulateOneTriangle(tr, (nm32f) maxWidth, (nm32f) maxHeight, spaceLeft, trVec);
		// If the number of result smaller triangles is too big
		if (res == -1) {
			// Finish triangulation
			break;
		} else {
			// Copy vertexes of the result triangles to the output vertex array (dstVertex)
			// Copy colors of the result triangles to the output color array (dstColor)
			for (size_t i = 0; i < trVec.size(); ++i) {
				Point a = trVec[i].points[0];
				Point b = trVec[i].points[1];
				Point c = trVec[i].points[2];

				dstVertex[currentDstSize + i] = 				 a.x;
				dstVertex[currentDstSize + i + maxDstSize] = 	 a.y;
				dstVertex[currentDstSize + i + 2 * maxDstSize] = a.z;
				dstVertex[currentDstSize + i + 3 * maxDstSize] = b.x;
				dstVertex[currentDstSize + i + 4 * maxDstSize] = b.y;
				dstVertex[currentDstSize + i + 5 * maxDstSize] = b.z;
				dstVertex[currentDstSize + i + 6 * maxDstSize] = c.x;
				dstVertex[currentDstSize + i + 7 * maxDstSize] = c.y;
				dstVertex[currentDstSize + i + 8 * maxDstSize] = c.z;

				dstColor[3 * currentDstSize + 3 * i] = 		a.color;
				dstColor[3 * currentDstSize + 3 * i + 1] = 	b.color;
				dstColor[3 * currentDstSize + 3 * i + 2] = 	c.color;
			}
			// Increase the number of the result output triangles
			currentDstSize += trVec.size();
		}
	}
	*srcTreatedCount = i;
	return currentDstSize;
}

int triangulateOneTriangle(const Triangle& tr, nm32f xMax, nm32f yMax, int trLimit, std::vector<Triangle>& trVec){
	std::stack<Triangle> trStack;
	trStack.push(tr);
	int trCount = 0;
	
	while ((!trStack.empty()) && (trCount < trLimit)) {
		// Get the triangle out of the stack
		Triangle triangle = trStack.top();
		trStack.pop();
		// Process the triangle:
		Triangle trOut1;
		Triangle trOut2;
		// Check the size and split if it is necessary
		//int res = checkAndSplitFirstLargeEdge(triangle, xMax, yMax, trOut1, trOut2);
		int res = checkAndSplitLargestEdge(triangle, xMax, yMax, trOut1, trOut2);
		// Triangle has been splited
		if (0 == res) {
			trStack.push(trOut1);
			trStack.push(trOut2);
			continue;
		} else {
			// Triangle size is OK
			// Push the triangle to the output 
			trCount += 1;
			trVec.push_back(triangle);
		}		
	}

	// If there are no more triangles in the stack to divide
	if (trStack.empty())
	{
		// All output triangles are in out
		return 0;
	} else {
		// 'While' is finished because there are no more free space
		// for the triangles
		return -1;
	}
}

int checkAndSplitFirstLargeEdge(const Triangle& tr, nm32f xMax, nm32f yMax, Triangle &trOut1, Triangle& trOut2)
{
	Point a = tr.points[0];
	Point b = tr.points[1];
	Point c = tr.points[2];
	// If some of the sides is too big:
	// 1. divide the triangle
	// 2. push two output triangles back to caller

	// Check ab side
	if (fabs(a.x - b.x) > xMax || fabs(a.y - b.y) > yMax) {
		Point d;
		d.x = (a.x + b.x) / 2;
		d.y = (a.y + b.y) / 2;
		d.z = (a.z + b.z) / 2;
		d.color.vec[0] = (a.color.vec[0] + b.color.vec[0]) / 2;
		d.color.vec[1] = (a.color.vec[1] + b.color.vec[1]) / 2;
		d.color.vec[2] = (a.color.vec[2] + b.color.vec[2]) / 2;
		d.color.vec[3] = (a.color.vec[3] + b.color.vec[3]) / 2;
		trOut1 = Triangle{a, d, c};
		trOut2 = Triangle{b, d, c};
		return 0;
	}
	// Check bc side
	if (fabs(b.x - c.x) > xMax || fabs(b.y - c.y) > yMax) {
		Point d;
		d.x = (b.x + c.x) / 2;
		d.y = (b.y + c.y) / 2;
		d.z = (b.z + c.z) / 2;
		d.color.vec[0] = (b.color.vec[0] + c.color.vec[0]) / 2;
		d.color.vec[1] = (b.color.vec[1] + c.color.vec[1]) / 2;
		d.color.vec[2] = (b.color.vec[2] + c.color.vec[2]) / 2;
		d.color.vec[3] = (b.color.vec[3] + c.color.vec[3]) / 2;
		//std::cout << " " << "d.x = " << d.x << ", " << "d.y = " << d.y << std::endl;
		trOut1 = Triangle{b, d, a};
		trOut2 = Triangle{c, d, a};
		return 0;
	}
	// Check ac side
	if (fabs(a.x - c.x) > xMax || fabs(a.y - c.y) > yMax) {
		Point d;
		d.x = (a.x + c.x) / 2;
		d.y = (a.y + c.y) / 2;
		d.z = (a.z + c.z) / 2;
		d.color.vec[0] = (a.color.vec[0] + c.color.vec[0]) / 2;
		d.color.vec[1] = (a.color.vec[1] + c.color.vec[1]) / 2;
		d.color.vec[2] = (a.color.vec[2] + c.color.vec[2]) / 2;
		d.color.vec[3] = (a.color.vec[3] + c.color.vec[3]) / 2;
		trOut1 = Triangle{a, d, b};
		trOut2 = Triangle{c, d, b};
		return 0;
	}
	return -1;
}

int checkAndSplitLargestEdge(const Triangle& tr, nm32f xMax, nm32f yMax, Triangle &trOut1, Triangle& trOut2)
{
	nm32f edgeSizeLimit = sqrt(pow(xMax, 2) + pow(yMax, 2));
	nm32f largestEdgeSize = 0;
	int largestEdgeID = 0;
	// Find largest edge
	for (int i = 0; i < 3; ++i) {
		nm32f currentEdgeSize = tr.edgeSize(i);
		if (currentEdgeSize > largestEdgeSize) {
			largestEdgeID = i;
			largestEdgeSize = currentEdgeSize;
		}
	}
	// If larges edge is too large then division is necessary 
	if (largestEdgeSize > edgeSizeLimit) {
		Point a = tr.points[tr.edges[largestEdgeID].p1];
		Point b = tr.points[tr.edges[largestEdgeID].p2];
		// Compute the other point of the triangle
		// !(0b01 | 0b00) = 0b10
		// !(0b10 | 0b00) = 0b01
		// !(0b01 | 0b10) = 0b00
		int cId = (~(tr.edges[largestEdgeID].p1 | tr.edges[largestEdgeID].p2)) & 0x03;
		Point c = tr.points[cId];
		Point d;
		d.x = (a.x + b.x) / 2;
		d.y = (a.y + b.y) / 2;
		d.z = (a.z + b.z) / 2;
		d.color.vec[0] = (a.color.vec[0] + b.color.vec[0]) / 2;
		d.color.vec[1] = (a.color.vec[1] + b.color.vec[1]) / 2;
		d.color.vec[2] = (a.color.vec[2] + b.color.vec[2]) / 2;
		d.color.vec[3] = (a.color.vec[3] + b.color.vec[3]) / 2;
		trOut1 = Triangle{ a, c, d };
		trOut2 = Triangle{ b, c, d };
		return 0;
	} else {
		return -1;
	}
}


