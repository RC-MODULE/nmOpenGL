#include <cmath> // pow for NMC-SDK
#include "nmpp.h"
#include "nmblas.h"
#include "service.h"


static int checkAndSplitFirstLargeEdge(const Triangle& tr, nm32f xMax, nm32f yMax, Triangle &trOut1, Triangle& trOut2);
static int checkAndSplitLargestEdge(const Triangle& tr, nm32f xMax, nm32f yMax, Triangle &trOut1, Triangle& trOut2);

static Buffer initBuf(void *data, int size)
{
	Buffer buf;

	buf.data = data;
	buf.back = size;
	buf.front = -1;
	buf.size = size;

	return buf;
}

static int bufIsFull(Buffer *buf)
{
	return ((buf->front + 1) == buf->back);	
}

static int bufIsEmpty(Buffer *buf)
{
	return (buf->back == buf->size);	
}

static int bufSize(Buffer *buf)
{
	return (buf->front + 1);	
}

static int bufSpace(Buffer *buf)
{
	return (buf->back - buf->front - 1);	
}

//      push here         
//          |
//          V
// |-|-|-|-|-|x|x|
// |-|-|-|-|-|x|x|
// |-|-|-|-|-|x|x|
static int pushBack(Buffer *buf, Vertices *vert, Colors *colors)
{
	if (bufIsFull(buf)){
		return -1;
	} else {
		TrianglePointers *trPtr = (TrianglePointers *) buf->data; 
		buf->back -= 1;
		//Push A vertex
		trPtr->v0.x[buf->back] = vert->v[0];
		trPtr->v0.y[buf->back] = vert->v[1];
		trPtr->v0.z[buf->back] = vert->v[2];
		trPtr->v0.color[buf->back] = colors->c[0];
		//Push B vertex
		trPtr->v1.x[buf->back] = vert->v[3];
		trPtr->v1.y[buf->back] = vert->v[4];
		trPtr->v1.z[buf->back] = vert->v[5];
		trPtr->v1.color[buf->back] = colors->c[1];
		//Push C vertex
		trPtr->v2.x[buf->back] = vert->v[6];
		trPtr->v2.y[buf->back] = vert->v[7];
		trPtr->v2.z[buf->back] = vert->v[8];
		trPtr->v2.color[buf->back] = colors->c[2];
		return 0;
	}
}

//   return this item         
//          |
//          V
// |-|-|-|-|x|x|x|
// |-|-|-|-|x|x|x|
// |-|-|-|-|x|x|x|
static int popBack(Buffer *buf, Vertices *vert, Colors *colors)
{
	if (bufIsEmpty(buf)){
		return -1;
	} else {
		TrianglePointers *trPtr = (TrianglePointers *) buf->data; 
		//Push A vertex
		vert->v[0] = trPtr->v0.x[buf->back];
		vert->v[1] = trPtr->v0.y[buf->back]; 
		vert->v[2] = trPtr->v0.z[buf->back]; 
		colors->c[0] = trPtr->v0.color[buf->back];
		//Push B vertex
		vert->v[3] = trPtr->v1.x[buf->back];
		vert->v[4] = trPtr->v1.y[buf->back]; 
		vert->v[5] = trPtr->v1.z[buf->back]; 
		colors->c[1] = trPtr->v1.color[buf->back];
		//Push C vertex
		vert->v[6] = trPtr->v2.x[buf->back];
		vert->v[7] = trPtr->v2.y[buf->back]; 
		vert->v[8] = trPtr->v2.z[buf->back]; 
		colors->c[2] = trPtr->v2.color[buf->back];

		buf->back += 1;
		return 0;
	}
}

