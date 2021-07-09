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

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstVertexLengthIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstVertexIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstColorLengthIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstColorIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES81Triangles_dstVertexLengthIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES81Triangles_dstVertexIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES81_dstColorLengthIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES81_dstColorIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES90Triangles_dstVertexLengthIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES90Triangles_dstVertexIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES90_dstColorLengthIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES90_dstColorIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES15Vertexes_returns6();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES45Vertexes_returns16();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES90Vertexes_returns30();

int repackToPrimitives_t_0_200_OutputTriangles_AllDataAreCorrect();
int repackToPrimitives_t_nOutputTriangles(int n);

// Performance tests
clock_t vertexPrimitiveRepack_modeIsGL_TRIANGLES_192Vertices();
clock_t vertexPrimitiveRepack_modeIsGL_TRIANGLES_48Vertices();

// TEMP test
//int repackToPrimitives_t(const v4nm32f *srcVertex,
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
    //RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstVertexLengthIsCorrect);
    //RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstVertexIsCorrect);
    //RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstColorLengthIsCorrect);
    //RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstColorIsCorrect);
    //RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES81Triangles_dstVertexLengthIsCorrect);
	//RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES81Triangles_dstVertexIsCorrect);
    //RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES81_dstColorLengthIsCorrect);
	//RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES81_dstColorIsCorrect);
    //RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES90Triangles_dstVertexLengthIsCorrect);
	//RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES90Triangles_dstVertexIsCorrect);
    //RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES90_dstColorLengthIsCorrect);
	//RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES90_dstColorIsCorrect);
	//RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES15Vertexes_returns6);
	//RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES45Vertexes_returns16);
	//RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES90Vertexes_returns30);	

	//puts("");
    //puts("Performance tests: ");
	//clock_t dt[2];
	//dt[0] = vertexPrimitiveRepack_modeIsGL_TRIANGLES_192Vertices();
	//dt[1] = vertexPrimitiveRepack_modeIsGL_TRIANGLES_48Vertices();
	//for (int i = 0; i < 2; ++i){
	//	printf("dt[%i] = %i\n\r", i + 1, (int)dt[i]);
	//}

    //puts("OK");
	RUN_TEST(repackToPrimitives_t_0_200_OutputTriangles_AllDataAreCorrect);
	//RUN_ARG_TEST(repackToPrimitives_t_nOutputTriangles, 65);	// >= 65 doesn't work, may be because of memory
	return 0;
}

#ifdef TEXTURE_ENABLED
#define ATTR_PER_VERTEX 10 
#else
#define ATTR_PER_VERTEX 8
#endif

void initializeInput(v4nm32f *srcVertex, v4nm32f *srcColor, v2nm32f *srcTex, int vertCount)
{
    for (int i = 0; i < vertCount; i++){
        for (int j = 0; j < 4; j++){
            srcVertex[i].vec[j] = (float)(4 * i + j);
            srcColor[i].vec[j] = (float)(4 * i + j);
		}
	}
#ifdef TEXTURE_ENABLED
    for (int i = 0; i < vertCount; i++){
		srcTex[i].v0 = (float)(2 * i);
		srcTex[i].v1 = (float)(2 * i + 1);
	}
#endif
}

