#include <iostream>
#include <cassert>
#include <cerrno>

#include "service.h"
#include "demo3d_nm0.h"
#include "tests.h"
#include "nmtype.h"
#include "nmgl.h"

// Performance testing
#include "time.h"


void Print(nm32f *ptr, size_t size);
void PrintV4nm32f(v4nm32f *ptr, size_t size);
void ZeroV4nm32f(v4nm32f *ptr, size_t size);
void initializeInput(v4nm32f *srcVertex, v4nm32f *srcColor, v2nm32f *srcTex, int vertCount);
void setDst(TrianglePointers *dst, nm32f *buf, int outputTrianglesCount);
int repackToPrimitives_tf_nOutputTriangles_AllDataAreCorrect(int n);
int repackToPrimitives_tf_nOutputTriangles_v0CoordsAreCorrect(int n);
int repackToPrimitives_tf_nOutputTriangles_v0ColorsAreCorrect(int n);
int repackToPrimitives_tf_nOutputTriangles_v1CoordsAreCorrect(int n);
int repackToPrimitives_tf_nOutputTriangles_v1ColorsAreCorrect(int n);
int repackToPrimitives_tf_nOutputTriangles_v2CoordsAreCorrect(int n);
int repackToPrimitives_tf_nOutputTriangles_v2ColorsAreCorrect(int n);
int repackToPrimitives_tf_nOutputTriangles_v0TextureCoordsAreCorrect(int n);
int repackToPrimitives_tf_nOutputTriangles_v1TextureCoordsAreCorrect(int n);
int repackToPrimitives_tf_nOutputTriangles_v2TextureCoordsAreCorrect(int n);
int repackToPrimitives_tf_0_200_OutputTriangles_AllDataAreCorrect();
int repackToPrimitives_tf_0_200_OutputTriangles_v0CoordsAreCorrect();
int repackToPrimitives_tf_0_200_OutputTriangles_v1CoordsAreCorrect();
int repackToPrimitives_tf_0_200_OutputTriangles_v2CoordsAreCorrect();
int repackToPrimitives_tf_0_200_OutputTriangles_v0ColorsAreCorrect();
int repackToPrimitives_tf_0_200_OutputTriangles_v1ColorsAreCorrect();
int repackToPrimitives_tf_0_200_OutputTriangles_v2ColorsAreCorrect();
int repackToPrimitives_tf_0_200_OutputTriangles_v0TextureCoordsAreCorrect();
int repackToPrimitives_tf_0_200_OutputTriangles_v1TextureCoordsAreCorrect();
int repackToPrimitives_tf_0_200_OutputTriangles_v2TextureCoordsAreCorrect();
//Performance tests
clock_t repackToPrimitives_tf_192Vertices();
clock_t repackToPrimitives_tf_48Vertices();

// TEMP test
//int repackToPrimitives_tf(const v4nm32f *srcVertex,
//	const v4nm32f *srcColor,
//	const v2nm32f *srcTex,
//	TrianglePointers *dstVertex,
//	int vertexAmount);
//


void Print(nm32f *ptr, size_t size)
{
	for (int i = 0; i < size; ++i){
		printf("%4.1f ", ptr[i]);
	}
	puts("");
}

void PrintV4nm32f(v4nm32f *ptr, size_t size)
{
	for (int i = 0; i < size; ++i){
		for (int j = 0; j < 4; ++j){
			printf("%4.1f ", ptr[i].vec[j]);
		}
	}
	puts("");
}

void ZeroV4nm32f(v4nm32f *ptr, size_t size)
{
	for (int i = 0; i < size; ++i){
		for (int j = 0; j < 4; ++j){
			ptr[i].vec[j] = 0.0;
		}
	}
}


int main(int argc, char **argv)
{
    //puts("VertexPrimitiveRepack tests: ");
	RUN_TEST(repackToPrimitives_tf_0_200_OutputTriangles_AllDataAreCorrect);
	RUN_TEST(repackToPrimitives_tf_0_200_OutputTriangles_v0CoordsAreCorrect);
	RUN_TEST(repackToPrimitives_tf_0_200_OutputTriangles_v1CoordsAreCorrect);
	RUN_TEST(repackToPrimitives_tf_0_200_OutputTriangles_v2CoordsAreCorrect);
	RUN_TEST(repackToPrimitives_tf_0_200_OutputTriangles_v0ColorsAreCorrect);
	RUN_TEST(repackToPrimitives_tf_0_200_OutputTriangles_v1ColorsAreCorrect);
	RUN_TEST(repackToPrimitives_tf_0_200_OutputTriangles_v2ColorsAreCorrect);
	RUN_TEST(repackToPrimitives_tf_0_200_OutputTriangles_v0TextureCoordsAreCorrect);
	RUN_TEST(repackToPrimitives_tf_0_200_OutputTriangles_v1TextureCoordsAreCorrect);
	RUN_TEST(repackToPrimitives_tf_0_200_OutputTriangles_v2TextureCoordsAreCorrect);
	//RUN_ARG_TEST(repackToPrimitives_tf_nOutputTriangles, 65);

	puts("");
    puts("Performance tests: ");
	clock_t dt[2];
	dt[0] = repackToPrimitives_tf_192Vertices();
	dt[1] = repackToPrimitives_tf_48Vertices();
	for (int i = 0; i < 2; ++i){
		printf("dt[%i] = %i\n\r", i + 1, (int)dt[i]);
	}

    puts("OK");
	return 0;
}

// TEXTURING_PART
#define ATTR_PER_VERTEX 10 // with texture coords
//#define ATTR_PER_VERTEX 8	// without texture coords

void initializeInput(v4nm32f *srcVertex, v4nm32f *srcColor, v2nm32f *srcTex, int vertCount)
{
    for (int i = 0; i < vertCount; i++){
        for (int j = 0; j < 4; j++){
            srcVertex[i].vec[j] = (float)(4 * i + j);
            srcColor[i].vec[j] = (float)(4 * i + j);
		}
	}
// TEXTURING_PART
    for (int i = 0; i < vertCount; i++){
		srcTex[i].v0 = (float)(2 * i);
		srcTex[i].v1 = (float)(2 * i + 1);
	}
// TEXTURING_PART
}