//      push here         
//          |
//          V
// |x|x|x|x|-|-|-|
// |x|x|x|x|-|-|-|
// |x|x|x|x|-|-|-|
static int pushFront(Buffer *buf, Vertices *vert, Colors *colors)
{
	if (bufIsFull(buf)){
		return -1;
	} else {
		TrianglePointers *trPtr = (TrianglePointers *) buf->data; 
		buf->front += 1;
		//Push A vertex
		trPtr->v0.x[buf->front] = vert->v[0];
		trPtr->v0.y[buf->front] = vert->v[1];
		trPtr->v0.z[buf->front] = vert->v[2];
		trPtr->v0.color[buf->front] = colors->c[0];
		//Push B vertex
		trPtr->v1.x[buf->front] = vert->v[3];
		trPtr->v1.y[buf->front] = vert->v[4];
		trPtr->v1.z[buf->front] = vert->v[5];
		trPtr->v1.color[buf->front] = colors->c[1];
		//Push C vertex
		trPtr->v2.x[buf->front] = vert->v[6];
		trPtr->v2.y[buf->front] = vert->v[7];
		trPtr->v2.z[buf->front] = vert->v[8];
		trPtr->v2.color[buf->front] = colors->c[2];
		return 0;
	}
}

SECTION(".text_demo3d")
int splitTriangles(TrianglePointers *srcVertex, 
				   int srcCount,
				   int maxWidth, 
				   int maxHeight, 
				   int maxDstSize, 
				   TrianglePointers *dstVertex, 
				   int *srcTreatedCount)
{
	int currentDstSize = 0;

	Buffer buf = initBuf((void *) dstVertex, maxDstSize);

	int i = 0; // make this iterator local to assign it later to srcTreatedCount
	for (i = 0; i < srcCount; ++i) {
		// Get the triangle from the source
		Point a;
		Point b;
		Point c;

		a.x = srcVertex->v0.x[i];
		a.y = srcVertex->v0.y[i];
		a.z = srcVertex->v0.z[i];
		b.x = srcVertex->v1.x[i];
		b.y = srcVertex->v1.y[i];
		b.z = srcVertex->v1.z[i];
		c.x = srcVertex->v2.x[i];
		c.y = srcVertex->v2.y[i];
		c.z = srcVertex->v2.z[i];
		a.color = srcVertex->v0.color[i];
		b.color = srcVertex->v1.color[i];
		c.color = srcVertex->v2.color[i];
		Triangle tr{a, b, c};

		// Try to triangulate the triangle
		int res = splitOneTriangle(tr, 
										(nm32f) maxWidth, 
										(nm32f) maxHeight, 
										&buf);
		// If the number of smaller triangles is too big
		if (res == -1) {
			// Finish triangulation
			break;
		} else {
			// Update the number of the result output triangles
			// in case of successful split
			// (splitOneTriangle modifies output buffer, so
			// update result size only in case of successful split)
			currentDstSize = res;
		}
	}
	*srcTreatedCount = i;
	return currentDstSize;
}

