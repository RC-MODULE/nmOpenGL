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
		printf("%f ", ptr[i]);
	}
	puts("");
}

int analyzeStackParams()
{
	constexpr int trianglesCount = 3;							// number of output triangles
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int expectedTrianglesCount = trianglesCount + trianglesCount % 2;
	constexpr int outputCoordCount = 4 * 3 * expectedTrianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    v2nm32f     srcTex[vertCount] = {0};
	float x0[expectedTrianglesCount];
	float y0[expectedTrianglesCount];
	float z0[expectedTrianglesCount];
	float w0[expectedTrianglesCount];
	float s0[expectedTrianglesCount];
	float t0[expectedTrianglesCount];
	float x1[expectedTrianglesCount];
	float y1[expectedTrianglesCount];
	float z1[expectedTrianglesCount];
	float w1[expectedTrianglesCount];
	float s1[expectedTrianglesCount];
	float t1[expectedTrianglesCount];
	float x2[expectedTrianglesCount];
	float y2[expectedTrianglesCount];
	float z2[expectedTrianglesCount];
	float w2[expectedTrianglesCount];
	float s2[expectedTrianglesCount];
	float t2[expectedTrianglesCount];
	TrianglePointers dst;
	dst.v0.x = x0;
	dst.v0.y = y0;
	dst.v0.z = z0;
	dst.v0.w = w0;
	dst.v0.s = s0;
	dst.v0.t = t0;
	dst.v1.x = x1;
	dst.v1.y = y1;
	dst.v1.z = z1;
	dst.v1.w = w1;
	dst.v1.s = s1;
	dst.v1.t = t1;
	dst.v2.x = x1;
	dst.v2.y = y1;
	dst.v2.z = z1;
	dst.v2.w = w1;
	dst.v2.s = s1;
	dst.v2.t = t1;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = (float)(4 * i + j);

    nm32f expectedDstVertex[outputCoordCount + expectedTrianglesCount]  = {0};
    for (int i = 0; i < trianglesCount; i++){
		x0[i] = srcVertex[3 * i].vec[0];
		y0[i] = srcVertex[3 * i].vec[1];
		z0[i] = srcVertex[3 * i].vec[2];
		w0[i] = srcVertex[3 * i].vec[3];
		//s0[i];
		//t0[i];
		x1[i] = srcVertex[3 * i + 1].vec[0];
		y1[i] = srcVertex[3 * i + 1].vec[1];
		z1[i] = srcVertex[3 * i + 1].vec[2];
		w1[i] = srcVertex[3 * i + 1].vec[3];
		//s1[i];
		//t1[i];
		x2[i] = srcVertex[3 * i + 2].vec[0];
		y2[i] = srcVertex[3 * i + 2].vec[1];
		z2[i] = srcVertex[3 * i + 2].vec[2];
		w2[i] = srcVertex[3 * i + 2].vec[3];
		//s2[i];
		//t2[i];
	}
	if (trianglesCount % 2){
		x0[expectedTrianglesCount - 1] = x0[expectedTrianglesCount - 2];
		y0[expectedTrianglesCount - 1] = y0[expectedTrianglesCount - 2];
		z0[expectedTrianglesCount - 1] = z0[expectedTrianglesCount - 2];
		w0[expectedTrianglesCount - 1] = w0[expectedTrianglesCount - 2];
		//s0[i];
		//t0[i];
		x1[expectedTrianglesCount - 1] = x1[expectedTrianglesCount - 2];
		y1[expectedTrianglesCount - 1] = y1[expectedTrianglesCount - 2];
		z1[expectedTrianglesCount - 1] = z1[expectedTrianglesCount - 2];
		w1[expectedTrianglesCount - 1] = w1[expectedTrianglesCount - 2];
		//s1[i];
		//t1[i];
		x2[expectedTrianglesCount - 1] = x2[expectedTrianglesCount - 2];
		y2[expectedTrianglesCount - 1] = y2[expectedTrianglesCount - 2];
		z2[expectedTrianglesCount - 1] = z2[expectedTrianglesCount - 2];
		w2[expectedTrianglesCount - 1] = w2[expectedTrianglesCount - 2];
		//s2[i];
		//t2[i];
	} else {
		// Do not correct 
	}

	// Act
    int res;
	res = repackToPrimitives_t(srcVertex, srcColor, srcTex, &dst, vertCount);
	printf("dst pointer = %p \n\r", &dst);
	printf("dst.v0 pointer = %p \n\r", &(dst.v0));
	printf("dst.v1 pointer = %p \n\r", &(dst.v1));
	printf("dst.v2 pointer = %p \n\r", &(dst.v2));
	printf("dst.v1.w pointer = %p \n\r", &(dst.v1.w));
	printf("dst.v1.w = %x \n\r", dst.v1.w);
	printf("res = %p \n\r", res);

	Print(x0, expectedTrianglesCount);
	Print(y0, expectedTrianglesCount);
	Print(z0, expectedTrianglesCount);
	Print(w0, expectedTrianglesCount);
	Print(x1, expectedTrianglesCount);
	Print(y1, expectedTrianglesCount);
	Print(z1, expectedTrianglesCount);
	Print(w1, expectedTrianglesCount);
	Print(x2, expectedTrianglesCount);
	Print(y2, expectedTrianglesCount);
	Print(z2, expectedTrianglesCount);
	Print(w2, expectedTrianglesCount);

	// Assert
    //TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + expectedTrianglesCount);

    return 0;
}
//


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
	RUN_TEST(analyzeStackParams);
	return 0;
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