void initializeExpectedOutput(TrianglePointers *dst_exp, 
		v4nm32f *srcVertex, 
		v4nm32f *srcColor, 
		v2nm32f *srcTex, 
		int trianglesCount, 
		int expectedTrianglesCount,
		int outputTrianglesCount)
{
	if (trianglesCount > 0){
		for (int i = 0; i < trianglesCount; i++){
			dst_exp->v0.x[i] = srcVertex[0].vec[0];
			dst_exp->v0.y[i] = srcVertex[0].vec[1];
			dst_exp->v0.z[i] = srcVertex[0].vec[2];
			dst_exp->v0.w[i] = srcVertex[0].vec[3];
			// TEXTURING_PART
			dst_exp->v0.s[i] = srcTex[0].v0;
			dst_exp->v0.t[i] = srcTex[0].v1;
			// TEXTURING_PART
			dst_exp->v0.color[i].vec[0] = srcColor[0].vec[0];
			dst_exp->v0.color[i].vec[1] = srcColor[0].vec[1];
			dst_exp->v0.color[i].vec[2] = srcColor[0].vec[2];
			dst_exp->v0.color[i].vec[3] = srcColor[0].vec[3];

			dst_exp->v1.x[i] = srcVertex[i + 1].vec[0];
			dst_exp->v1.y[i] = srcVertex[i + 1].vec[1];
			dst_exp->v1.z[i] = srcVertex[i + 1].vec[2];
			dst_exp->v1.w[i] = srcVertex[i + 1].vec[3];
			// TEXTURING_PART
			dst_exp->v1.s[i] = srcTex[i + 1].v0;
			dst_exp->v1.t[i] = srcTex[i + 1].v1;
			// TEXTURING_PART
			dst_exp->v1.color[i].vec[0] = srcColor[i + 1].vec[0];
			dst_exp->v1.color[i].vec[1] = srcColor[i + 1].vec[1];
			dst_exp->v1.color[i].vec[2] = srcColor[i + 1].vec[2];
			dst_exp->v1.color[i].vec[3] = srcColor[i + 1].vec[3];

			dst_exp->v2.x[i] = srcVertex[i + 2].vec[0];         
			dst_exp->v2.y[i] = srcVertex[i + 2].vec[1];         
			dst_exp->v2.z[i] = srcVertex[i + 2].vec[2];
			dst_exp->v2.w[i] = srcVertex[i + 2].vec[3];
			// TEXTURING_PART
			dst_exp->v2.s[i] = srcTex[i + 2].v0;
			dst_exp->v2.t[i] = srcTex[i + 2].v1;
			// TEXTURING_PART
			dst_exp->v2.color[i].vec[0] = srcColor[i + 2].vec[0];
			dst_exp->v2.color[i].vec[1] = srcColor[i + 2].vec[1];
			dst_exp->v2.color[i].vec[2] = srcColor[i + 2].vec[2];
			dst_exp->v2.color[i].vec[3] = srcColor[i + 2].vec[3];
		}
		if (trianglesCount % 2){
			dst_exp->v0.x[expectedTrianglesCount - 1] = dst_exp->v0.x[expectedTrianglesCount - 2];
			dst_exp->v0.y[expectedTrianglesCount - 1] = dst_exp->v0.y[expectedTrianglesCount - 2];
			dst_exp->v0.z[expectedTrianglesCount - 1] = dst_exp->v0.z[expectedTrianglesCount - 2];
			dst_exp->v0.w[expectedTrianglesCount - 1] = dst_exp->v0.w[expectedTrianglesCount - 2];
			// TEXTURING_PART
			dst_exp->v0.s[expectedTrianglesCount - 1] = dst_exp->v0.s[expectedTrianglesCount - 2];
			dst_exp->v0.t[expectedTrianglesCount - 1] = dst_exp->v0.t[expectedTrianglesCount - 2];
			// TEXTURING_PART
			dst_exp->v0.color[expectedTrianglesCount - 1].vec[0] = dst_exp->v0.color[expectedTrianglesCount - 2].vec[0];
			dst_exp->v0.color[expectedTrianglesCount - 1].vec[1] = dst_exp->v0.color[expectedTrianglesCount - 2].vec[1];
			dst_exp->v0.color[expectedTrianglesCount - 1].vec[2] = dst_exp->v0.color[expectedTrianglesCount - 2].vec[2];
			dst_exp->v0.color[expectedTrianglesCount - 1].vec[3] = dst_exp->v0.color[expectedTrianglesCount - 2].vec[3];

			dst_exp->v1.x[expectedTrianglesCount - 1] = dst_exp->v1.x[expectedTrianglesCount - 2];
			dst_exp->v1.y[expectedTrianglesCount - 1] = dst_exp->v1.y[expectedTrianglesCount - 2];
			dst_exp->v1.z[expectedTrianglesCount - 1] = dst_exp->v1.z[expectedTrianglesCount - 2];
			dst_exp->v1.w[expectedTrianglesCount - 1] = dst_exp->v1.w[expectedTrianglesCount - 2];
			// TEXTURING_PART
			dst_exp->v1.s[expectedTrianglesCount - 1] = dst_exp->v1.s[expectedTrianglesCount - 2];
			dst_exp->v1.t[expectedTrianglesCount - 1] = dst_exp->v1.t[expectedTrianglesCount - 2];
			// TEXTURING_PART
			dst_exp->v1.color[expectedTrianglesCount - 1].vec[0] = dst_exp->v1.color[expectedTrianglesCount - 2].vec[0];
			dst_exp->v1.color[expectedTrianglesCount - 1].vec[1] = dst_exp->v1.color[expectedTrianglesCount - 2].vec[1];
			dst_exp->v1.color[expectedTrianglesCount - 1].vec[2] = dst_exp->v1.color[expectedTrianglesCount - 2].vec[2];
			dst_exp->v1.color[expectedTrianglesCount - 1].vec[3] = dst_exp->v1.color[expectedTrianglesCount - 2].vec[3];

			dst_exp->v2.x[expectedTrianglesCount - 1] = dst_exp->v2.x[expectedTrianglesCount - 2];
			dst_exp->v2.y[expectedTrianglesCount - 1] = dst_exp->v2.y[expectedTrianglesCount - 2];
			dst_exp->v2.z[expectedTrianglesCount - 1] = dst_exp->v2.z[expectedTrianglesCount - 2];
			dst_exp->v2.w[expectedTrianglesCount - 1] = dst_exp->v2.w[expectedTrianglesCount - 2];
			// TEXTURING_PART
			dst_exp->v2.s[expectedTrianglesCount - 1] = dst_exp->v2.s[expectedTrianglesCount - 2];
			dst_exp->v2.t[expectedTrianglesCount - 1] = dst_exp->v2.t[expectedTrianglesCount - 2];
			// TEXTURING_PART
			dst_exp->v2.color[expectedTrianglesCount - 1].vec[0] = dst_exp->v2.color[expectedTrianglesCount - 2].vec[0];
			dst_exp->v2.color[expectedTrianglesCount - 1].vec[1] = dst_exp->v2.color[expectedTrianglesCount - 2].vec[1];
			dst_exp->v2.color[expectedTrianglesCount - 1].vec[2] = dst_exp->v2.color[expectedTrianglesCount - 2].vec[2];
			dst_exp->v2.color[expectedTrianglesCount - 1].vec[3] = dst_exp->v2.color[expectedTrianglesCount - 2].vec[3];
		} else {
			// Do not correct 
		}
	}
}