void initializeExpectedOutput(TrianglePointers *dst_exp, 
		v4nm32f *srcVertex, 
		v4nm32f *srcColor, 
		v2nm32f *srcTex, 
		int trianglesCount, 
		int expectedTrianglesCount,
		int outputTrianglesCount)
{

    for (int i = 0; i < trianglesCount; i++){
		dst_exp->v0.x[i] = srcVertex[3 * i].vec[0];
		dst_exp->v0.y[i] = srcVertex[3 * i].vec[1];
		dst_exp->v0.z[i] = srcVertex[3 * i].vec[2];
		dst_exp->v0.w[i] = srcVertex[3 * i].vec[3];
#ifdef TEXTURE_ENABLED
		dst_exp->v0.s[i] = srcTex[3 * i].v0;
		dst_exp->v0.t[i] = srcTex[3 * i].v1;
#endif
		dst_exp->v0.color[i].vec[0] = srcColor[3 * i].vec[0];
		dst_exp->v0.color[i].vec[1] = srcColor[3 * i].vec[1];
		dst_exp->v0.color[i].vec[2] = srcColor[3 * i].vec[2];
		dst_exp->v0.color[i].vec[3] = srcColor[3 * i].vec[3];
		
		dst_exp->v1.x[i] = srcVertex[3 * i + 1].vec[0];
		dst_exp->v1.y[i] = srcVertex[3 * i + 1].vec[1];
		dst_exp->v1.z[i] = srcVertex[3 * i + 1].vec[2];
		dst_exp->v1.w[i] = srcVertex[3 * i + 1].vec[3];
#ifdef TEXTURE_ENABLED
		dst_exp->v1.s[i] = srcTex[3 * i + 1].v0;
		dst_exp->v1.t[i] = srcTex[3 * i + 1].v1;
#endif
		dst_exp->v1.color[i].vec[0] = srcColor[3 * i + 1].vec[0];
		dst_exp->v1.color[i].vec[1] = srcColor[3 * i + 1].vec[1];
		dst_exp->v1.color[i].vec[2] = srcColor[3 * i + 1].vec[2];
		dst_exp->v1.color[i].vec[3] = srcColor[3 * i + 1].vec[3];

		dst_exp->v2.x[i] = srcVertex[3 * i + 2].vec[0];
		dst_exp->v2.y[i] = srcVertex[3 * i + 2].vec[1];
		dst_exp->v2.z[i] = srcVertex[3 * i + 2].vec[2];
		dst_exp->v2.w[i] = srcVertex[3 * i + 2].vec[3];
#ifdef TEXTURE_ENABLED
		dst_exp->v2.s[i] = srcTex[3 * i + 2].v0;
		dst_exp->v2.t[i] = srcTex[3 * i + 2].v1;
#endif
		dst_exp->v2.color[i].vec[0] = srcColor[3 * i + 2].vec[0];
		dst_exp->v2.color[i].vec[1] = srcColor[3 * i + 2].vec[1];
		dst_exp->v2.color[i].vec[2] = srcColor[3 * i + 2].vec[2];
		dst_exp->v2.color[i].vec[3] = srcColor[3 * i + 2].vec[3];
	}
	if (trianglesCount % 2){
		dst_exp->v0.x[expectedTrianglesCount - 1] = dst_exp->v0.x[expectedTrianglesCount - 2];
		dst_exp->v0.y[expectedTrianglesCount - 1] = dst_exp->v0.y[expectedTrianglesCount - 2];
		dst_exp->v0.z[expectedTrianglesCount - 1] = dst_exp->v0.z[expectedTrianglesCount - 2];
		dst_exp->v0.w[expectedTrianglesCount - 1] = dst_exp->v0.w[expectedTrianglesCount - 2];
#ifdef TEXTURE_ENABLED
		dst_exp->v0.s[expectedTrianglesCount - 1] = dst_exp->v0.s[expectedTrianglesCount - 2];
		dst_exp->v0.t[expectedTrianglesCount - 1] = dst_exp->v0.t[expectedTrianglesCount - 2];
#endif
		dst_exp->v0.color[expectedTrianglesCount - 1].vec[0] = dst_exp->v0.color[expectedTrianglesCount - 2].vec[0];
		dst_exp->v0.color[expectedTrianglesCount - 1].vec[1] = dst_exp->v0.color[expectedTrianglesCount - 2].vec[1];
		dst_exp->v0.color[expectedTrianglesCount - 1].vec[2] = dst_exp->v0.color[expectedTrianglesCount - 2].vec[2];
		dst_exp->v0.color[expectedTrianglesCount - 1].vec[3] = dst_exp->v0.color[expectedTrianglesCount - 2].vec[3];

		dst_exp->v1.x[expectedTrianglesCount - 1] = dst_exp->v1.x[expectedTrianglesCount - 2];
		dst_exp->v1.y[expectedTrianglesCount - 1] = dst_exp->v1.y[expectedTrianglesCount - 2];
		dst_exp->v1.z[expectedTrianglesCount - 1] = dst_exp->v1.z[expectedTrianglesCount - 2];
		dst_exp->v1.w[expectedTrianglesCount - 1] = dst_exp->v1.w[expectedTrianglesCount - 2];
#ifdef TEXTURE_ENABLED
		dst_exp->v1.s[expectedTrianglesCount - 1] = dst_exp->v1.s[expectedTrianglesCount - 2];
		dst_exp->v1.t[expectedTrianglesCount - 1] = dst_exp->v1.t[expectedTrianglesCount - 2];
#endif
		dst_exp->v1.color[expectedTrianglesCount - 1].vec[0] = dst_exp->v1.color[expectedTrianglesCount - 2].vec[0];
		dst_exp->v1.color[expectedTrianglesCount - 1].vec[1] = dst_exp->v1.color[expectedTrianglesCount - 2].vec[1];
		dst_exp->v1.color[expectedTrianglesCount - 1].vec[2] = dst_exp->v1.color[expectedTrianglesCount - 2].vec[2];
		dst_exp->v1.color[expectedTrianglesCount - 1].vec[3] = dst_exp->v1.color[expectedTrianglesCount - 2].vec[3];

		dst_exp->v2.x[expectedTrianglesCount - 1] = dst_exp->v2.x[expectedTrianglesCount - 2];
		dst_exp->v2.y[expectedTrianglesCount - 1] = dst_exp->v2.y[expectedTrianglesCount - 2];
		dst_exp->v2.z[expectedTrianglesCount - 1] = dst_exp->v2.z[expectedTrianglesCount - 2];
		dst_exp->v2.w[expectedTrianglesCount - 1] = dst_exp->v2.w[expectedTrianglesCount - 2];
#ifdef TEXTURE_ENABLED
		dst_exp->v2.s[expectedTrianglesCount - 1] = dst_exp->v2.s[expectedTrianglesCount - 2];
		dst_exp->v2.t[expectedTrianglesCount - 1] = dst_exp->v2.t[expectedTrianglesCount - 2];
#endif
		dst_exp->v2.color[expectedTrianglesCount - 1].vec[0] = dst_exp->v2.color[expectedTrianglesCount - 2].vec[0];
		dst_exp->v2.color[expectedTrianglesCount - 1].vec[1] = dst_exp->v2.color[expectedTrianglesCount - 2].vec[1];
		dst_exp->v2.color[expectedTrianglesCount - 1].vec[2] = dst_exp->v2.color[expectedTrianglesCount - 2].vec[2];
		dst_exp->v2.color[expectedTrianglesCount - 1].vec[3] = dst_exp->v2.color[expectedTrianglesCount - 2].vec[3];
	} else {
		// Do not correct 
	}
}

