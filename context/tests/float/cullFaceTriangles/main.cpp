#include "raster_float.h"
#include "uassert.h"
#include "nmgldef.h"
#include "context_float.h"

#define MAX_TEST_SIZE 96
float x0[NMGL_SIZE];
float y0[NMGL_SIZE];
float z0[NMGL_SIZE];
float w0[NMGL_SIZE];
float s0[NMGL_SIZE];
float t0[NMGL_SIZE];
v4nm32f color0[NMGL_SIZE];

float x1[NMGL_SIZE];
float y1[NMGL_SIZE];
float z1[NMGL_SIZE];
float w1[NMGL_SIZE];
float s1[NMGL_SIZE];
float t1[NMGL_SIZE];
v4nm32f color1[NMGL_SIZE];

float x2[NMGL_SIZE];
float y2[NMGL_SIZE];
float z2[NMGL_SIZE];
float w2[NMGL_SIZE];
float s2[NMGL_SIZE];
float t2[NMGL_SIZE];
v4nm32f color2[NMGL_SIZE];

static void setValue(TrianglePointers &triangles, int item, v2nm32f *triangle){
	triangles.v0.x[item] = triangle[0].v0;
	triangles.v0.y[item] = triangle[0].v1;
	triangles.v1.x[item] = triangle[1].v0;
	triangles.v1.y[item] = triangle[1].v1;
	triangles.v2.x[item] = triangle[2].v0;
	triangles.v2.y[item] = triangle[2].v1;
}

void printTriangles(TrianglePointers &points, int size, const char* name){
	for(int i=0;i<size;i++){
		DEBUG_PLOG_LEVEL_4("%s[%4d]: ", name, i);
		DEBUG_PLOG_LEVEL_4("{%f, %f}, ", points.v0.x[i], points.v0.y[i]);
		DEBUG_PLOG_LEVEL_4("{%f, %f}, ", points.v1.x[i], points.v1.y[i]);
		DEBUG_PLOG_LEVEL_4("{%f, %f}\n", points.v2.x[i], points.v2.y[i]);
	}
}

static bool checkValue(TrianglePointers &triangles, int item, v2nm32f *triangle){
	bool v0x = triangles.v0.x[item] == triangle[0].v0;
	bool v0y = triangles.v0.y[item] == triangle[0].v1;
	bool v1x = triangles.v1.x[item] == triangle[1].v0;
	bool v1y = triangles.v1.y[item] == triangle[1].v1;
	bool v2x = triangles.v2.x[item] == triangle[2].v0;
	bool v2y = triangles.v2.y[item] == triangle[2].v1;
	return v0x && v0y && v1x && v1y && v2x && v2y;
}


void testValues(TrianglePointers &pointers, NMGLenum frontface, NMGLenum cullface, v2nm32f *passTriangle, v2nm32f *failTriangle, const char* name){
	for(int size = 0; size < MAX_TEST_SIZE; size+=2){
		DEBUG_PLOG_LEVEL_2("size=%d\n", size);
		{
			for(int i = 0; i < size; i++){
				setValue(pointers, i, failTriangle);
			}
			int count = cullFaceSortTriangles(pointers, size, frontface, cullface);	
			DEBUG_PLOG_LEVEL_2("count=%d\n", count);
			uassert(count == 0);
		}

		{
			for(int i = 0; i < size; i++){
				setValue(pointers, i, passTriangle);
			}
			int count = cullFaceSortTriangles(pointers, size, frontface, cullface);	

			DEBUG_PLOG_LEVEL_2("count=%d\n", count);
			uassert(count == size);

			for(int i = 0; i < count; i++){
				uassert(checkValue(pointers, i, passTriangle));
			}
		}

		{
			for(int i = 0; i < size; i++){
				if(i % 2){
					setValue(pointers, i, passTriangle);
				}else{
					setValue(pointers, i, failTriangle);	
				}
			}

			int count = cullFaceSortTriangles(pointers, size, frontface, cullface);	

			DEBUG_PLOG_LEVEL_2("count=%d\n", count);
			int halfsize = size / 2;
			halfsize = (halfsize % 2) ? halfsize + 1: halfsize;
			uassert(count == halfsize);
			for(int i = 0; i < count; i++){
				uassert(checkValue(pointers, i, passTriangle));
			}
		}
	}
	
	DEBUG_TEST_OK();
}