void setDst(TrianglePointers *dst, nm32f *buf, int outputTrianglesCount)
{
	dst->v0.x = buf;
	dst->v0.y = dst->v0.x + outputTrianglesCount;
	dst->v0.z = dst->v0.y + outputTrianglesCount;
	dst->v0.w = dst->v0.z + outputTrianglesCount;
// TEXTURING_PART
	dst->v0.s = dst->v0.w + outputTrianglesCount;
	dst->v0.t = dst->v0.s + outputTrianglesCount;
    dst->v0.color = (v4nm32f *)(dst->v0.t + outputTrianglesCount);

	dst->v1.x = buf + ATTR_PER_VERTEX * outputTrianglesCount;
	dst->v1.y = dst->v1.x + outputTrianglesCount;
	dst->v1.z = dst->v1.y + outputTrianglesCount;
	dst->v1.w = dst->v1.z + outputTrianglesCount;
// TEXTURING_PART
	dst->v1.s = dst->v1.w + outputTrianglesCount;
	dst->v1.t = dst->v1.s + outputTrianglesCount;
    dst->v1.color = (v4nm32f *)(dst->v1.t + outputTrianglesCount);
	
	dst->v2.x = buf + ATTR_PER_VERTEX * 2 * outputTrianglesCount;
	dst->v2.y = dst->v2.x + outputTrianglesCount;
	dst->v2.z = dst->v2.y + outputTrianglesCount;
	dst->v2.w = dst->v2.z + outputTrianglesCount;
// TEXTURING_PART
	dst->v2.s = dst->v2.w + outputTrianglesCount;
	dst->v2.t = dst->v2.s + outputTrianglesCount;
    dst->v2.color = (v4nm32f *)(dst->v2.t + outputTrianglesCount);
}

