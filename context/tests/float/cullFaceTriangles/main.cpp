#include "raster_float.h"
#include "stdio.h"

float x0[1024];
float y0[1024];
float x1[1024];
float y1[1024];
float x2[1024];
float y2[1024];

int main(){
	NMGL_CullFace cullface;
	
	TrianglePointers pointers;
	pointers.v0.x = x0;
	pointers.v0.y = y0;
	pointers.v1.x = x1;
	pointers.v1.y = y1;
	pointers.v2.x = x2;
	pointers.v2.y = y2;
	

	
	cullFaceSortTriangles(cullface, pointers, 2);
	printf("asdasd\n");
	return 0;
}