void setDst(TrianglePointers *dst, nm32f *buf, int outputTrianglesCount)
{
	dst->v0.x = buf;
	dst->v0.y = dst->v0.x + outputTrianglesCount;
	dst->v0.z = dst->v0.y + outputTrianglesCount;
	dst->v0.w = dst->v0.z + outputTrianglesCount;
#ifdef TEXTURE_ENABLED
	dst->v0.s = dst->v0.w + outputTrianglesCount;
	dst->v0.t = dst->v0.s + outputTrianglesCount;
    dst->v0.color = (v4nm32f *)(dst->v0.t + outputTrianglesCount);
#else
    dst->v0.color = (v4nm32f *)(dst->v0.w + outputTrianglesCount);
#endif

	dst->v1.x = buf + ATTR_PER_VERTEX * outputTrianglesCount;
	dst->v1.y = dst->v1.x + outputTrianglesCount;
	dst->v1.z = dst->v1.y + outputTrianglesCount;
	dst->v1.w = dst->v1.z + outputTrianglesCount;
#ifdef TEXTURE_ENABLED
	dst->v1.s = dst->v1.w + outputTrianglesCount;
	dst->v1.t = dst->v1.s + outputTrianglesCount;
    dst->v1.color = (v4nm32f *)(dst->v1.t + outputTrianglesCount);
#else
    dst->v1.color = (v4nm32f *)(dst->v1.w + outputTrianglesCount);
#endif
	
	dst->v2.x = buf + ATTR_PER_VERTEX * 2 * outputTrianglesCount;
	dst->v2.y = dst->v2.x + outputTrianglesCount;
	dst->v2.z = dst->v2.y + outputTrianglesCount;
	dst->v2.w = dst->v2.z + outputTrianglesCount;
#ifdef TEXTURE_ENABLED
	dst->v2.s = dst->v2.w + outputTrianglesCount;
	dst->v2.t = dst->v2.s + outputTrianglesCount;
    dst->v2.color = (v4nm32f *)(dst->v1.t + outputTrianglesCount);
#else
    dst->v2.color = (v4nm32f *)(dst->v2.w + outputTrianglesCount);
#endif
}