int repackToPrimitives_tf_nOutputTriangles_AllDataAreCorrect(int n)
{
	int trianglesCount = n;							// number of output triangles
	// ZERO_COUNT - количество выходных параметров, которые должны остаться неизменными после работы функции
	// Это число должно быть чётным, от него зависит адрес, с которого начинаются
	// некоторые массивы выходных координат. Функция пишет каждую координату 
	// по 64 бита (векторный процессор), так что выходной адрес должен быть чётным.
	// ZERO_COUNT - number of triangles at the end that should not be touched
	int ZERO_COUNT = 4;							
    int vertCount = trianglesCount + 2;				// number of input vertices
	int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
	int outputTrianglesCount = expectedTrianglesCount + ZERO_COUNT;
	// 10 floats - x, y, z, w, s, t, r, g, b, a
	//  8 floats - x, y, z, w,       r, g, b, a
	int outputCoordCount = ATTR_PER_VERTEX * 3 * outputTrianglesCount;	// number of output coords

#ifdef __NM__
    v4nm32f     srcVertex[vertCount];
    v4nm32f     srcColor[vertCount];
    v2nm32f     srcTex[vertCount];
	nm32f real_output[outputCoordCount];
	nm32f expected_output[outputCoordCount];
#else
    v4nm32f     *srcVertex 			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v4nm32f     *srcColor			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v2nm32f     *srcTex				= (v2nm32f *) malloc(sizeof(v2nm32f) * vertCount);
	nm32f 		*real_output		= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
	nm32f 		*expected_output	= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
#endif	// __NM__
	
	// Initialize input arrays: coords, colors and textures (if texturing is enabled)
	initializeInput(srcVertex, srcColor, srcTex, vertCount);
	
	// Initialize these arrays explicitly because variable-length arrays
	// can not be initialized
	for (int i = 0; i < outputCoordCount; ++i){
		real_output[i] = 0;
		expected_output[i] = 0;
	}
	
	//	Set real output pointers, i.e. dst
	TrianglePointers dst;
	setDst(&dst, real_output, outputTrianglesCount);

	// Set expected output pointers
	TrianglePointers dst_exp;
	setDst(&dst_exp, expected_output, outputTrianglesCount);

	initializeExpectedOutput(&dst_exp, 
			srcVertex, 
			srcColor, 
			srcTex, 
			trianglesCount, 
			expectedTrianglesCount,
			outputTrianglesCount); 

	// Act
    int res;
	res = repackToPrimitives_tf(srcVertex, srcColor, srcTex, &dst, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL(real_output, expected_output, outputCoordCount);
    return 0;
}

int repackToPrimitives_tf_nOutputTriangles_v0CoordsAreCorrect(int n)
{
	int trianglesCount = n;							// number of output triangles
	// ZERO_COUNT - количество выходных параметров, которые должны остаться неизменными после работы функции
	// Это число должно быть чётным, от него зависит адрес, с которого начинаются
	// некоторые массивы выходных координат. Функция пишет каждую координату 
	// по 64 бита (векторный процессор), так что выходной адрес должен быть чётным.
	// ZERO_COUNT - number of triangles at the end that should not be touched
	int ZERO_COUNT = 4;							
    int vertCount = trianglesCount + 2;				// number of input vertices
	int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
	int outputTrianglesCount = expectedTrianglesCount + ZERO_COUNT;
	// 10 floats - x, y, z, w, s, t, r, g, b, a
	//  8 floats - x, y, z, w,       r, g, b, a
	int outputCoordCount = ATTR_PER_VERTEX * 3 * outputTrianglesCount;	// number of output coords

#ifdef __NM__
    v4nm32f     srcVertex[vertCount];
    v4nm32f     srcColor[vertCount];
    v2nm32f     srcTex[vertCount];
	nm32f real_output[outputCoordCount];
	nm32f expected_output[outputCoordCount];
#else
    v4nm32f     *srcVertex 			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v4nm32f     *srcColor			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v2nm32f     *srcTex				= (v2nm32f *) malloc(sizeof(v2nm32f) * vertCount);
	nm32f 		*real_output		= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
	nm32f 		*expected_output	= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
#endif	// __NM__
	
	// Initialize input arrays: coords, colors and textures (if texturing is enabled)
	initializeInput(srcVertex, srcColor, srcTex, vertCount);
	
	// Initialize these arrays explicitly because variable-length arrays
	// can not be initialized
	for (int i = 0; i < outputCoordCount; ++i){
		real_output[i] = 0;
		expected_output[i] = 0;
	}
	
	//	Set real output pointers, i.e. dst
	TrianglePointers dst;
	setDst(&dst, real_output, outputTrianglesCount);

	// Set expected output pointers
	TrianglePointers dst_exp;
	setDst(&dst_exp, expected_output, outputTrianglesCount);

	initializeExpectedOutput(&dst_exp, 
			srcVertex, 
			srcColor, 
			srcTex, 
			trianglesCount, 
			expectedTrianglesCount,
			outputTrianglesCount); 

	// Act
    int res;
	res = repackToPrimitives_tf(srcVertex, srcColor, srcTex, &dst, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL(dst.v0.x, dst_exp.v0.x, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v0.y, dst_exp.v0.y, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v0.z, dst_exp.v0.z, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v0.w, dst_exp.v0.w, outputTrianglesCount);
    return 0;
}
int repackToPrimitives_tf_nOutputTriangles_v0ColorsAreCorrect(int n)
{
	int trianglesCount = n;							// number of output triangles
	// ZERO_COUNT - количество выходных параметров, которые должны остаться неизменными после работы функции
	// Это число должно быть чётным, от него зависит адрес, с которого начинаются
	// некоторые массивы выходных координат. Функция пишет каждую координату 
	// по 64 бита (векторный процессор), так что выходной адрес должен быть чётным.
	// ZERO_COUNT - number of triangles at the end that should not be touched
	int ZERO_COUNT = 4;							
    int vertCount = trianglesCount + 2;				// number of input vertices
	int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
	int outputTrianglesCount = expectedTrianglesCount + ZERO_COUNT;
	// 10 floats - x, y, z, w, s, t, r, g, b, a
	//  8 floats - x, y, z, w,       r, g, b, a
	int outputCoordCount = ATTR_PER_VERTEX * 3 * outputTrianglesCount;	// number of output coords

#ifdef __NM__
    v4nm32f     srcVertex[vertCount];
    v4nm32f     srcColor[vertCount];
    v2nm32f     srcTex[vertCount];
	nm32f real_output[outputCoordCount];
	nm32f expected_output[outputCoordCount];
#else
    v4nm32f     *srcVertex 			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v4nm32f     *srcColor			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v2nm32f     *srcTex				= (v2nm32f *) malloc(sizeof(v2nm32f) * vertCount);
	nm32f 		*real_output		= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
	nm32f 		*expected_output	= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
#endif	// __NM__
	
	// Initialize input arrays: coords, colors and textures (if texturing is enabled)
	initializeInput(srcVertex, srcColor, srcTex, vertCount);
	
	// Initialize these arrays explicitly because variable-length arrays
	// can not be initialized
	for (int i = 0; i < outputCoordCount; ++i){
		real_output[i] = 0;
		expected_output[i] = 0;
	}
	
	//	Set real output pointers, i.e. dst
	TrianglePointers dst;
	setDst(&dst, real_output, outputTrianglesCount);

	// Set expected output pointers
	TrianglePointers dst_exp;
	setDst(&dst_exp, expected_output, outputTrianglesCount);

	initializeExpectedOutput(&dst_exp, 
			srcVertex, 
			srcColor, 
			srcTex, 
			trianglesCount, 
			expectedTrianglesCount,
			outputTrianglesCount); 

	// Act
    int res;
	res = repackToPrimitives_tf(srcVertex, srcColor, srcTex, &dst, vertCount);

	// Assert
    TEST_VEC_ARRAYS_EQUAL(dst.v0.color, dst_exp.v0.color, outputTrianglesCount);
    return 0;
}
int repackToPrimitives_tf_nOutputTriangles_v1CoordsAreCorrect(int n)
{
	int trianglesCount = n;							// number of output triangles
	// ZERO_COUNT - количество выходных параметров, которые должны остаться неизменными после работы функции
	// Это число должно быть чётным, от него зависит адрес, с которого начинаются
	// некоторые массивы выходных координат. Функция пишет каждую координату 
	// по 64 бита (векторный процессор), так что выходной адрес должен быть чётным.
	// ZERO_COUNT - number of triangles at the end that should not be touched
	int ZERO_COUNT = 4;							
    int vertCount = trianglesCount + 2;				// number of input vertices
	int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
	int outputTrianglesCount = expectedTrianglesCount + ZERO_COUNT;
	// 10 floats - x, y, z, w, s, t, r, g, b, a
	//  8 floats - x, y, z, w,       r, g, b, a
	int outputCoordCount = ATTR_PER_VERTEX * 3 * outputTrianglesCount;	// number of output coords

#ifdef __NM__
    v4nm32f     srcVertex[vertCount];
    v4nm32f     srcColor[vertCount];
    v2nm32f     srcTex[vertCount];
	nm32f real_output[outputCoordCount];
	nm32f expected_output[outputCoordCount];
#else
    v4nm32f     *srcVertex 			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v4nm32f     *srcColor			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v2nm32f     *srcTex				= (v2nm32f *) malloc(sizeof(v2nm32f) * vertCount);
	nm32f 		*real_output		= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
	nm32f 		*expected_output	= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
#endif	// __NM__
	
	// Initialize input arrays: coords, colors and textures (if texturing is enabled)
	initializeInput(srcVertex, srcColor, srcTex, vertCount);
	
	// Initialize these arrays explicitly because variable-length arrays
	// can not be initialized
	for (int i = 0; i < outputCoordCount; ++i){
		real_output[i] = 0;
		expected_output[i] = 0;
	}
	
	//	Set real output pointers, i.e. dst
	TrianglePointers dst;
	setDst(&dst, real_output, outputTrianglesCount);

	// Set expected output pointers
	TrianglePointers dst_exp;
	setDst(&dst_exp, expected_output, outputTrianglesCount);

	initializeExpectedOutput(&dst_exp, 
			srcVertex, 
			srcColor, 
			srcTex, 
			trianglesCount, 
			expectedTrianglesCount,
			outputTrianglesCount); 

	// Act
    int res;
	res = repackToPrimitives_tf(srcVertex, srcColor, srcTex, &dst, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL(dst.v1.x, dst_exp.v1.x, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v1.y, dst_exp.v1.y, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v1.z, dst_exp.v1.z, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v1.w, dst_exp.v1.w, outputTrianglesCount);
    return 0;
}
int repackToPrimitives_tf_nOutputTriangles_v1ColorsAreCorrect(int n)
{
	int trianglesCount = n;							// number of output triangles
	// ZERO_COUNT - количество выходных параметров, которые должны остаться неизменными после работы функции
	// Это число должно быть чётным, от него зависит адрес, с которого начинаются
	// некоторые массивы выходных координат. Функция пишет каждую координату 
	// по 64 бита (векторный процессор), так что выходной адрес должен быть чётным.
	// ZERO_COUNT - number of triangles at the end that should not be touched
	int ZERO_COUNT = 4;							
    int vertCount = trianglesCount + 2;				// number of input vertices
	int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
	int outputTrianglesCount = expectedTrianglesCount + ZERO_COUNT;
	// 10 floats - x, y, z, w, s, t, r, g, b, a
	//  8 floats - x, y, z, w,       r, g, b, a
	int outputCoordCount = ATTR_PER_VERTEX * 3 * outputTrianglesCount;	// number of output coords

#ifdef __NM__
    v4nm32f     srcVertex[vertCount];
    v4nm32f     srcColor[vertCount];
    v2nm32f     srcTex[vertCount];
	nm32f real_output[outputCoordCount];
	nm32f expected_output[outputCoordCount];
#else
    v4nm32f     *srcVertex 			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v4nm32f     *srcColor			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v2nm32f     *srcTex				= (v2nm32f *) malloc(sizeof(v2nm32f) * vertCount);
	nm32f 		*real_output		= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
	nm32f 		*expected_output	= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
#endif	// __NM__
	
	// Initialize input arrays: coords, colors and textures (if texturing is enabled)
	initializeInput(srcVertex, srcColor, srcTex, vertCount);
	
	// Initialize these arrays explicitly because variable-length arrays
	// can not be initialized
	for (int i = 0; i < outputCoordCount; ++i){
		real_output[i] = 0;
		expected_output[i] = 0;
	}
	
	//	Set real output pointers, i.e. dst
	TrianglePointers dst;
	setDst(&dst, real_output, outputTrianglesCount);

	// Set expected output pointers
	TrianglePointers dst_exp;
	setDst(&dst_exp, expected_output, outputTrianglesCount);

	initializeExpectedOutput(&dst_exp, 
			srcVertex, 
			srcColor, 
			srcTex, 
			trianglesCount, 
			expectedTrianglesCount,
			outputTrianglesCount); 

	// Act
    int res;
	res = repackToPrimitives_tf(srcVertex, srcColor, srcTex, &dst, vertCount);

	// Assert
    TEST_VEC_ARRAYS_EQUAL(dst.v1.color, dst_exp.v1.color, outputTrianglesCount);
    return 0;
}

int repackToPrimitives_tf_nOutputTriangles_v2CoordsAreCorrect(int n)
{
	int trianglesCount = n;							// number of output triangles
	// ZERO_COUNT - количество выходных параметров, которые должны остаться неизменными после работы функции
	// Это число должно быть чётным, от него зависит адрес, с которого начинаются
	// некоторые массивы выходных координат. Функция пишет каждую координату 
	// по 64 бита (векторный процессор), так что выходной адрес должен быть чётным.
	// ZERO_COUNT - number of triangles at the end that should not be touched
	int ZERO_COUNT = 4;							
    int vertCount = trianglesCount + 2;				// number of input vertices
	int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
	int outputTrianglesCount = expectedTrianglesCount + ZERO_COUNT;
	// 10 floats - x, y, z, w, s, t, r, g, b, a
	//  8 floats - x, y, z, w,       r, g, b, a
	int outputCoordCount = ATTR_PER_VERTEX * 3 * outputTrianglesCount;	// number of output coords

#ifdef __NM__
    v4nm32f     srcVertex[vertCount];
    v4nm32f     srcColor[vertCount];
    v2nm32f     srcTex[vertCount];
	nm32f real_output[outputCoordCount];
	nm32f expected_output[outputCoordCount];
#else
    v4nm32f     *srcVertex 			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v4nm32f     *srcColor			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v2nm32f     *srcTex				= (v2nm32f *) malloc(sizeof(v2nm32f) * vertCount);
	nm32f 		*real_output		= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
	nm32f 		*expected_output	= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
#endif	// __NM__
	
	// Initialize input arrays: coords, colors and textures (if texturing is enabled)
	initializeInput(srcVertex, srcColor, srcTex, vertCount);
	
	// Initialize these arrays explicitly because variable-length arrays
	// can not be initialized
	for (int i = 0; i < outputCoordCount; ++i){
		real_output[i] = 0;
		expected_output[i] = 0;
	}
	
	//	Set real output pointers, i.e. dst
	TrianglePointers dst;
	setDst(&dst, real_output, outputTrianglesCount);

	// Set expected output pointers
	TrianglePointers dst_exp;
	setDst(&dst_exp, expected_output, outputTrianglesCount);

	initializeExpectedOutput(&dst_exp, 
			srcVertex, 
			srcColor, 
			srcTex, 
			trianglesCount, 
			expectedTrianglesCount,
			outputTrianglesCount); 

	// Act
    int res;
	res = repackToPrimitives_tf(srcVertex, srcColor, srcTex, &dst, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL(dst.v2.x, dst_exp.v2.x, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v2.y, dst_exp.v2.y, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v2.z, dst_exp.v2.z, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v2.w, dst_exp.v2.w, outputTrianglesCount);
    return 0;
}

int repackToPrimitives_tf_nOutputTriangles_v2ColorsAreCorrect(int n)
{
	int trianglesCount = n;							// number of output triangles
	// ZERO_COUNT - количество выходных параметров, которые должны остаться неизменными после работы функции
	// Это число должно быть чётным, от него зависит адрес, с которого начинаются
	// некоторые массивы выходных координат. Функция пишет каждую координату 
	// по 64 бита (векторный процессор), так что выходной адрес должен быть чётным.
	// ZERO_COUNT - number of triangles at the end that should not be touched
	int ZERO_COUNT = 4;							
    int vertCount = trianglesCount + 2;				// number of input vertices
	int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
	int outputTrianglesCount = expectedTrianglesCount + ZERO_COUNT;
	// 10 floats - x, y, z, w, s, t, r, g, b, a
	//  8 floats - x, y, z, w,       r, g, b, a
	int outputCoordCount = ATTR_PER_VERTEX * 3 * outputTrianglesCount;	// number of output coords

#ifdef __NM__
    v4nm32f     srcVertex[vertCount];
    v4nm32f     srcColor[vertCount];
    v2nm32f     srcTex[vertCount];
	nm32f real_output[outputCoordCount];
	nm32f expected_output[outputCoordCount];
#else
    v4nm32f     *srcVertex 			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v4nm32f     *srcColor			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v2nm32f     *srcTex				= (v2nm32f *) malloc(sizeof(v2nm32f) * vertCount);
	nm32f 		*real_output		= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
	nm32f 		*expected_output	= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
#endif	// __NM__
	
	// Initialize input arrays: coords, colors and textures (if texturing is enabled)
	initializeInput(srcVertex, srcColor, srcTex, vertCount);
	
	// Initialize these arrays explicitly because variable-length arrays
	// can not be initialized
	for (int i = 0; i < outputCoordCount; ++i){
		real_output[i] = 0;
		expected_output[i] = 0;
	}
	
	//	Set real output pointers, i.e. dst
	TrianglePointers dst;
	setDst(&dst, real_output, outputTrianglesCount);

	// Set expected output pointers
	TrianglePointers dst_exp;
	setDst(&dst_exp, expected_output, outputTrianglesCount);

	initializeExpectedOutput(&dst_exp, 
			srcVertex, 
			srcColor, 
			srcTex, 
			trianglesCount, 
			expectedTrianglesCount,
			outputTrianglesCount); 

	// Act
    int res;
	res = repackToPrimitives_tf(srcVertex, srcColor, srcTex, &dst, vertCount);

	// Assert
    TEST_VEC_ARRAYS_EQUAL(dst.v2.color, dst_exp.v2.color, outputTrianglesCount);
    return 0;
}

int repackToPrimitives_tf_nOutputTriangles_v0TextureCoordsAreCorrect(int n)
{
	int trianglesCount = n;							// number of output triangles
	// ZERO_COUNT - количество выходных параметров, которые должны остаться неизменными после работы функции
	// Это число должно быть чётным, от него зависит адрес, с которого начинаются
	// некоторые массивы выходных координат. Функция пишет каждую координату 
	// по 64 бита (векторный процессор), так что выходной адрес должен быть чётным.
	// ZERO_COUNT - number of triangles at the end that should not be touched
	int ZERO_COUNT = 4;							
    int vertCount = trianglesCount + 2;				// number of input vertices
	int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
	int outputTrianglesCount = expectedTrianglesCount + ZERO_COUNT;
	// 10 floats - x, y, z, w, s, t, r, g, b, a
	//  8 floats - x, y, z, w,       r, g, b, a
	int outputCoordCount = ATTR_PER_VERTEX * 3 * outputTrianglesCount;	// number of output coords

#ifdef __NM__
    v4nm32f     srcVertex[vertCount];
    v4nm32f     srcColor[vertCount];
    v2nm32f     srcTex[vertCount];
	nm32f real_output[outputCoordCount];
	nm32f expected_output[outputCoordCount];
#else
    v4nm32f     *srcVertex 			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v4nm32f     *srcColor			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v2nm32f     *srcTex				= (v2nm32f *) malloc(sizeof(v2nm32f) * vertCount);
	nm32f 		*real_output		= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
	nm32f 		*expected_output	= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
#endif	// __NM__
	
	// Initialize input arrays: coords, colors and textures (if texturing is enabled)
	initializeInput(srcVertex, srcColor, srcTex, vertCount);
	
	// Initialize these arrays explicitly because variable-length arrays
	// can not be initialized
	for (int i = 0; i < outputCoordCount; ++i){
		real_output[i] = 0;
		expected_output[i] = 0;
	}
	
	//	Set real output pointers, i.e. dst
	TrianglePointers dst;
	setDst(&dst, real_output, outputTrianglesCount);

	// Set expected output pointers
	TrianglePointers dst_exp;
	setDst(&dst_exp, expected_output, outputTrianglesCount);

	initializeExpectedOutput(&dst_exp, 
			srcVertex, 
			srcColor, 
			srcTex, 
			trianglesCount, 
			expectedTrianglesCount,
			outputTrianglesCount); 

	// Act
    int res;
	res = repackToPrimitives_tf(srcVertex, srcColor, srcTex, &dst, vertCount);

	// Assert
	// TEXTURING_PART
    TEST_ARRAYS_EQUAL(dst.v0.s, dst_exp.v0.s, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v0.t, dst_exp.v0.t, outputTrianglesCount);
	// TEXTURING_PART
    return 0;
}

int repackToPrimitives_tf_nOutputTriangles_v1TextureCoordsAreCorrect(int n)
{
	int trianglesCount = n;							// number of output triangles
	// ZERO_COUNT - количество выходных параметров, которые должны остаться неизменными после работы функции
	// Это число должно быть чётным, от него зависит адрес, с которого начинаются
	// некоторые массивы выходных координат. Функция пишет каждую координату 
	// по 64 бита (векторный процессор), так что выходной адрес должен быть чётным.
	// ZERO_COUNT - number of triangles at the end that should not be touched
	int ZERO_COUNT = 4;							
    int vertCount = trianglesCount + 2;				// number of input vertices
	int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
	int outputTrianglesCount = expectedTrianglesCount + ZERO_COUNT;
	// 10 floats - x, y, z, w, s, t, r, g, b, a
	//  8 floats - x, y, z, w,       r, g, b, a
	int outputCoordCount = ATTR_PER_VERTEX * 3 * outputTrianglesCount;	// number of output coords

#ifdef __NM__
    v4nm32f     srcVertex[vertCount];
    v4nm32f     srcColor[vertCount];
    v2nm32f     srcTex[vertCount];
	nm32f real_output[outputCoordCount];
	nm32f expected_output[outputCoordCount];
#else
    v4nm32f     *srcVertex 			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v4nm32f     *srcColor			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v2nm32f     *srcTex				= (v2nm32f *) malloc(sizeof(v2nm32f) * vertCount);
	nm32f 		*real_output		= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
	nm32f 		*expected_output	= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
#endif	// __NM__
	
	// Initialize input arrays: coords, colors and textures (if texturing is enabled)
	initializeInput(srcVertex, srcColor, srcTex, vertCount);
	
	// Initialize these arrays explicitly because variable-length arrays
	// can not be initialized
	for (int i = 0; i < outputCoordCount; ++i){
		real_output[i] = 0;
		expected_output[i] = 0;
	}
	
	//	Set real output pointers, i.e. dst
	TrianglePointers dst;
	setDst(&dst, real_output, outputTrianglesCount);

	// Set expected output pointers
	TrianglePointers dst_exp;
	setDst(&dst_exp, expected_output, outputTrianglesCount);

	initializeExpectedOutput(&dst_exp, 
			srcVertex, 
			srcColor, 
			srcTex, 
			trianglesCount, 
			expectedTrianglesCount,
			outputTrianglesCount); 

	// Act
    int res;
	res = repackToPrimitives_tf(srcVertex, srcColor, srcTex, &dst, vertCount);

	// Assert
	// TEXTURING_PART
    TEST_ARRAYS_EQUAL(dst.v1.s, dst_exp.v1.s, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v1.t, dst_exp.v1.t, outputTrianglesCount);
	// TEXTURING_PART
    return 0;
}

int repackToPrimitives_tf_nOutputTriangles_v2TextureCoordsAreCorrect(int n)
{
	int trianglesCount = n;							// number of output triangles
	// ZERO_COUNT - количество выходных параметров, которые должны остаться неизменными после работы функции
	// Это число должно быть чётным, от него зависит адрес, с которого начинаются
	// некоторые массивы выходных координат. Функция пишет каждую координату 
	// по 64 бита (векторный процессор), так что выходной адрес должен быть чётным.
	// ZERO_COUNT - number of triangles at the end that should not be touched
	int ZERO_COUNT = 4;							
    int vertCount = trianglesCount + 2;				// number of input vertices
	int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
	int outputTrianglesCount = expectedTrianglesCount + ZERO_COUNT;
	// 10 floats - x, y, z, w, s, t, r, g, b, a
	//  8 floats - x, y, z, w,       r, g, b, a
	int outputCoordCount = ATTR_PER_VERTEX * 3 * outputTrianglesCount;	// number of output coords

#ifdef __NM__
    v4nm32f     srcVertex[vertCount];
    v4nm32f     srcColor[vertCount];
    v2nm32f     srcTex[vertCount];
	nm32f real_output[outputCoordCount];
	nm32f expected_output[outputCoordCount];
#else
    v4nm32f     *srcVertex 			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v4nm32f     *srcColor			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v2nm32f     *srcTex				= (v2nm32f *) malloc(sizeof(v2nm32f) * vertCount);
	nm32f 		*real_output		= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
	nm32f 		*expected_output	= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
#endif	// __NM__
	
	// Initialize input arrays: coords, colors and textures (if texturing is enabled)
	initializeInput(srcVertex, srcColor, srcTex, vertCount);
	
	// Initialize these arrays explicitly because variable-length arrays
	// can not be initialized
	for (int i = 0; i < outputCoordCount; ++i){
		real_output[i] = 0;
		expected_output[i] = 0;
	}
	
	//	Set real output pointers, i.e. dst
	TrianglePointers dst;
	setDst(&dst, real_output, outputTrianglesCount);

	// Set expected output pointers
	TrianglePointers dst_exp;
	setDst(&dst_exp, expected_output, outputTrianglesCount);

	initializeExpectedOutput(&dst_exp, 
			srcVertex, 
			srcColor, 
			srcTex, 
			trianglesCount, 
			expectedTrianglesCount,
			outputTrianglesCount); 

	// Act
    int res;
	res = repackToPrimitives_tf(srcVertex, srcColor, srcTex, &dst, vertCount);

	// Assert
	// TEXTURING_PART
    TEST_ARRAYS_EQUAL(dst.v2.s, dst_exp.v2.s, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v2.t, dst_exp.v2.t, outputTrianglesCount);
	// TEXTURING_PART
    return 0;
}

int repackToPrimitives_tf_0_200_OutputTriangles_AllDataAreCorrect()
{
	int res = 0;
	for (int i = 0; i < 200; ++i){
    	res += repackToPrimitives_tf_nOutputTriangles_AllDataAreCorrect(i);
	}
	return res;
}

int repackToPrimitives_tf_0_200_OutputTriangles_v0CoordsAreCorrect()
{
	int res = 0;
	for (int i = 0; i < 200; ++i){
    	res += repackToPrimitives_tf_nOutputTriangles_v0CoordsAreCorrect(i);
	}
	return res;
}

int repackToPrimitives_tf_0_200_OutputTriangles_v1CoordsAreCorrect()
{
	int res = 0;
	for (int i = 0; i < 200; ++i){
    	res += repackToPrimitives_tf_nOutputTriangles_v1CoordsAreCorrect(i);
	}
	return res;
}

int repackToPrimitives_tf_0_200_OutputTriangles_v2CoordsAreCorrect()
{
	int res = 0;
	for (int i = 0; i < 200; ++i){
    	res += repackToPrimitives_tf_nOutputTriangles_v2CoordsAreCorrect(i);
	}
	return res;
}

int repackToPrimitives_tf_0_200_OutputTriangles_v0ColorsAreCorrect()
{
	int res = 0;
	for (int i = 0; i < 200; ++i){
    	res += repackToPrimitives_tf_nOutputTriangles_v0ColorsAreCorrect(i);
	}
	return res;
}

int repackToPrimitives_tf_0_200_OutputTriangles_v1ColorsAreCorrect()
{
	int res = 0;
	for (int i = 0; i < 200; ++i){
    	res += repackToPrimitives_tf_nOutputTriangles_v1ColorsAreCorrect(i);
	}
	return res;
}

int repackToPrimitives_tf_0_200_OutputTriangles_v2ColorsAreCorrect()
{
	int res = 0;
	for (int i = 0; i < 200; ++i){
    	res += repackToPrimitives_tf_nOutputTriangles_v2ColorsAreCorrect(i);
	}
	return res;
}

int repackToPrimitives_tf_0_200_OutputTriangles_v0TextureCoordsAreCorrect()
{
	int res = 0;
	for (int i = 0; i < 200; ++i){
    	res += repackToPrimitives_tf_nOutputTriangles_v0TextureCoordsAreCorrect(i);
	}
	return res;
}

int repackToPrimitives_tf_0_200_OutputTriangles_v1TextureCoordsAreCorrect()
{
	int res = 0;
	for (int i = 0; i < 200; ++i){
    	res += repackToPrimitives_tf_nOutputTriangles_v1TextureCoordsAreCorrect(i);
	}
	return res;
}

int repackToPrimitives_tf_0_200_OutputTriangles_v2TextureCoordsAreCorrect()
{
	int res = 0;
	for (int i = 0; i < 200; ++i){
    	res += repackToPrimitives_tf_nOutputTriangles_v2TextureCoordsAreCorrect(i);
	}
	return res;
}

clock_t repackToPrimitives_tf_192Vertices()
{
	// Arrange
	int trianglesCount = 64;							// number of output triangles
    int vertCount = trianglesCount + 2;				// number of input vertices
	int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
	int outputTrianglesCount = expectedTrianglesCount;
	// 10 floats - x, y, z, w, s, t, r, g, b, a
	//  8 floats - x, y, z, w,       r, g, b, a
	int outputCoordCount = ATTR_PER_VERTEX * 3 * outputTrianglesCount;	// number of output coords

#ifdef __NM__
    v4nm32f     srcVertex[vertCount];
    v4nm32f     srcColor[vertCount];
    v2nm32f     srcTex[vertCount];
	nm32f real_output[outputCoordCount];
#else
    v4nm32f     *srcVertex 			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v4nm32f     *srcColor			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v2nm32f     *srcTex				= (v2nm32f *) malloc(sizeof(v2nm32f) * vertCount);
	nm32f 		*real_output		= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
#endif	// __NM__
	
	//	Set real output pointers, i.e. dst
	TrianglePointers dst;
	setDst(&dst, real_output, outputTrianglesCount);

	clock_t t1, t2, dt;
	
	// Act
	int res;
	t1 = clock();
	res = repackToPrimitives_tf(srcVertex, srcColor, srcTex, &dst, vertCount);
	t2 = clock();
	dt = t2 - t1;
	(void) res;

	return dt;
}

clock_t repackToPrimitives_tf_48Vertices()
{
	// Arrange
	int trianglesCount = 16;							// number of output triangles
    int vertCount = trianglesCount + 2;				// number of input vertices
	int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
	int outputTrianglesCount = expectedTrianglesCount;
	// 10 floats - x, y, z, w, s, t, r, g, b, a
	//  8 floats - x, y, z, w,       r, g, b, a
	int outputCoordCount = ATTR_PER_VERTEX * 3 * outputTrianglesCount;	// number of output coords

#ifdef __NM__
    v4nm32f     srcVertex[vertCount];
    v4nm32f     srcColor[vertCount];
    v2nm32f     srcTex[vertCount];
	nm32f real_output[outputCoordCount];
#else
    v4nm32f     *srcVertex 			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v4nm32f     *srcColor			= (v4nm32f *) malloc(sizeof(v4nm32f) * vertCount);
    v2nm32f     *srcTex				= (v2nm32f *) malloc(sizeof(v2nm32f) * vertCount);
	nm32f 		*real_output		= (nm32f *) malloc(sizeof(nm32f) * outputCoordCount);
#endif	// __NM__
	
	//	Set real output pointers, i.e. dst
	TrianglePointers dst;
	setDst(&dst, real_output, outputTrianglesCount);

	clock_t t1, t2, dt;
	
	// Act
	int res;
	t1 = clock();
	res = repackToPrimitives_tf(srcVertex, srcColor, srcTex, &dst, vertCount);
	t2 = clock();
	dt = t2 - t1;
	(void) res;

	return dt;
}