// Return:
// -1 - деление не удалось, в выходном буфере нет места
// n - текущее количество треугольников в выходном буфере
int splitOneTriangle(	const Triangle& tr, 
							nm32f xMax, 
							nm32f yMax, 
							Buffer *buf)
{
	// Check the free space in output buffer
	// (atleast for input triangle if it is OK and don't be splitted)
	int freeSpace = bufSpace(buf);
	if (freeSpace < 1){
			return -1;
	} else {
			// do nothing here, just continue
	}

	Vertices trVertices = {
							tr.points[0].x, tr.points[0].y, tr.points[0].z,
							tr.points[1].x, tr.points[1].y, tr.points[1].z,
							tr.points[2].x, tr.points[2].y, tr.points[2].z,
						  };	
	Colors trColors = {
						tr.points[0].color, 
						tr.points[1].color, 
						tr.points[2].color
					  };	
	pushBack(buf, &trVertices, &trColors);

	int overflow = 0; // Indicate that there is no more space in output buffer

	while (!bufIsEmpty(buf) && !overflow) {
		// Get the triangle out of the stack
		Vertices curTrVertices;
		Colors curTrColors;
		popBack(buf, &curTrVertices, &curTrColors);
		// Create triangle from vertices and colors
		// to pass it to the function
		Point a;
		Point b;
		Point c;

		a.x = curTrVertices.v[0]; 
		a.y = curTrVertices.v[1]; 
		a.z = curTrVertices.v[2]; 
		b.x = curTrVertices.v[3]; 
		b.y = curTrVertices.v[4]; 
		b.z = curTrVertices.v[5]; 
		c.x = curTrVertices.v[6]; 
		c.y = curTrVertices.v[7]; 
		c.z = curTrVertices.v[8]; 
		a.color = curTrColors.c[0]; 
		b.color = curTrColors.c[1]; 
		c.color = curTrColors.c[2]; 
		Triangle tr{a, b, c};

		// Process the triangle:
		Triangle trOut1;
		Triangle trOut2;
		// Check the size and split if it is necessary
		//int trIsSplitted = checkAndSplitFirstLargeEdge(triangle, xMax, yMax, trOut1, trOut2);
		int trIsSplitted = checkAndSplitLargestEdge(tr, xMax, yMax, trOut1, trOut2);
		// Triangle has been splitted
		if (trIsSplitted) {
			if (freeSpace >= 2){
				Vertices trOut1Vertices = {
											trOut1.points[0].x,
											trOut1.points[0].y,
											trOut1.points[0].z,
											trOut1.points[1].x,
											trOut1.points[1].y,
											trOut1.points[1].z,
											trOut1.points[2].x,
											trOut1.points[2].y,
											trOut1.points[2].z
										  };
				Colors trOut1Colors = {
											trOut1.points[0].color,
											trOut1.points[1].color,
											trOut1.points[2].color
										};
				pushBack(buf, &trOut1Vertices, &trOut1Colors);

				Vertices trOut2Vertices = {
											trOut2.points[0].x,
											trOut2.points[0].y,
											trOut2.points[0].z,
											trOut2.points[1].x,
											trOut2.points[1].y,
											trOut2.points[1].z,
											trOut2.points[2].x,
											trOut2.points[2].y,
											trOut2.points[2].z
										  };
				Colors trOut2Colors = {
											trOut2.points[0].color,
											trOut2.points[1].color,
											trOut2.points[2].color
										};
				pushBack(buf, &trOut2Vertices, &trOut2Colors);
			} else {
				// This triangle splitted is too big
				// There is no space in output buffer
				overflow = 1;
			}
		} else {
			// Triangle size is OK
			// Push the triangle to the output 
			// There is always place in output buffer
			// because one triangle is popped out of the buffer
			Vertices trVertices = {
										tr.points[0].x,
										tr.points[0].y,
										tr.points[0].z,
										tr.points[1].x,
										tr.points[1].y,
										tr.points[1].z,
										tr.points[2].x,
										tr.points[2].y,
										tr.points[2].z
									  };
			Colors trColors = {
										tr.points[0].color,
										tr.points[1].color,
										tr.points[2].color
									};

			pushFront(buf, &trVertices, &trColors);
		}		
	}

	// If there are no more triangles in the stack to divide
	if (bufIsEmpty(buf))
	{
		// All output triangles are in output buffer
		int ret = bufSize(buf);
		return ret; 
	} else {
		// 'While' is finished because there are no more free space
		// for the triangles
		return -1;
	}
}

#if 0
int checkAndSplitFirstLargeEdge(const Triangle& tr, 
								nm32f xMax, 
								nm32f yMax, 
								Triangle &trOut1, 
								Triangle& trOut2)
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
		return 1;
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
		trOut1 = Triangle{b, d, a};
		trOut2 = Triangle{c, d, a};
		return 1;
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
		return 1;
	}
	return 0;
}
#endif
int checkAndSplitLargestEdge(	const Triangle& tr, 
								nm32f xMax, 
								nm32f yMax, 
								Triangle &trOut1, 
								Triangle& trOut2)
{
	// If the largest edge is too large then division is necessary 
	edgeProjection edge0Projection = tr.edgeGetProjection(0);
	edgeProjection edge1Projection = tr.edgeGetProjection(1);
	edgeProjection edge2Projection = tr.edgeGetProjection(2);
	if (edge0Projection.dx > xMax || edge0Projection.dy > yMax ||
	    edge1Projection.dx > xMax || edge1Projection.dy > yMax ||
	    edge2Projection.dx > xMax || edge2Projection.dy > yMax) {
		nm32f largestEdgeSize = 0;
		int largestEdgeID = 0;
		// Find the largest edge
		for (int i = 0; i < 3; ++i) {
			nm32f currentEdgeSize = tr.edgeSize(i);
			if (currentEdgeSize > largestEdgeSize) {
				largestEdgeID = i;
				largestEdgeSize = currentEdgeSize;
			}
		}
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
		trOut1 = Triangle{ c, a, d };
		trOut2 = Triangle{ c, d, b };
		return 1;
	} else {
		return 0;
	}
}

