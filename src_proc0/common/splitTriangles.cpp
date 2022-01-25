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
// TEXTURING_PART without texcoords and wclips
static int pushBack(Buffer *buf, Vertices *vert, Colors *colors, Texcoords *texcoords, Wclips *wclips)
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

		//TEXTURING_PART
		trPtr->v0.w[buf->back] = wclips->w[0];
		trPtr->v1.w[buf->back] = wclips->w[1];
		trPtr->v2.w[buf->back] = wclips->w[2];
		trPtr->v0.s[buf->back] = texcoords->st[0];
		trPtr->v0.t[buf->back] = texcoords->st[1];
		trPtr->v1.s[buf->back] = texcoords->st[2];
		trPtr->v1.t[buf->back] = texcoords->st[3];
		trPtr->v2.s[buf->back] = texcoords->st[4];
		trPtr->v2.t[buf->back] = texcoords->st[5];
		//TEXTURING_PART
		return 0;
	}
}

//   return this item         
//          |
//          V
// |-|-|-|-|x|x|x|
// |-|-|-|-|x|x|x|
// |-|-|-|-|x|x|x|
// TEXTURING_PART without texcoords and wclips
static int popBack(Buffer *buf, Vertices *vert, Colors *colors, Texcoords *texcoords, Wclips *wclips)
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
		
		//TEXTURING_PART
		wclips->w[0] = trPtr->v0.w[buf->back];
		wclips->w[1] = trPtr->v1.w[buf->back];
		wclips->w[2] = trPtr->v2.w[buf->back];
		texcoords->st[0] = trPtr->v0.s[buf->back];
		texcoords->st[1] = trPtr->v0.t[buf->back];
		texcoords->st[2] = trPtr->v1.s[buf->back];
		texcoords->st[3] = trPtr->v1.t[buf->back];
		texcoords->st[4] = trPtr->v2.s[buf->back];
		texcoords->st[5] = trPtr->v2.t[buf->back];
		//TEXTURING_PART
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
// TEXTURING_PART without texcoords and wclips
static int pushFront(Buffer *buf, Vertices *vert, Colors *colors, Texcoords *texcoords, Wclips *wclips)
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
		//TEXTURING_PART
		trPtr->v0.w[buf->front] = wclips->w[0];
		trPtr->v1.w[buf->front] = wclips->w[1];
		trPtr->v2.w[buf->front] = wclips->w[2];
		trPtr->v0.s[buf->front] = texcoords->st[0];
		trPtr->v0.t[buf->front] = texcoords->st[1];
		trPtr->v1.s[buf->front] = texcoords->st[2];
		trPtr->v1.t[buf->front] = texcoords->st[3];
		trPtr->v2.s[buf->front] = texcoords->st[4];
		trPtr->v2.t[buf->front] = texcoords->st[5];
		//TEXTURING_PART
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

    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	int i = *srcTreatedCount; // make this iterator local to assign it later to srcTreatedCount
	for (i = *srcTreatedCount; i < srcCount; ++i) {
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
		if (NMGL_SMOOTH == context->shadeModel) {
			a.color = srcVertex->v0.color[i];
			b.color = srcVertex->v1.color[i];
			c.color = srcVertex->v2.color[i];
		}
		else {
			a.color = srcVertex->v2.color[i];
			b.color = srcVertex->v2.color[i];
			c.color = srcVertex->v2.color[i];
		}
		//TEXTURING_PART
		a.w = srcVertex->v0.w[i];
		b.w = srcVertex->v1.w[i];
		c.w = srcVertex->v2.w[i];
		a.s = srcVertex->v0.s[i];
		a.t = srcVertex->v0.t[i];
		b.s = srcVertex->v1.s[i];
		b.t = srcVertex->v1.t[i];
		c.s = srcVertex->v2.s[i];
		c.t = srcVertex->v2.t[i];
		//TEXTURING_PART
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
	// TEXTURING_PART
	Texcoords trTexcoords = {
		                      tr.points[0].s,
		                      tr.points[0].t,
		                      tr.points[1].s,
		                      tr.points[1].t,
		                      tr.points[2].s,
		                      tr.points[2].t
	                        };

	Wclips trWclips = {
                       	tr.points[0].w,
                    	tr.points[1].w,
                    	tr.points[2].w
                      };

	pushBack(buf, &trVertices, &trColors, &trTexcoords, &trWclips);
	// TEXTURING_PART

	int overflow = 0; // Indicate that there is no more space in output buffer

	while (!bufIsEmpty(buf) && !overflow) {
		// Get the triangle out of the stack
		Vertices curTrVertices;
		Colors curTrColors;
		//TEXTURING_PART
		Texcoords curTrTexcoords;
		Wclips curTrWclips;
		popBack(buf, &curTrVertices, &curTrColors, &curTrTexcoords, &curTrWclips);
		//TEXTURING_PART
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
		//TEXTURING_PART
		a.w = curTrWclips.w[0];
		b.w = curTrWclips.w[1];
		c.w = curTrWclips.w[2];
		a.s = curTrTexcoords.st[0];
		a.t = curTrTexcoords.st[1];
		b.s = curTrTexcoords.st[2];
		b.t = curTrTexcoords.st[3];
		c.s = curTrTexcoords.st[4];
		c.t = curTrTexcoords.st[5];
		//TEXTURING_PART
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
				// TEXTURING_PART
				Texcoords trOut1Texcoords = {
											trOut1.points[0].s,
											trOut1.points[0].t,
											trOut1.points[1].s,
											trOut1.points[1].t,
											trOut1.points[2].s,
											trOut1.points[2].t
										};
                                        
				Wclips trOut1Wclips = {
											trOut1.points[0].w,
											trOut1.points[1].w,
											trOut1.points[2].w
										};

				pushBack(buf, &trOut1Vertices, &trOut1Colors, &trOut1Texcoords, &trOut1Wclips);
				// TEXTURING_PART
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
				//TEXTURING_PART
				Texcoords trOut2Texcoords = {
											trOut2.points[0].s,
											trOut2.points[0].t,
											trOut2.points[1].s,
											trOut2.points[1].t,
											trOut2.points[2].s,
											trOut2.points[2].t
										};
                                        
				Wclips trOut2Wclips = {
											trOut2.points[0].w,
											trOut2.points[1].w,
											trOut2.points[2].w
									  };
				pushBack(buf, &trOut2Vertices, &trOut2Colors, &trOut2Texcoords, &trOut2Wclips);
				//TEXTURING_PART
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
			//TEXTURING_PART
			Texcoords trTexcoords = {
				                      tr.points[0].s,
				                      tr.points[0].t,
				                      tr.points[1].s,
				                      tr.points[1].t,
				                      tr.points[2].s,
				                      tr.points[2].t
			                        };

			Wclips trWclips = {
				                tr.points[0].w,
				                tr.points[1].w,
				                tr.points[2].w
			                  };

			pushFront(buf, &trVertices, &trColors, &trTexcoords, &trWclips);
			//TEXTURING_PART

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
// TEXTURING_PART	
#if defined (PERSPECTIVE_CORRECT_TRIANGULATION)
		//XXX: Warning. Potential division by zero
		float oneOverWa = 1.0 / a.w;
		float oneOverWb = 1.0 / b.w;
		float oneOverW = 1.0 / (oneOverWa + oneOverWb);

		d.color.vec[0] = (a.color.vec[0] * oneOverWa + b.color.vec[0] * oneOverWb) * oneOverW;
		d.color.vec[1] = (a.color.vec[1] * oneOverWa + b.color.vec[1] * oneOverWb) * oneOverW;
		d.color.vec[2] = (a.color.vec[2] * oneOverWa + b.color.vec[2] * oneOverWb) * oneOverW;
		d.color.vec[3] = (a.color.vec[3] * oneOverWa + b.color.vec[3] * oneOverWb) * oneOverW;
#else //PERSPECTIVE_CORRECT_TRIANGULATION
		d.color.vec[0] = (a.color.vec[0] + b.color.vec[0]) * 0.5;
		d.color.vec[1] = (a.color.vec[1] + b.color.vec[1]) * 0.5;
		d.color.vec[2] = (a.color.vec[2] + b.color.vec[2]) * 0.5;
		d.color.vec[3] = (a.color.vec[3] + b.color.vec[3]) * 0.5;
#endif //PERSPECTIVE_CORRECT_TRIANGULATION

// TEXTURING_PART
#ifdef PERSPECTIVE_CORRECT_TRIANGULATION
		d.w = (a.w * oneOverWa + b.w * oneOverWb) * oneOverW;
#else //PERSPECTIVE_CORRECT_TRIANGULATION
		d.w = (a.w + b.w) * 0.5;
#endif //PERSPECTIVE_CORRECT_TRIANGULATION

#ifdef PERSPECTIVE_CORRECT_TRIANGULATION
		d.s = (a.s * oneOverWa + b.s * oneOverWb) * oneOverW;
		d.t = (a.t * oneOverWa + b.t * oneOverWb) * oneOverW;
#else //PERSPECTIVE_CORRECT_TRIANGULATION
		d.s = (a.s + b.s) * 0.5;
		d.t = (a.t + b.t) * 0.5;
#endif //PERSPECTIVE_CORRECT_TRIANGULATION
// TEXTURING_PART

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

#if defined (PERSPECTIVE_CORRECT_TRIANGULATION)
		//XXX: Warning. Potential division by zero
		float oneOverWb = 1.0 / b.w;
		float oneOverWc = 1.0 / c.w;
		float oneOverW = 1.0 / (oneOverWb + oneOverWc);

		d.color.vec[0] = (b.color.vec[0] * oneOverWb + c.color.vec[0] * oneOverWc) * oneOverW;
		d.color.vec[1] = (b.color.vec[1] * oneOverWb + c.color.vec[1] * oneOverWc) * oneOverW;
		d.color.vec[2] = (b.color.vec[2] * oneOverWb + c.color.vec[2] * oneOverWc) * oneOverW;
		d.color.vec[3] = (b.color.vec[3] * oneOverWb + c.color.vec[3] * oneOverWc) * oneOverW;
#else //PERSPECTIVE_CORRECT_TRIANGULATION
		d.color.vec[0] = (b.color.vec[0] + c.color.vec[0]) * 0.5;
		d.color.vec[1] = (b.color.vec[1] + c.color.vec[1]) * 0.5;
		d.color.vec[2] = (b.color.vec[2] + c.color.vec[2]) * 0.5;
		d.color.vec[3] = (b.color.vec[3] + c.color.vec[3]) * 0.5;
#endif //PERSPECTIVE_CORRECT_TRIANGULATION

//TEXTURING_PART
#ifdef PERSPECTIVE_CORRECT_TRIANGULATION
		d.w = (b.w * oneOverWb + c.w * oneOverWc) * oneOverW;
#else //PERSPECTIVE_CORRECT_TRIANGULATION
		d.w = (b.w + c.w) * 0.5;
#endif //PERSPECTIVE_CORRECT_TRIANGULATION

#ifdef PERSPECTIVE_CORRECT_TRIANGULATION
		d.s = (b.s * oneOverWb + c.s * oneOverWc) * oneOverW;
		d.t = (b.t * oneOverWb + c.t * oneOverWc) * oneOverW;
#else //PERSPECTIVE_CORRECT_TRIANGULATION
		d.s = (b.s + c.s) * 0.5;
		d.t = (b.t + c.t) * 0.5;
#endif //PERSPECTIVE_CORRECT_TRIANGULATION
//TEXTURING_PART

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
		
//TEXTURING_PART
#if defined (PERSPECTIVE_CORRECT_TRIANGULATION)
		//XXX: Warning. Potential division by zero
		float oneOverWa = 1.0 / a.w;
		float oneOverWc = 1.0 / c.w;
		float oneOverW = 1.0 / (oneOverWa + oneOverWc);
		
		d.color.vec[0] = (a.color.vec[0] * oneOverWa + c.color.vec[0] * oneOverWc) * oneOverW;
		d.color.vec[1] = (a.color.vec[1] * oneOverWa + c.color.vec[1] * oneOverWc) * oneOverW;
		d.color.vec[2] = (a.color.vec[2] * oneOverWa + c.color.vec[2] * oneOverWc) * oneOverW;
		d.color.vec[3] = (a.color.vec[3] * oneOverWa + c.color.vec[3] * oneOverWc) * oneOverW;
#else //PERSPECTIVE_CORRECT_TRIANGULATION
		d.color.vec[0] = (a.color.vec[0] + c.color.vec[0]) * 0.5;
		d.color.vec[1] = (a.color.vec[1] + c.color.vec[1]) * 0.5;
		d.color.vec[2] = (a.color.vec[2] + c.color.vec[2]) * 0.5;
		d.color.vec[3] = (a.color.vec[3] + c.color.vec[3]) * 0.5;
#endif //PERSPECTIVE_CORRECT_TRIANGULATION
		
//TEXTURING_PART
#ifdef PERSPECTIVE_CORRECT_TRIANGULATION
		d.w = (a.w * oneOverWa + c.w * oneOverWc) * oneOverW;
#else //PERSPECTIVE_CORRECT_TRIANGULATION
		d.w = (a.w + c.w) * 0.5;
#endif //PERSPECTIVE_CORRECT_TRIANGULATION

#ifdef PERSPECTIVE_CORRECT_TRIANGULATION
		d.s = (a.s * oneOverWa + c.s * oneOverWc) * oneOverW;
		d.t = (a.t * oneOverWa + c.t * oneOverWc) * oneOverW;
#else //PERSPECTIVE_CORRECT_TRIANGULATION
		d.s = (a.s + c.s) * 0.5;
		d.t = (a.t + c.t) * 0.5;
#endif //PERSPECTIVE_CORRECT_TRIANGULATION
//TEXTURING_PART

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

    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

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

#ifdef PERSPECTIVE_CORRECT_TRIANGULATION
		//XXX: Warning. Potential division by zero
		float oneOverWa = 1.0 / a.w;
		float oneOverWb = 1.0 / b.w;
		float oneOverW = 1.0 / (oneOverWa + oneOverWb);
#endif //PERSPECTIVE_CORRECT_TRIANGULATION

		if (NMGL_FLAT == context->shadeModel) {
			d.color.vec[0] = a.color.vec[0];
			d.color.vec[1] = a.color.vec[1];
			d.color.vec[2] = a.color.vec[2];
			d.color.vec[3] = a.color.vec[3];
		} else {
#ifdef PERSPECTIVE_CORRECT_TRIANGULATION
			d.color.vec[0] = (a.color.vec[0] * oneOverWa + b.color.vec[0] * oneOverWb) * oneOverW;
			d.color.vec[1] = (a.color.vec[1] * oneOverWa + b.color.vec[1] * oneOverWb) * oneOverW;
			d.color.vec[2] = (a.color.vec[2] * oneOverWa + b.color.vec[2] * oneOverWb) * oneOverW;
			d.color.vec[3] = (a.color.vec[3] * oneOverWa + b.color.vec[3] * oneOverWb) * oneOverW;
#else //PERSPECTIVE_CORRECT_TRIANGULATION
			d.color.vec[0] = (a.color.vec[0] + b.color.vec[0]) * 0.5;
			d.color.vec[1] = (a.color.vec[1] + b.color.vec[1]) * 0.5;
			d.color.vec[2] = (a.color.vec[2] + b.color.vec[2]) * 0.5;
			d.color.vec[3] = (a.color.vec[3] + b.color.vec[3]) * 0.5;
#endif //PERSPECTIVE_CORRECT_TRIANGULATION
		}
//TEXTURING_PART
#ifdef PERSPECTIVE_CORRECT_TRIANGULATION
		d.w = (a.w * oneOverWa + b.w * oneOverWb) * oneOverW;
#else //PERSPECTIVE_CORRECT_TRIANGULATION
		d.w = (a.w + b.w) * 0.5;
#endif //PERSPECTIVE_CORRECT_TRIANGULATION

#ifdef PERSPECTIVE_CORRECT_TRIANGULATION
		d.s = (a.s * oneOverWa + b.s * oneOverWb) * oneOverW;
		d.t = (a.t * oneOverWa + b.t * oneOverWb) * oneOverW;
#else //PERSPECTIVE_CORRECT_TRIANGULATION
		d.s = (a.s + b.s) * 0.5;
		d.t = (a.t + b.t) * 0.5;
#endif //PERSPECTIVE_CORRECT_TRIANGULATION
//TEXTURING_PART
			
		trOut1 = Triangle{ c, a, d };
		trOut2 = Triangle{ c, d, b };
		return 1;
	} else {
		return 0;
	}
}

