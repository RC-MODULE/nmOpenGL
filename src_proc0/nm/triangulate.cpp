#include <cmath> // pow for NMC-SDK

#include "service.h"

int checkAndSplitFirstLargeEdge(const Triangle& tr, nm32f xMax, nm32f yMax, Triangle &trOut1, Triangle& trOut2);
int checkAndSplitLargestEdge(const Triangle& tr, nm32f xMax, nm32f yMax, Triangle &trOut1, Triangle& trOut2);

nm32f Triangle::edgeSize(int i) const
{
	Point p1 = points[edges[i].p1];
	Point p2 = points[edges[i].p2];

	//nm32f dx = fabs(p1.x - p2.x);
	//nm32f dy = fabs(p1.y - p2.y);
	nm32f dx = (p1.x - p2.x);
	nm32f dy = (p1.y - p2.y);

	//nm32f size = sqrt(pow(dx, 2) + pow(dy, 2));
	//nm32f size = sqrt(dx * dx + dy * dy);
	nm32f size = dx * dx + dy * dy;
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
static int pushBackVertices(Buffer *vbuf, Vertices *vert)
{
	if (bufIsFull(vbuf)){
		return -1;
	} else {
		vbuf->back -= 1;
		nm32f *dst = (nm32f *) vbuf->data; 
		for (int i = 0; i < 9; ++i){
			dst[i * vbuf->size + vbuf->back] = vert->v[i];
		}	
		return 0;
	}
}

//      push here         
//          |
//          V
// |x|x|x|x|-|-|-|
// |x|x|x|x|-|-|-|
// |x|x|x|x|-|-|-|
static int pushFrontVertices(Buffer *vbuf, Vertices *vert)
{
	if (bufIsFull(vbuf)){
		return -1;
	} else {
		vbuf->front += 1;
		nm32f *dst = (nm32f *) vbuf->data; 
		for (int i = 0; i < 9; ++i){
			dst[i * vbuf->size + vbuf->front] = vert->v[i];
		}	
		return 0;
	}
}

//   return this item         
//          |
//          V
// |-|-|-|-|x|x|x|
// |-|-|-|-|x|x|x|
// |-|-|-|-|x|x|x|
static int popBackVertices(Buffer *vbuf, Vertices *vert)
{
	if (bufIsEmpty(vbuf)){
		return -1;
	} else {
		nm32f *src = (nm32f *) vbuf->data; 
		for (int i = 0; i < 9; ++i){
			vert->v[i] = src[i * vbuf->size + vbuf->back];
		}	
		vbuf->back += 1;
		return 0;
	}
}

// 				|-|-|-|-|-|-|-|
// push here ->	|-|-|-|-|-|-|-|
// 				|x|x|x|x|x|x|x|
// 				|x|x|x|x|x|x|x|
static int pushBackColors(Buffer *cbuf, Colors *colors)
{
	if (bufIsFull(cbuf)){
		return -1;
	} else {
		cbuf->back -= 1;
		Colors *topColor = &((Colors *) cbuf->data)[cbuf->back];
		nm32f *dst = (nm32f *) topColor;
		for (int i = 0; i < 3; ++i){
			for (int j = 0; j < 4; ++j){
				dst[i * 4 + j] = colors->c[i].vec[j];
			}
		}
		return 0;
	}
}

// 				|x|x|x|x|x|x|x|
// 				|x|x|x|x|x|x|x|
// push here ->	|-|-|-|-|-|-|-|
// 				|-|-|-|-|-|-|-|
static int pushFrontColors(Buffer *cbuf, Colors *colors)
{
	if (bufIsFull(cbuf)){
		return -1;
	} else {
		cbuf->front += 1;
		Colors *frontColor = &((Colors *) cbuf->data)[cbuf->front];
		nm32f *dst = (nm32f *) frontColor;
		for (int i = 0; i < 3; ++i){
			for (int j = 0; j < 4; ++j){
				dst[i * 4 + j] = colors->c[i].vec[j];
			}
		}
		return 0;
	}
}

// 						|-|-|-|-|-|-|-|
// return this item ->	|x|x|x|x|x|x|x|
// 						|x|x|x|x|x|x|x|
// 						|x|x|x|x|x|x|x|
static int popBackColors(Buffer *cbuf, Colors *colors)
{
	if (bufIsEmpty(cbuf)){
		return -1;
	} else {
		Colors *topColor = &((Colors *) cbuf->data)[cbuf->back];
		nm32f *src = (nm32f *) topColor;
		for (int i = 0; i < 3; ++i){
			for (int j = 0; j < 4; ++j){
				colors->c[i].vec[j] = src[i * 4 + j];
			}
		}
		cbuf->back += 1;
		return 0;
	}
}

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
	int currentDstSize = 0;

	Buffer verticesStack = initBuf((void *) dstVertex, maxDstSize);
	Buffer colorsStack = initBuf((void *) dstColor, maxDstSize);

	int i = 0; // make this iterator local to assign it later to srcTreatedCount
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
		int res = triangulateOneTriangle(tr, 
										(nm32f) maxWidth, 
										(nm32f) maxHeight, 
										&verticesStack, 
										&colorsStack);
		// If the number of smaller triangles is too big
		if (res == -1) {
			// Finish triangulation
			break;
		} else {
			// Update the number of the result output triangles
			// in case of successful split
			// (triangulateOneTriangle modifies output buffer, so
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
int triangulateOneTriangle(	const Triangle& tr, 
							nm32f xMax, 
							nm32f yMax, 
							Buffer *verticesStack, 
							Buffer *colorsStack)
{
	// Check the free space in output buffer
	// (atleast for input triangle if it is OK and don't be splitted)
	int vsize = bufSpace(verticesStack);
	if (vsize < 1){
			return -1;
	} else {
			// do nothing here, just continue
	}

	Colors trColors = {
						tr.points[0].color, 
						tr.points[1].color, 
						tr.points[2].color
					  };	
	pushBackColors(colorsStack, &trColors);

	Vertices trVertices = {
							tr.points[0].x, tr.points[0].y, tr.points[0].z,
							tr.points[1].x, tr.points[1].y, tr.points[1].z,
							tr.points[2].x, tr.points[2].y, tr.points[2].z,
						  };	
	pushBackVertices(verticesStack, &trVertices);

	int overflow = 0; // Indicate that there is no more space in output buffer

	while (!bufIsEmpty(verticesStack) && !overflow) {
		// Get the triangle out of the stack
		Vertices curTrVertices;
		Colors curTrColors;
		popBackVertices(verticesStack, &curTrVertices);
		popBackColors(colorsStack, &curTrColors);
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
			if (vsize >= 2){
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
				pushBackVertices(verticesStack, &trOut1Vertices);
				pushBackColors(colorsStack, &trOut1Colors);

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
				pushBackVertices(verticesStack, &trOut2Vertices);
				pushBackColors(colorsStack, &trOut2Colors);
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

			pushFrontVertices(verticesStack, &trVertices);
			pushFrontColors(colorsStack, &trColors);
		}		
	}

	// If there are no more triangles in the stack to divide
	if (bufIsEmpty(verticesStack))
	{
		// All output triangles are in output buffer
		int ret = bufSize(verticesStack);
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
	// If the largest edge is too large then division is necessary 
	edgeProjection largestEdgeProjection = tr.edgeGetProjection(largestEdgeID);
	if (largestEdgeProjection.dx > xMax || largestEdgeProjection.dy > yMax) {
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
		return 1;
	} else {
		return 0;
	}
}