int repackToPrimitives_t_nOutputTriangles(int n)
{
	int trianglesCount = n;							// number of output triangles
	// ZERO_COUNT - количество выходных параметров, которые должны остаться неизменными после работы функции
	// Это число должно быть чётным, от него зависит адрес, с которого начинаются
	// некоторые массивы выходных координат. Функция пишет каждую координату 
	// по 64 бита (векторный процессор), так что выходной адрес должен быть чётным.
	// ZERO_COUNT - number of triangles at the end that should not be touched
	int ZERO_COUNT = 4;							
    int vertCount = trianglesCount * 3;				// number of input vertices
	int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
	int outputTrianglesCount = expectedTrianglesCount + ZERO_COUNT;
	// 10 floats - x, y, z, w, s, t, r, g, b, a
	//  8 floats - x, y, z, w,       r, g, b, a
	int outputCoordCount = ATTR_PER_VERTEX * 3 * outputTrianglesCount;	// number of output coords

    v4nm32f     srcVertex[vertCount];
    v4nm32f     srcColor[vertCount];
    v2nm32f     srcTex[vertCount];
	nm32f real_output[outputCoordCount];
	nm32f expected_output[outputCoordCount];
	
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

	// Initialize expected output
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
	res = repackToPrimitives_t(srcVertex, srcColor, srcTex, &dst, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL(real_output, expected_output, outputCoordCount);
    TEST_ARRAYS_EQUAL(dst.v0.x, dst_exp.v0.x, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v0.y, dst_exp.v0.y, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v0.z, dst_exp.v0.z, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v0.w, dst_exp.v0.w, outputTrianglesCount);
    TEST_VEC_ARRAYS_EQUAL(dst.v0.color, dst_exp.v0.color, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v1.x, dst_exp.v1.x, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v1.y, dst_exp.v1.y, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v1.z, dst_exp.v1.z, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v1.w, dst_exp.v1.w, outputTrianglesCount);
    TEST_VEC_ARRAYS_EQUAL(dst.v1.color, dst_exp.v1.color, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v2.x, dst_exp.v2.x, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v2.y, dst_exp.v2.y, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v2.z, dst_exp.v2.z, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v2.w, dst_exp.v2.w, outputTrianglesCount);
    TEST_VEC_ARRAYS_EQUAL(dst.v2.color, dst_exp.v2.color, outputTrianglesCount);
#ifdef TEXTURE_ENABLED
    TEST_ARRAYS_EQUAL(dst.v0.s, dst_exp.v0.s, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v0.t, dst_exp.v0.t, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v1.s, dst_exp.v1.s, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v1.t, dst_exp.v1.t, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v2.s, dst_exp.v2.s, outputTrianglesCount);
    TEST_ARRAYS_EQUAL(dst.v2.t, dst_exp.v2.t, outputTrianglesCount);
#endif
    return 0;
}

int repackToPrimitives_t_0_200_OutputTriangles_AllDataAreCorrect()
{
	int res = 0;
	for (int i = 0; i < 200; ++i){
    	res += repackToPrimitives_t_nOutputTriangles(i);
	}
	return res;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstVertexLengthIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 3;							// number of output triangles
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + expectedTrianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * (expectedTrianglesCount + 1)] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = 1;

    nm32f expectedDstVertex[outputCoordCount + expectedTrianglesCount]  = {0};
    for (int i = 0; i < outputCoordCount; i++){
        expectedDstVertex[i] = 1;
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + expectedTrianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstVertexIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount = 3;							// number of output triangles
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + expectedTrianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * expectedTrianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = (float)(4 * i + j);

    nm32f expectedDstVertex[outputCoordCount + expectedTrianglesCount]  = {0};
    for (int i = 0; i < trianglesCount; i++){
		expectedDstVertex[ 0 * expectedTrianglesCount + i] = srcVertex[3 * i].vec[0];
		expectedDstVertex[ 1 * expectedTrianglesCount + i] = srcVertex[3 * i].vec[1];
		expectedDstVertex[ 2 * expectedTrianglesCount + i] = srcVertex[3 * i].vec[2];
		expectedDstVertex[ 3 * expectedTrianglesCount + i] = srcVertex[3 * i].vec[3];
		expectedDstVertex[ 4 * expectedTrianglesCount + i] = srcVertex[3 * i + 1].vec[0];
		expectedDstVertex[ 5 * expectedTrianglesCount + i] = srcVertex[3 * i + 1].vec[1];
		expectedDstVertex[ 6 * expectedTrianglesCount + i] = srcVertex[3 * i + 1].vec[2];
		expectedDstVertex[ 7 * expectedTrianglesCount + i] = srcVertex[3 * i + 1].vec[3];
		expectedDstVertex[ 8 * expectedTrianglesCount + i] = srcVertex[3 * i + 2].vec[0];
		expectedDstVertex[ 9 * expectedTrianglesCount + i] = srcVertex[3 * i + 2].vec[1];
		expectedDstVertex[10 * expectedTrianglesCount + i] = srcVertex[3 * i + 2].vec[2];
		expectedDstVertex[11 * expectedTrianglesCount + i] = srcVertex[3 * i + 2].vec[3];
	}
	if (trianglesCount % 2){
		expectedDstVertex[ 1 * expectedTrianglesCount - 1] = expectedDstVertex[ 1 * expectedTrianglesCount - 2];
		expectedDstVertex[ 2 * expectedTrianglesCount - 1] = expectedDstVertex[ 2 * expectedTrianglesCount - 2];
		expectedDstVertex[ 3 * expectedTrianglesCount - 1] = expectedDstVertex[ 3 * expectedTrianglesCount - 2];
		expectedDstVertex[ 4 * expectedTrianglesCount - 1] = expectedDstVertex[ 4 * expectedTrianglesCount - 2];
		expectedDstVertex[ 5 * expectedTrianglesCount - 1] = expectedDstVertex[ 5 * expectedTrianglesCount - 2];
		expectedDstVertex[ 6 * expectedTrianglesCount - 1] = expectedDstVertex[ 6 * expectedTrianglesCount - 2];
		expectedDstVertex[ 7 * expectedTrianglesCount - 1] = expectedDstVertex[ 7 * expectedTrianglesCount - 2];
		expectedDstVertex[ 8 * expectedTrianglesCount - 1] = expectedDstVertex[ 8 * expectedTrianglesCount - 2];
		expectedDstVertex[ 9 * expectedTrianglesCount - 1] = expectedDstVertex[ 9 * expectedTrianglesCount - 2];
		expectedDstVertex[10 * expectedTrianglesCount - 1] = expectedDstVertex[10 * expectedTrianglesCount - 2];
		expectedDstVertex[11 * expectedTrianglesCount - 1] = expectedDstVertex[11 * expectedTrianglesCount - 2];
		expectedDstVertex[12 * expectedTrianglesCount - 1] = expectedDstVertex[12 * expectedTrianglesCount - 2];
	} else {
		// Do not correct 
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + expectedTrianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstColorLengthIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount = 3;							// number of output triangles
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * (expectedTrianglesCount + 1)] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcColor[i].vec[j] = 1;

    v4nm32f expectedDstColor[3 * (expectedTrianglesCount + 1)] = {0};

    for (int i = 0; i < 3 * expectedTrianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[i].vec[j] = 1;
		}
	}
	
	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * (expectedTrianglesCount + 1));

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstColorIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount = 3;							// number of output triangles
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * expectedTrianglesCount] = {0};
    v4nm32f     expectedDstColor[3 * expectedTrianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++){
        for (int j = 0; j < 4; j++){
            srcColor[i].vec[j] = (float)(4 * i + j);
		}
	}

    for (int i = 0; i < trianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[3 * i].vec[j] = srcColor[3 * i].vec[j];
            expectedDstColor[3 * i + 1].vec[j] = srcColor[3 * i + 1].vec[j];
            expectedDstColor[3 * i + 2].vec[j] = srcColor[3 * i + 2].vec[j];
		}
	}
	if (trianglesCount % 2) {
		for(int j = 0; j < 4; ++j){
            expectedDstColor[3 * trianglesCount].vec[j] 	= expectedDstColor[3 * (trianglesCount - 1)].vec[j]    ;
            expectedDstColor[3 * trianglesCount + 1].vec[j] = expectedDstColor[3 * (trianglesCount - 1) + 1].vec[j];
            expectedDstColor[3 * trianglesCount + 2].vec[j] = expectedDstColor[3 * (trianglesCount - 1) + 2].vec[j];
		}
	} else {
		// Do nothing
	}


	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * expectedTrianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES81Triangles_dstVertexLengthIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 81;							// number of output triangles
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + expectedTrianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * expectedTrianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = 1;

    nm32f expectedDstVertex[outputCoordCount + expectedTrianglesCount]  = {0};
    for (int i = 0; i < outputCoordCount; i++){
        expectedDstVertex[i] = 1;
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + expectedTrianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES81Triangles_dstVertexIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 81;							// number of output triangles
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + expectedTrianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * expectedTrianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = (float)(4 * i + j);

    nm32f expectedDstVertex[outputCoordCount + expectedTrianglesCount]  = {0};
    for (int i = 0; i < trianglesCount; i++){
		expectedDstVertex[ 0 * expectedTrianglesCount + i] = srcVertex[3 * i].vec[0];
		expectedDstVertex[ 1 * expectedTrianglesCount + i] = srcVertex[3 * i].vec[1];
		expectedDstVertex[ 2 * expectedTrianglesCount + i] = srcVertex[3 * i].vec[2];
		expectedDstVertex[ 3 * expectedTrianglesCount + i] = srcVertex[3 * i].vec[3];
		expectedDstVertex[ 4 * expectedTrianglesCount + i] = srcVertex[3 * i + 1].vec[0];
		expectedDstVertex[ 5 * expectedTrianglesCount + i] = srcVertex[3 * i + 1].vec[1];
		expectedDstVertex[ 6 * expectedTrianglesCount + i] = srcVertex[3 * i + 1].vec[2];
		expectedDstVertex[ 7 * expectedTrianglesCount + i] = srcVertex[3 * i + 1].vec[3];
		expectedDstVertex[ 8 * expectedTrianglesCount + i] = srcVertex[3 * i + 2].vec[0];
		expectedDstVertex[ 9 * expectedTrianglesCount + i] = srcVertex[3 * i + 2].vec[1];
		expectedDstVertex[10 * expectedTrianglesCount + i] = srcVertex[3 * i + 2].vec[2];
		expectedDstVertex[11 * expectedTrianglesCount + i] = srcVertex[3 * i + 2].vec[3];
	}
	if (trianglesCount % 2){
		expectedDstVertex[ 1 * expectedTrianglesCount - 1] = expectedDstVertex[ 1 * expectedTrianglesCount - 2];
		expectedDstVertex[ 2 * expectedTrianglesCount - 1] = expectedDstVertex[ 2 * expectedTrianglesCount - 2];
		expectedDstVertex[ 3 * expectedTrianglesCount - 1] = expectedDstVertex[ 3 * expectedTrianglesCount - 2];
		expectedDstVertex[ 4 * expectedTrianglesCount - 1] = expectedDstVertex[ 4 * expectedTrianglesCount - 2];
		expectedDstVertex[ 5 * expectedTrianglesCount - 1] = expectedDstVertex[ 5 * expectedTrianglesCount - 2];
		expectedDstVertex[ 6 * expectedTrianglesCount - 1] = expectedDstVertex[ 6 * expectedTrianglesCount - 2];
		expectedDstVertex[ 7 * expectedTrianglesCount - 1] = expectedDstVertex[ 7 * expectedTrianglesCount - 2];
		expectedDstVertex[ 8 * expectedTrianglesCount - 1] = expectedDstVertex[ 8 * expectedTrianglesCount - 2];
		expectedDstVertex[ 9 * expectedTrianglesCount - 1] = expectedDstVertex[ 9 * expectedTrianglesCount - 2];
		expectedDstVertex[10 * expectedTrianglesCount - 1] = expectedDstVertex[10 * expectedTrianglesCount - 2];
		expectedDstVertex[11 * expectedTrianglesCount - 1] = expectedDstVertex[11 * expectedTrianglesCount - 2];
		expectedDstVertex[12 * expectedTrianglesCount - 1] = expectedDstVertex[12 * expectedTrianglesCount - 2];
	} else {
		// Do not correct 
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + expectedTrianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES81_dstColorLengthIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount = 81;							// number of output triangles
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * (expectedTrianglesCount + 1)] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcColor[i].vec[j] = 1;

    v4nm32f expectedDstColor[3 * (expectedTrianglesCount + 1)] = {0};

    for (int i = 0; i < 3 * expectedTrianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[i].vec[j] = 1;
		}
	}
	
	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * (expectedTrianglesCount + 1));

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES81_dstColorIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount = 81;							// number of output triangles
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * expectedTrianglesCount] = {0};
    v4nm32f     expectedDstColor[3 * expectedTrianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++){
        for (int j = 0; j < 4; j++){
            srcColor[i].vec[j] = (float)(4 * i + j);
		}
	}

    for (int i = 0; i < trianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[3 * i].vec[j] = srcColor[3 * i].vec[j];
            expectedDstColor[3 * i + 1].vec[j] = srcColor[3 * i + 1].vec[j];
            expectedDstColor[3 * i + 2].vec[j] = srcColor[3 * i + 2].vec[j];
		}
	}
	if (trianglesCount % 2) {
		for(int j = 0; j < 4; ++j){
            expectedDstColor[3 * trianglesCount].vec[j] 	= expectedDstColor[3 * (trianglesCount - 1)].vec[j]    ;
            expectedDstColor[3 * trianglesCount + 1].vec[j] = expectedDstColor[3 * (trianglesCount - 1) + 1].vec[j];
            expectedDstColor[3 * trianglesCount + 2].vec[j] = expectedDstColor[3 * (trianglesCount - 1) + 2].vec[j];
		}
	} else {
		// Do nothing
	}


	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * expectedTrianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES90Triangles_dstVertexLengthIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 90;							// number of output triangles
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + expectedTrianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * expectedTrianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = 1;

    nm32f expectedDstVertex[outputCoordCount + expectedTrianglesCount]  = {0};
    for (int i = 0; i < outputCoordCount; i++){
        expectedDstVertex[i] = 1;
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + expectedTrianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES90Triangles_dstVertexIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 90;							// number of output triangles
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + expectedTrianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * expectedTrianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = (float)(4 * i + j);

    nm32f expectedDstVertex[outputCoordCount + expectedTrianglesCount]  = {0};
    for (int i = 0; i < trianglesCount; i++){
		expectedDstVertex[ 0 * expectedTrianglesCount + i] = srcVertex[3 * i].vec[0];
		expectedDstVertex[ 1 * expectedTrianglesCount + i] = srcVertex[3 * i].vec[1];
		expectedDstVertex[ 2 * expectedTrianglesCount + i] = srcVertex[3 * i].vec[2];
		expectedDstVertex[ 3 * expectedTrianglesCount + i] = srcVertex[3 * i].vec[3];
		expectedDstVertex[ 4 * expectedTrianglesCount + i] = srcVertex[3 * i + 1].vec[0];
		expectedDstVertex[ 5 * expectedTrianglesCount + i] = srcVertex[3 * i + 1].vec[1];
		expectedDstVertex[ 6 * expectedTrianglesCount + i] = srcVertex[3 * i + 1].vec[2];
		expectedDstVertex[ 7 * expectedTrianglesCount + i] = srcVertex[3 * i + 1].vec[3];
		expectedDstVertex[ 8 * expectedTrianglesCount + i] = srcVertex[3 * i + 2].vec[0];
		expectedDstVertex[ 9 * expectedTrianglesCount + i] = srcVertex[3 * i + 2].vec[1];
		expectedDstVertex[10 * expectedTrianglesCount + i] = srcVertex[3 * i + 2].vec[2];
		expectedDstVertex[11 * expectedTrianglesCount + i] = srcVertex[3 * i + 2].vec[3];
	}
	if (trianglesCount % 2){
		expectedDstVertex[ 1 * expectedTrianglesCount - 1] = expectedDstVertex[ 1 * expectedTrianglesCount - 2];
		expectedDstVertex[ 2 * expectedTrianglesCount - 1] = expectedDstVertex[ 2 * expectedTrianglesCount - 2];
		expectedDstVertex[ 3 * expectedTrianglesCount - 1] = expectedDstVertex[ 3 * expectedTrianglesCount - 2];
		expectedDstVertex[ 4 * expectedTrianglesCount - 1] = expectedDstVertex[ 4 * expectedTrianglesCount - 2];
		expectedDstVertex[ 5 * expectedTrianglesCount - 1] = expectedDstVertex[ 5 * expectedTrianglesCount - 2];
		expectedDstVertex[ 6 * expectedTrianglesCount - 1] = expectedDstVertex[ 6 * expectedTrianglesCount - 2];
		expectedDstVertex[ 7 * expectedTrianglesCount - 1] = expectedDstVertex[ 7 * expectedTrianglesCount - 2];
		expectedDstVertex[ 8 * expectedTrianglesCount - 1] = expectedDstVertex[ 8 * expectedTrianglesCount - 2];
		expectedDstVertex[ 9 * expectedTrianglesCount - 1] = expectedDstVertex[ 9 * expectedTrianglesCount - 2];
		expectedDstVertex[10 * expectedTrianglesCount - 1] = expectedDstVertex[10 * expectedTrianglesCount - 2];
		expectedDstVertex[11 * expectedTrianglesCount - 1] = expectedDstVertex[11 * expectedTrianglesCount - 2];
		expectedDstVertex[12 * expectedTrianglesCount - 1] = expectedDstVertex[12 * expectedTrianglesCount - 2];
	} else {
		// Do not correct 
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + expectedTrianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES90_dstColorLengthIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount = 90;							// number of output triangles
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * (expectedTrianglesCount + 1)] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcColor[i].vec[j] = 1;

    v4nm32f expectedDstColor[3 * (expectedTrianglesCount + 1)] = {0};

    for (int i = 0; i < 3 * expectedTrianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[i].vec[j] = 1;
		}
	}
	
	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * (expectedTrianglesCount + 1));

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES90_dstColorIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount = 90;							// number of output triangles
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * expectedTrianglesCount] = {0};
    v4nm32f     expectedDstColor[3 * expectedTrianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++){
        for (int j = 0; j < 4; j++){
            srcColor[i].vec[j] = (float)(4 * i + j);
		}
	}

    for (int i = 0; i < trianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[3 * i].vec[j] = srcColor[3 * i].vec[j];
            expectedDstColor[3 * i + 1].vec[j] = srcColor[3 * i + 1].vec[j];
            expectedDstColor[3 * i + 2].vec[j] = srcColor[3 * i + 2].vec[j];
		}
	}
	if (trianglesCount % 2) {
		for(int j = 0; j < 4; ++j){
            expectedDstColor[3 * trianglesCount].vec[j] 	= expectedDstColor[3 * (trianglesCount - 1)].vec[j]    ;
            expectedDstColor[3 * trianglesCount + 1].vec[j] = expectedDstColor[3 * (trianglesCount - 1) + 1].vec[j];
            expectedDstColor[3 * trianglesCount + 2].vec[j] = expectedDstColor[3 * (trianglesCount - 1) + 2].vec[j];
		}
	} else {
		// Do nothing
	}


	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * expectedTrianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES15Vertexes_returns6()
{
	// Arrange
	constexpr int trianglesCount = 5;							// number of output triangles
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + expectedTrianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * expectedTrianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

	// Act
	int res;
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
	

	// Assert
    TEST_ASSERT (expectedTrianglesCount == res);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES45Vertexes_returns16()
{
	// Arrange
	constexpr int trianglesCount = 15;							// number of output triangles
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + expectedTrianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * expectedTrianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

	// Act
	int res;
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
	

	// Assert
    TEST_ASSERT (expectedTrianglesCount == res);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES90Vertexes_returns30()
{
	// Arrange
	constexpr int trianglesCount = 30;							// number of output triangles
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + expectedTrianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * expectedTrianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

	// Act
	int res;
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
	

	// Assert
    TEST_ASSERT (expectedTrianglesCount == res);

    return 0;
}

clock_t vertexPrimitiveRepack_modeIsGL_TRIANGLES_192Vertices()
{
	// Arrange
	constexpr int trianglesCount = 64;							// number of output triangles
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

	clock_t t1, t2, dt;
	
	// Act
	int res;
	t1 = clock();
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
	t2 = clock();
	dt = t2 - t1;	
	(void) res;

    return dt;
}

clock_t vertexPrimitiveRepack_modeIsGL_TRIANGLES_48Vertices()
{
	// Arrange
	constexpr int trianglesCount = 16;							// number of output triangles
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

	clock_t t1, t2, dt;
	
	// Act
	int res;
	t1 = clock();
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
	t2 = clock();
	dt = t2 - t1;	
	(void) res;

    return dt;
}