void testValuesFaB(TrianglePointers &pointers, v2nm32f *triangle_ccw, v2nm32f *triangle_cw){
	for(int size = 0; size < MAX_TEST_SIZE; size+=2){
		DEBUG_PLOG_LEVEL_2("size=%d\n", size);
		{
			for(int i = 0; i < size; i++){
				setValue(pointers, i, triangle_ccw);
			}
			int count = cullFaceSortTriangles(pointers, size, NMGL_CCW, NMGL_FRONT_AND_BACK);	
			uassert(count == 0);
		}
		{
			for(int i = 0; i < size; i++){
				setValue(pointers, i, triangle_ccw);
			}
			int count = cullFaceSortTriangles(pointers, size, NMGL_CW, NMGL_FRONT_AND_BACK);	
			uassert(count == 0);
		}
		
		{
			for(int i = 0; i < size; i++){
				setValue(pointers, i, triangle_cw);
			}
			int count = cullFaceSortTriangles(pointers, size, NMGL_CCW, NMGL_FRONT_AND_BACK);	
			uassert(count == 0);
		}
		{
			for(int i = 0; i < size; i++){
				setValue(pointers, i, triangle_cw);
			}
			int count = cullFaceSortTriangles(pointers, size, NMGL_CW, NMGL_FRONT_AND_BACK);	
			uassert(count == 0);
		}
		
		{
			for(int i = 0; i < size; i++){
				if(i % 2){
					setValue(pointers, i, triangle_ccw);
				}else{
					setValue(pointers, i, triangle_cw);	
				}
			}
			int count = cullFaceSortTriangles(pointers, size, NMGL_CCW, NMGL_FRONT_AND_BACK);	
			uassert(count == 0);
		}
		{
			for(int i = 0; i < size; i++){
				if(i % 2){
					setValue(pointers, i, triangle_ccw);
				}else{
					setValue(pointers, i, triangle_cw);	
				}
			}
			int count = cullFaceSortTriangles(pointers, size, NMGL_CW, NMGL_FRONT_AND_BACK);	
			uassert(count == 0);
		}
	}

	DEBUG_TEST_OK();
}

int main(){
	
	DEBUG_PLOG_FILE();
	
	TrianglePointers pointers;
	

	pointers.v0.x = x0;
	pointers.v0.y = y0;
	pointers.v0.z = z0;
	pointers.v0.w = w0;
	pointers.v0.s = s0;
	pointers.v0.t = t0;
	pointers.v0.color = color0;

	pointers.v1.x = x1;
	pointers.v1.y = y1;
	pointers.v1.z = z1;
	pointers.v1.w = w1;
	pointers.v1.s = s1;
	pointers.v1.t = t1;
	pointers.v1.color = color1;

	pointers.v2.x = x2;
	pointers.v2.y = y2;
	pointers.v2.z = z2;
	pointers.v2.w = w2;
	pointers.v2.s = s2;
	pointers.v2.t = t2;
	pointers.v2.color = color2;

	v2nm32f triangle_cw[] = { {1, 1}, {1, 0}, {0, 0} };
	v2nm32f triangle_ccw[] = { {1, 1}, {0, 0}, {1, 0} };

	testValues(pointers, NMGL_CCW , NMGL_BACK , triangle_ccw, triangle_cw, "CCW Back");
	testValues(pointers, NMGL_CW  , NMGL_BACK , triangle_cw, triangle_ccw, "CW Back");
	testValues(pointers, NMGL_CCW , NMGL_FRONT, triangle_cw, triangle_ccw, "CCW Front");
	testValues(pointers, NMGL_CW  , NMGL_FRONT, triangle_ccw, triangle_cw, "CW Front");
	//testValuesFaB(pointers, triangle_ccw, triangle_cw);
	
	return 0;
}