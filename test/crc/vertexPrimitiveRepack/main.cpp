#include <iostream>
#include <cassert>
#include <cerrno>

#include "service.h"
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
int vertexPrimitiveRepack_modeIsGL_TRIANGLES15Vertexes_returns5();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES45Vertexes_returns15();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES90Vertexes_returns30();

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_dstVertexLengthIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_dstVertexIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_dstColorLengthIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_dstColorIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP81Triangles_dstVertexLengthIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP81Triangles_dstVertexIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP81_dstColorLengthIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP81_dstColorIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP90Triangles_dstVertexLengthIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP90Triangles_dstVertexIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP90_dstColorLengthIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP90_dstColorIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP17Vertexes_returns15();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP47Vertexes_returns45();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP92Vertexes_returns90();

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_dstVertexLengthIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_dstVertexIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_dstColorLengthIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_dstColorIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN81Triangles_dstVertexLengthIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN81Triangles_dstVertexIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN81_dstColorLengthIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN81_dstColorIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN90Triangles_dstVertexLengthIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN90Triangles_dstVertexIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN90_dstColorLengthIsCorrect ();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN90_dstColorIsCorrect();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN15Vertexes_returns13();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN45Vertexes_returns43();
int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN90Vertexes_returns88();

// Performance tests
clock_t vertexPrimitiveRepack_modeIsGL_TRIANGLES_192Vertices();
clock_t vertexPrimitiveRepack_modeIsGL_TRIANGLES_48Vertices();
clock_t vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_66Vertices();
clock_t vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_34Vertices();
clock_t vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_66Vertices();
clock_t vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_34Vertices();

int main(int argc, char **argv)
{
    puts("VertexPrimitiveRepack tests: ");
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstVertexLengthIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstVertexIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstColorLengthIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstColorIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES81Triangles_dstVertexLengthIsCorrect);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES81Triangles_dstVertexIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES81_dstColorLengthIsCorrect);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES81_dstColorIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES90Triangles_dstVertexLengthIsCorrect);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES90Triangles_dstVertexIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES90_dstColorLengthIsCorrect);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES90_dstColorIsCorrect);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES15Vertexes_returns5);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES45Vertexes_returns15);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES90Vertexes_returns30);	

    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_dstVertexLengthIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_dstVertexIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_dstColorLengthIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_dstColorIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP81Triangles_dstVertexLengthIsCorrect);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP81Triangles_dstVertexIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP81_dstColorLengthIsCorrect);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP81_dstColorIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP90Triangles_dstVertexLengthIsCorrect);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP90Triangles_dstVertexIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP90_dstColorLengthIsCorrect);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP90_dstColorIsCorrect);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP17Vertexes_returns15);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP47Vertexes_returns45);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP92Vertexes_returns90);	

    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_dstVertexLengthIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_dstVertexIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_dstColorLengthIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_dstColorIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN81Triangles_dstVertexLengthIsCorrect);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN81Triangles_dstVertexIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN81_dstColorLengthIsCorrect);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN81_dstColorIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN90Triangles_dstVertexLengthIsCorrect);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN90Triangles_dstVertexIsCorrect);
    RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN90_dstColorLengthIsCorrect);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN90_dstColorIsCorrect);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN15Vertexes_returns13);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN45Vertexes_returns43);
	RUN_TEST(vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN90Vertexes_returns88);	

	puts("");
    puts("Performance tests: ");
	clock_t dt[6];
	dt[0] = vertexPrimitiveRepack_modeIsGL_TRIANGLES_192Vertices();
	dt[1] = vertexPrimitiveRepack_modeIsGL_TRIANGLES_48Vertices();
	dt[2] = vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_66Vertices();
	dt[3] = vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_34Vertices();
	dt[4] = vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_66Vertices();	
	dt[5] = vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_34Vertices();
	for (int i = 0; i < 6; ++i){
		printf("dt[%i] = %i\n\r", i + 1, dt[i]);
	}

    puts("OK");
	return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstVertexLengthIsCorrect()
{
	// Arrange
    v4nm32f     srcVertex[9] = {0};
    v4nm32f     srcColor[9] = {0};
    nm32f       dstVertex[48] = {0} ;
    v4nm32f     dstColor[12] = {0};

    int mode = NMGL_TRIANGLES;
    int vertCount = 9;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = 1;

    nm32f expectedDstVertex[48]  = {0};
    for (int i = 0; i < 36; i++)
        expectedDstVertex[i] = 1;
	
	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
  
	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, 48);
    
    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstVertexIsCorrect ()
{
	// Arrange
	v4nm32f     srcVertex[9] =	{
	  	  							1,1,1,0,
									2,3,2,1,
	  								2,1,1,0,
	  								2,1,2,1,
	  								2,3,1,0,
	  								3,1,2,1,
	  								3,1,1,0,
	  								2,3,1,1,
	  								3,3,2,0
	  						 	};
	v4nm32f     srcColor[9] = {0};
	nm32f       dstVertex[48] = {0} ;
	v4nm32f     dstColor[12] = {0};
	
	int mode = NMGL_TRIANGLES;
	int vertCount = 9;
	
	nm32f expectedDstVertex[36] =	{
	  	  								1,2,3,
	  									1,1,1,
	  									1,2,1,
	  									0,1,0,
	  									2,2,2,
	  									3,3,3,
	  									2,1,1,
	  									1,0,1,
	  									2,3,3,
	  									1,1,3,
	  									1,2,2,
	  									0,1,0
									};
	
	// Act
	vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
	
	// Assert
	TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, 36);
	
	return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstColorLengthIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount = 3;							// number of output triangles
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};	
    v4nm32f     dstColor[3 * (trianglesCount + 1)] = {0};	// one more triangle filled with zeroes

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcColor[i].vec[j] = 1;

    v4nm32f expectedDstColor[3 * (trianglesCount + 1)] = {0};

    for (int i = 0; i < 3 * trianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[i].vec[j] = 1;
		}
	}
	
	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * (trianglesCount + 1));

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_dstColorIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount = 3;							// number of output triangles
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] =	{
										{1,1,1,1},
										{2,2,2,2},
										{3,3,3,3},
										{4,4,4,4},
										{5,5,5,5},
										{6,6,6,6},
										{7,7,7,7},
										{8,8,8,8},
										{9,9,9,9}
										};
    nm32f       dstVertex[outputCoordCount] = {0};	
    v4nm32f     dstColor[3 * trianglesCount] = {0};	
    v4nm32f expectedDstColor[3 * trianglesCount] =	{
													{1,1,1,1},
													{2,2,2,2},
													{3,3,3,3},
													{4,4,4,4},
													{5,5,5,5},
													{6,6,6,6},
													{7,7,7,7},
													{8,8,8,8},
													{9,9,9,9}
													};
    int mode = NMGL_TRIANGLES;

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES81Triangles_dstVertexLengthIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 81;							// number of output triangles
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + trianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = 1;

    nm32f expectedDstVertex[outputCoordCount + trianglesCount]  = {0};
    for (int i = 0; i < outputCoordCount; i++){
        expectedDstVertex[i] = 1;
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES81Triangles_dstVertexIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 81;							// number of output triangles
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + trianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = (float)(4 * i + j);

    nm32f expectedDstVertex[outputCoordCount + trianglesCount]  = {0};
    for (int i = 0; i < trianglesCount; i++){
		expectedDstVertex[ 0 * trianglesCount + i] = srcVertex[3 * i].vec[0];
		expectedDstVertex[ 1 * trianglesCount + i] = srcVertex[3 * i].vec[1];
		expectedDstVertex[ 2 * trianglesCount + i] = srcVertex[3 * i].vec[2];
		expectedDstVertex[ 3 * trianglesCount + i] = srcVertex[3 * i].vec[3];
		expectedDstVertex[ 4 * trianglesCount + i] = srcVertex[3 * i + 1].vec[0];
		expectedDstVertex[ 5 * trianglesCount + i] = srcVertex[3 * i + 1].vec[1];
		expectedDstVertex[ 6 * trianglesCount + i] = srcVertex[3 * i + 1].vec[2];
		expectedDstVertex[ 7 * trianglesCount + i] = srcVertex[3 * i + 1].vec[3];
		expectedDstVertex[ 8 * trianglesCount + i] = srcVertex[3 * i + 2].vec[0];
		expectedDstVertex[ 9 * trianglesCount + i] = srcVertex[3 * i + 2].vec[1];
		expectedDstVertex[10 * trianglesCount + i] = srcVertex[3 * i + 2].vec[2];
		expectedDstVertex[11 * trianglesCount + i] = srcVertex[3 * i + 2].vec[3];
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES81_dstColorLengthIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount = 81;							// number of output triangles
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};	
    v4nm32f     dstColor[3 * (trianglesCount + 1)] = {0};	// one more triangle filled with zeroes

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcColor[i].vec[j] = 1;

    v4nm32f expectedDstColor[3 * (trianglesCount + 1)] = {0};

    for (int i = 0; i < 3 * trianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[i].vec[j] = 1;
		}
	}
	
	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * (trianglesCount + 1));

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES81_dstColorIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount 	= 81;						// number of output triangles
    constexpr int vertCount 		= trianglesCount * 3;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	
    v4nm32f	expectedDstColor[3 * trianglesCount]	= {0};
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

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES90Triangles_dstVertexLengthIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 90;							// number of output triangles
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + trianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = 1;

    nm32f expectedDstVertex[outputCoordCount + trianglesCount]  = {0};
    for (int i = 0; i < outputCoordCount; i++){
        expectedDstVertex[i] = 1;
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES90Triangles_dstVertexIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 90;							// number of output triangles
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + trianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = (float)(4 * i + j);

    nm32f expectedDstVertex[outputCoordCount + trianglesCount]  = {0};
    for (int i = 0; i < trianglesCount; i++){
		expectedDstVertex[ 0 * trianglesCount + i] = srcVertex[3 * i].vec[0];
		expectedDstVertex[ 1 * trianglesCount + i] = srcVertex[3 * i].vec[1];
		expectedDstVertex[ 2 * trianglesCount + i] = srcVertex[3 * i].vec[2];
		expectedDstVertex[ 3 * trianglesCount + i] = srcVertex[3 * i].vec[3];
		expectedDstVertex[ 4 * trianglesCount + i] = srcVertex[3 * i + 1].vec[0];
		expectedDstVertex[ 5 * trianglesCount + i] = srcVertex[3 * i + 1].vec[1];
		expectedDstVertex[ 6 * trianglesCount + i] = srcVertex[3 * i + 1].vec[2];
		expectedDstVertex[ 7 * trianglesCount + i] = srcVertex[3 * i + 1].vec[3];
		expectedDstVertex[ 8 * trianglesCount + i] = srcVertex[3 * i + 2].vec[0];
		expectedDstVertex[ 9 * trianglesCount + i] = srcVertex[3 * i + 2].vec[1];
		expectedDstVertex[10 * trianglesCount + i] = srcVertex[3 * i + 2].vec[2];
		expectedDstVertex[11 * trianglesCount + i] = srcVertex[3 * i + 2].vec[3];
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
	
	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES90_dstColorLengthIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount = 90;							// number of output triangles
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};	
    v4nm32f     dstColor[3 * (trianglesCount + 1)] = {0};	// one more triangle filled with zeroes

    int mode = NMGL_TRIANGLES;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcColor[i].vec[j] = 1;

    v4nm32f expectedDstColor[3 * (trianglesCount + 1)] = {0};

    for (int i = 0; i < 3 * trianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[i].vec[j] = 1;
		}
	}
	
	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * (trianglesCount + 1));

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES90_dstColorIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount 	= 90;						// number of output triangles
    constexpr int vertCount 		= trianglesCount * 3;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	
    v4nm32f	expectedDstColor[3 * trianglesCount]	= {0};
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

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES15Vertexes_returns5()
{
	// Arrange
	constexpr int trianglesCount = 5;							// number of output triangles
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};	
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

	// Act
	int res;
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
	

	// Assert
    TEST_ASSERT (trianglesCount == res);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES45Vertexes_returns15()
{
	// Arrange
	constexpr int trianglesCount = 15;							// number of output triangles
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

	// Act
	int res;
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ASSERT (trianglesCount == res);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES90Vertexes_returns30()
{
	// Arrange
	constexpr int trianglesCount = 30;							// number of output triangles
    constexpr int vertCount = trianglesCount * 3;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLES;

	// Act
	int res;
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ASSERT (trianglesCount == res);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_dstVertexLengthIsCorrect ()
{
	// Arrange
    v4nm32f     srcVertex[5] = {0};
    v4nm32f     srcColor[5] = {0};
    nm32f       dstVertex[48] = {0} ;
    v4nm32f     dstColor[12] = {0};

    int mode = NMGL_TRIANGLE_STRIP;
    int vertCount = 5;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = 1;

    nm32f expectedDstVertex[48]  = {0};
    for (int i = 0; i < 36; i++)
        expectedDstVertex[i] = 1;

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, 48);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_dstVertexIsCorrect ()
{
	// Arrange
	v4nm32f     srcVertex[5] =	{
									1,1,1,0,
	    							2,3,2,1,
	  								2,1,1,0,
	  								3,3,2,1,
	  								3,1,2,0
	  							 };
	v4nm32f     srcColor[5] = {0};
	nm32f       dstVertex[48] = {0} ;
	v4nm32f     dstColor[12] = {0};
	
	int mode = NMGL_TRIANGLE_STRIP;
	int vertCount = 5;
	
	nm32f expectedDstVertex[36] =	{
										1,2,2,
										1,1,1,
										1,1,1,
										0,0,0,
										2,2,3,
										3,3,3,
										2,2,2,
										1,1,1,
										2,3,3,
										1,3,1,
										1,2,2,
										0,1,0
									};
	// Act	
	vertexPrimitiveRepack(srcVertex, srcColor, dstVertex, dstColor, mode, vertCount);
	
	// Assert
	TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, 36);
	
	return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_dstColorLengthIsCorrect ()
{
	// Arrange 
    v4nm32f     srcVertex[5] = {0};
    v4nm32f     srcColor[5] = {0};
    nm32f       dstVertex[48] = {0} ;
    v4nm32f     dstColor[12] = {0};
    v4nm32f     expectedDstColor[12] = {0};

    int mode = NMGL_TRIANGLE_STRIP;
    int vertCount = 5;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcColor[i].vec[j] = 1;

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 4; j++)
            expectedDstColor[i].vec[j] = 1;

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 12);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_dstColorIsCorrect ()
{
	// Arrange
    v4nm32f     srcColor[5] =	{
									{1,1,1,1},
									{2,2,2,2},
									{3,3,3,3},
									{4,4,4,4},
									{5,5,5,5}
								};
    v4nm32f     srcVertex[5] = {0};
    nm32f       dstVertex[36] = {0} ;
    v4nm32f     dstColor[9] = {0};
    v4nm32f     expectedDstColor[9] =	{
											{1,1,1,1},
											{2,2,2,2},
											{3,3,3,3},
											{3,3,3,3},
											{2,2,2,2},
											{4,4,4,4},
											{3,3,3,3},
											{4,4,4,4},
											{5,5,5,5}
										};

    int mode = NMGL_TRIANGLE_STRIP;
    int vertCount = 5;
	
	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 9);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP81Triangles_dstVertexLengthIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 81;							// number of output triangles
    constexpr int vertCount = trianglesCount + 2;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + trianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLE_STRIP;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = 1;

    nm32f expectedDstVertex[outputCoordCount + trianglesCount]  = {0};
    for (int i = 0; i < outputCoordCount; i++){
        expectedDstVertex[i] = 1;
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP81Triangles_dstVertexIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 81;							// number of output triangles
    constexpr int vertCount = trianglesCount + 2;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + trianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLE_STRIP;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = (float)(4 * i + j);

    nm32f expectedDstVertex[outputCoordCount + trianglesCount]  = {0};
    for (int i = 0; i < trianglesCount; i++){
		expectedDstVertex[ 0 * trianglesCount + i] = srcVertex[i + (i&1)].vec[0];	// Для чётных i: i, для нечётных i: i + 1
		expectedDstVertex[ 1 * trianglesCount + i] = srcVertex[i + (i&1)].vec[1];
		expectedDstVertex[ 2 * trianglesCount + i] = srcVertex[i + (i&1)].vec[2];
		expectedDstVertex[ 3 * trianglesCount + i] = srcVertex[i + (i&1)].vec[3];
		expectedDstVertex[ 4 * trianglesCount + i] = srcVertex[i + ((~i)&1)].vec[0];	// Для чётных i: i + 1, для нечётных i: i
		expectedDstVertex[ 5 * trianglesCount + i] = srcVertex[i + ((~i)&1)].vec[1];
		expectedDstVertex[ 6 * trianglesCount + i] = srcVertex[i + ((~i)&1)].vec[2];
		expectedDstVertex[ 7 * trianglesCount + i] = srcVertex[i + ((~i)&1)].vec[3];
		expectedDstVertex[ 8 * trianglesCount + i] = srcVertex[i + 2].vec[0];
		expectedDstVertex[ 9 * trianglesCount + i] = srcVertex[i + 2].vec[1];
		expectedDstVertex[10 * trianglesCount + i] = srcVertex[i + 2].vec[2];
		expectedDstVertex[11 * trianglesCount + i] = srcVertex[i + 2].vec[3];
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
	
	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP81_dstColorLengthIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount = 81;							// number of output triangles
    constexpr int vertCount = trianglesCount + 2;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};	
    v4nm32f     dstColor[3 * (trianglesCount + 1)] = {0};	// one more triangle filled with zeroes

    int mode = NMGL_TRIANGLE_STRIP;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcColor[i].vec[j] = 1;

    v4nm32f expectedDstColor[3 * (trianglesCount + 1)] = {0};

    for (int i = 0; i < 3 * trianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[i].vec[j] = 1;
		}
	}
	
	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * (trianglesCount + 1));

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP81_dstColorIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount 	= 81;						// number of output triangles
    constexpr int vertCount 		= trianglesCount + 2;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	
    v4nm32f	expectedDstColor[3 * trianglesCount]	= {0};
    int mode = NMGL_TRIANGLE_STRIP;

    for (int i = 0; i < vertCount; i++){
        for (int j = 0; j < 4; j++){
            srcColor[i].vec[j] = (float)(4 * i + j);
		}
	}

    for (int i = 0; i < trianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[3 * i].vec[j] = srcColor[i + (i&1)].vec[j];
            expectedDstColor[3 * i + 1].vec[j] = srcColor[i + ((~i)&1)].vec[j];
            expectedDstColor[3 * i + 2].vec[j] = srcColor[i + 2].vec[j];
		}
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP90Triangles_dstVertexLengthIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 90;							// number of output triangles
    constexpr int vertCount = trianglesCount + 2;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + trianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLE_STRIP;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = 1;

    nm32f expectedDstVertex[outputCoordCount + trianglesCount]  = {0};
    for (int i = 0; i < outputCoordCount; i++){
        expectedDstVertex[i] = 1;
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP90Triangles_dstVertexIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 90;							// number of output triangles
    constexpr int vertCount = trianglesCount + 2;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + trianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLE_STRIP;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = (float)(4 * i + j);

    nm32f expectedDstVertex[outputCoordCount + trianglesCount]  = {0};
    for (int i = 0; i < trianglesCount; i++){
		expectedDstVertex[ 0 * trianglesCount + i] = srcVertex[i + (i&1)].vec[0];	// Для чётных i: i, для нечётных i: i + 1
		expectedDstVertex[ 1 * trianglesCount + i] = srcVertex[i + (i&1)].vec[1];
		expectedDstVertex[ 2 * trianglesCount + i] = srcVertex[i + (i&1)].vec[2];
		expectedDstVertex[ 3 * trianglesCount + i] = srcVertex[i + (i&1)].vec[3];
		expectedDstVertex[ 4 * trianglesCount + i] = srcVertex[i + ((~i)&1)].vec[0];	// Для чётных i: i + 1, для нечётных i: i
		expectedDstVertex[ 5 * trianglesCount + i] = srcVertex[i + ((~i)&1)].vec[1];
		expectedDstVertex[ 6 * trianglesCount + i] = srcVertex[i + ((~i)&1)].vec[2];
		expectedDstVertex[ 7 * trianglesCount + i] = srcVertex[i + ((~i)&1)].vec[3];
		expectedDstVertex[ 8 * trianglesCount + i] = srcVertex[i + 2].vec[0];
		expectedDstVertex[ 9 * trianglesCount + i] = srcVertex[i + 2].vec[1];
		expectedDstVertex[10 * trianglesCount + i] = srcVertex[i + 2].vec[2];
		expectedDstVertex[11 * trianglesCount + i] = srcVertex[i + 2].vec[3];
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
	
	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP90_dstColorLengthIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount = 90;							// number of output triangles
    constexpr int vertCount = trianglesCount + 2;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};	
    v4nm32f     dstColor[3 * (trianglesCount + 1)] = {0};	// one more triangle filled with zeroes

    int mode = NMGL_TRIANGLE_STRIP;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcColor[i].vec[j] = 1;

    v4nm32f expectedDstColor[3 * (trianglesCount + 1)] = {0};

    for (int i = 0; i < 3 * trianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[i].vec[j] = 1;
		}
	}
	
	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * (trianglesCount + 1));

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP90_dstColorIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount 	= 90;						// number of output triangles
    constexpr int vertCount 		= trianglesCount + 2;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	
    v4nm32f	expectedDstColor[3 * trianglesCount]	= {0};
    int mode = NMGL_TRIANGLE_STRIP;

    for (int i = 0; i < vertCount; i++){
        for (int j = 0; j < 4; j++){
            srcColor[i].vec[j] = (float)(4 * i + j);
		}
	}

    for (int i = 0; i < trianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[3 * i].vec[j] = srcColor[i + (i&1)].vec[j];
            expectedDstColor[3 * i + 1].vec[j] = srcColor[i + ((~i)&1)].vec[j];
            expectedDstColor[3 * i + 2].vec[j] = srcColor[i + 2].vec[j];
		}
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP17Vertexes_returns15 ()
{
	// Arrange
	constexpr int trianglesCount 	= 15;						// number of output triangles
    constexpr int vertCount 		= trianglesCount + 2;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	

    int mode = NMGL_TRIANGLE_STRIP;

	// Act
	int res;
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ASSERT (trianglesCount == res);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP47Vertexes_returns45 ()
{
	// Arrange
	constexpr int trianglesCount 	= 45;						// number of output triangles
    constexpr int vertCount 		= trianglesCount + 2;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	

    int mode = NMGL_TRIANGLE_STRIP;

	// Act
	int res;
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ASSERT (trianglesCount == res);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP92Vertexes_returns90 ()
{
	// Arrange
	constexpr int trianglesCount 	= 90;						// number of output triangles
    constexpr int vertCount 		= trianglesCount + 2;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	

    int mode = NMGL_TRIANGLE_STRIP;

	// Act
	int res;
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ASSERT (trianglesCount == res);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_dstVertexLengthIsCorrect ()
{
	// Arrange
    constexpr int vertCount = 5;
	constexpr int outputCoordCount = 4 * 3 * (vertCount - 2);
    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + 12] = {0} ;	// 12 more
    v4nm32f     dstColor[3 * (vertCount - 2)] = {0};

    int mode = NMGL_TRIANGLE_FAN;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = 1;

    nm32f expectedDstVertex[outputCoordCount + 12]  = {0};
    for (int i = 0; i < outputCoordCount; i++)
        expectedDstVertex[i] = 1;

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + 12);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_dstVertexIsCorrect ()
{
	// Arrange
	v4nm32f     srcVertex[5] = {0};
	v4nm32f     srcColor[5] = {0};
	nm32f       dstVertex[48] = {0} ;
	v4nm32f     dstColor[12] = {0};
	
	int mode = NMGL_TRIANGLE_FAN;
	int vertCount = 5;
	
	srcVertex[0].vec[0] = 1;
	srcVertex[0].vec[1] = 1;
	srcVertex[0].vec[2] = 1;
	srcVertex[0].vec[3] = 0;
	
	srcVertex[1].vec[0] = 2;
	srcVertex[1].vec[1] = 3;
	srcVertex[1].vec[2] = 2;
	srcVertex[1].vec[3] = 1;
	
	srcVertex[2].vec[0] = 2;
	srcVertex[2].vec[1] = 1;
	srcVertex[2].vec[2] = 1;
	srcVertex[2].vec[3] = 0;
	
	srcVertex[3].vec[0] = 3;
	srcVertex[3].vec[1] = 3;
	srcVertex[3].vec[2] = 2;
	srcVertex[3].vec[3] = 1;
	
	srcVertex[4].vec[0] = 3;
	srcVertex[4].vec[1] = 1;
	srcVertex[4].vec[2] = 2;
	srcVertex[4].vec[3] = 0;
	
	nm32f expectedDstVertex[36] =	{
	  	  								1,1,1,
	  									1,1,1,
	  									1,1,1,
	  									0,0,0,
	  									2,2,3,
	  									3,1,3,
	  									2,1,2,
	  									1,0,1,
	  									2,3,3,
	  									1,3,1,
	  									1,2,2,
	  									0,1,0
									};
	
	// Act
	vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
	
	// Assert
	TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, 36);
	
	return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_dstColorLengthIsCorrect ()
{
	// Arrange
    v4nm32f     srcVertex[5] = {0};
    v4nm32f     srcColor[5] = {0};
    nm32f       dstVertex[48] = {0} ;
    v4nm32f     dstColor[12] = {0};
    v4nm32f     expectedDstColor[12] = {0};

    int mode = NMGL_TRIANGLE_FAN;
    int vertCount = 5;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcColor[i].vec[j] = 1;

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 4; j++)
            expectedDstColor[i].vec[j] = 1;
	
	// Act
	vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 12);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_dstColorIsCorrect ()
{
	// Arrange
    v4nm32f     srcColor[5] =	{
									{1,1,1,1},
									{2,2,2,2},
									{3,3,3,3},
									{4,4,4,4},
									{5,5,5,5}
								};
    v4nm32f     srcVertex[5] = {0};
    nm32f       dstVertex[36] = {0} ;
    v4nm32f     dstColor[9] = {0};
    v4nm32f     expectedDstColor[9] =	{
											{1,1,1,1},
											{2,2,2,2},
											{3,3,3,3},
											{1,1,1,1},
											{3,3,3,3},
											{4,4,4,4},
											{1,1,1,1},
											{4,4,4,4},
											{5,5,5,5}
										};

    int mode = NMGL_TRIANGLE_FAN;
    int vertCount = 5;

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 9);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN81Triangles_dstVertexLengthIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 81;							// number of output triangles
    constexpr int vertCount = trianglesCount + 2;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + trianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLE_FAN;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = 1;

    nm32f expectedDstVertex[outputCoordCount + trianglesCount]  = {0};
    for (int i = 0; i < outputCoordCount; i++){
        expectedDstVertex[i] = 1;
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN81Triangles_dstVertexIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 81;							// number of output triangles
    constexpr int vertCount = trianglesCount + 2;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + trianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLE_FAN;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = (float)(4 * i + j);

    nm32f expectedDstVertex[outputCoordCount + trianglesCount]  = {0};
    for (int i = 0; i < trianglesCount; i++){
		expectedDstVertex[ 0 * trianglesCount + i] = srcVertex[0].vec[0];	
		expectedDstVertex[ 1 * trianglesCount + i] = srcVertex[0].vec[1];
		expectedDstVertex[ 2 * trianglesCount + i] = srcVertex[0].vec[2];
		expectedDstVertex[ 3 * trianglesCount + i] = srcVertex[0].vec[3];
		expectedDstVertex[ 4 * trianglesCount + i] = srcVertex[i + 1].vec[0];
		expectedDstVertex[ 5 * trianglesCount + i] = srcVertex[i + 1].vec[1];
		expectedDstVertex[ 6 * trianglesCount + i] = srcVertex[i + 1].vec[2];
		expectedDstVertex[ 7 * trianglesCount + i] = srcVertex[i + 1].vec[3];
		expectedDstVertex[ 8 * trianglesCount + i] = srcVertex[i + 2].vec[0];
		expectedDstVertex[ 9 * trianglesCount + i] = srcVertex[i + 2].vec[1];
		expectedDstVertex[10 * trianglesCount + i] = srcVertex[i + 2].vec[2];
		expectedDstVertex[11 * trianglesCount + i] = srcVertex[i + 2].vec[3];
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
	
	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN81_dstColorLengthIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount = 81;							// number of output triangles
    constexpr int vertCount = trianglesCount + 2;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};	
    v4nm32f     dstColor[3 * (trianglesCount + 1)] = {0};	// one more triangle filled with zeroes

    int mode = NMGL_TRIANGLE_FAN;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcColor[i].vec[j] = 1;

    v4nm32f expectedDstColor[3 * (trianglesCount + 1)] = {0};

    for (int i = 0; i < 3 * trianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[i].vec[j] = 1;
		}
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * (trianglesCount + 1));

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN81_dstColorIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount 	= 81;						// number of output triangles
    constexpr int vertCount 		= trianglesCount + 2;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	
    v4nm32f	expectedDstColor[3 * trianglesCount]	= {0};

    int mode = NMGL_TRIANGLE_FAN;

    for (int i = 0; i < vertCount; i++){
        for (int j = 0; j < 4; j++){
            srcColor[i].vec[j] = (float)(4 * i + j);
		}
	}

    for (int i = 0; i < trianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[3 * i].vec[j] = srcColor[0].vec[j];
            expectedDstColor[3 * i + 1].vec[j] = srcColor[i + 1].vec[j];
            expectedDstColor[3 * i + 2].vec[j] = srcColor[i + 2].vec[j];
		}
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN90Triangles_dstVertexLengthIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 90;							// number of output triangles
    constexpr int vertCount = trianglesCount + 2;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + trianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLE_FAN;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = 1;

    nm32f expectedDstVertex[outputCoordCount + trianglesCount]  = {0};
    for (int i = 0; i < outputCoordCount; i++){
        expectedDstVertex[i] = 1;
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN90Triangles_dstVertexIsCorrect()
{
	// Arrange
	constexpr int trianglesCount = 90;							// number of output triangles
    constexpr int vertCount = trianglesCount + 2;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount + trianglesCount] = {0};	// one more row filled with zeroes
    v4nm32f     dstColor[3 * trianglesCount] = {0};

    int mode = NMGL_TRIANGLE_FAN;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcVertex[i].vec[j] = (float)(4 * i + j);

    nm32f expectedDstVertex[outputCoordCount + trianglesCount]  = {0};
    for (int i = 0; i < trianglesCount; i++){
		expectedDstVertex[ 0 * trianglesCount + i] = srcVertex[0].vec[0];	
		expectedDstVertex[ 1 * trianglesCount + i] = srcVertex[0].vec[1];
		expectedDstVertex[ 2 * trianglesCount + i] = srcVertex[0].vec[2];
		expectedDstVertex[ 3 * trianglesCount + i] = srcVertex[0].vec[3];
		expectedDstVertex[ 4 * trianglesCount + i] = srcVertex[i + 1].vec[0];
		expectedDstVertex[ 5 * trianglesCount + i] = srcVertex[i + 1].vec[1];
		expectedDstVertex[ 6 * trianglesCount + i] = srcVertex[i + 1].vec[2];
		expectedDstVertex[ 7 * trianglesCount + i] = srcVertex[i + 1].vec[3];
		expectedDstVertex[ 8 * trianglesCount + i] = srcVertex[i + 2].vec[0];
		expectedDstVertex[ 9 * trianglesCount + i] = srcVertex[i + 2].vec[1];
		expectedDstVertex[10 * trianglesCount + i] = srcVertex[i + 2].vec[2];
		expectedDstVertex[11 * trianglesCount + i] = srcVertex[i + 2].vec[3];
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
	
	// Assert
    TEST_ARRAYS_EQUAL (dstVertex, expectedDstVertex, outputCoordCount + trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN90_dstColorLengthIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount = 90;							// number of output triangles
    constexpr int vertCount = trianglesCount + 2;				// number of input vertexes
	constexpr int outputCoordCount = 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f     srcVertex[vertCount] = {0};
    v4nm32f     srcColor[vertCount] = {0};
    nm32f       dstVertex[outputCoordCount] = {0};	
    v4nm32f     dstColor[3 * (trianglesCount + 1)] = {0};	// one more triangle filled with zeroes

    int mode = NMGL_TRIANGLE_FAN;

    for (int i = 0; i < vertCount; i++)
        for (int j = 0; j < 4; j++)
            srcColor[i].vec[j] = 1;

    v4nm32f expectedDstColor[3 * (trianglesCount + 1)] = {0};

    for (int i = 0; i < 3 * trianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[i].vec[j] = 1;
		}
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
    
	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * (trianglesCount + 1));

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN90_dstColorIsCorrect ()
{
	// Arrange
	constexpr int trianglesCount 	= 90;						// number of output triangles
    constexpr int vertCount 		= trianglesCount + 2;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	
    v4nm32f	expectedDstColor[3 * trianglesCount]	= {0};

    int mode = NMGL_TRIANGLE_FAN;

    for (int i = 0; i < vertCount; i++){
        for (int j = 0; j < 4; j++){
            srcColor[i].vec[j] = (float)(4 * i + j);
		}
	}

    for (int i = 0; i < trianglesCount; i++){
        for (int j = 0; j < 4; j++){
            expectedDstColor[3 * i].vec[j] = srcColor[0].vec[j];
            expectedDstColor[3 * i + 1].vec[j] = srcColor[i + 1].vec[j];
            expectedDstColor[3 * i + 2].vec[j] = srcColor[i + 2].vec[j];
		}
	}

	// Act
    vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_VEC_ARRAYS_EQUAL (dstColor, expectedDstColor, 3 * trianglesCount);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN15Vertexes_returns13 ()
{
	// Arrange
	constexpr int trianglesCount 	= 13;						// number of output triangles
    constexpr int vertCount 		= trianglesCount + 2;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	

    int mode = NMGL_TRIANGLE_FAN;

	// Act
	int res;
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ASSERT (trianglesCount == res);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN45Vertexes_returns43 ()
{
	// Arrange
	constexpr int trianglesCount 	= 43;						// number of output triangles
    constexpr int vertCount 		= trianglesCount + 2;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	

    int mode = NMGL_TRIANGLE_FAN;

	// Act
	int res;
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ASSERT (trianglesCount == res);

    return 0;
}

int vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN90Vertexes_returns88 ()
{
	// Arrange
	constexpr int trianglesCount 	= 88;						// number of output triangles
    constexpr int vertCount 		= trianglesCount + 2;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	

    int mode = NMGL_TRIANGLE_FAN;

	// Act
	int res;
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);

	// Assert
    TEST_ASSERT (trianglesCount == res);

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

clock_t vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_66Vertices()
{
	// Arrange
	constexpr int trianglesCount 	= 64;						// number of output triangles
    constexpr int vertCount 		= trianglesCount + 2;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	

    int mode = NMGL_TRIANGLE_STRIP;

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

clock_t vertexPrimitiveRepack_modeIsGL_TRIANGLES_STRIP_34Vertices()
{
	// Arrange
	constexpr int trianglesCount 	= 32;						// number of output triangles
    constexpr int vertCount 		= trianglesCount + 2;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	

    int mode = NMGL_TRIANGLE_STRIP;

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

clock_t vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_66Vertices()
{
	// Arrange
	constexpr int trianglesCount 	= 64;						// number of output triangles
    constexpr int vertCount 		= trianglesCount + 2;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	

    int mode = NMGL_TRIANGLE_FAN;

	clock_t t1, t2, dt;

	// Act
	int res;
	t1 = clock();
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
	t2 = clock();
	dt = t2 - t1;

    return dt;
}

clock_t vertexPrimitiveRepack_modeIsGL_TRIANGLES_FAN_34Vertices()
{
	// Arrange
	constexpr int trianglesCount 	= 32;						// number of output triangles
    constexpr int vertCount 		= trianglesCount + 2;		// number of input vertexes
	constexpr int outputCoordCount 	= 4 * 3 * trianglesCount;	// number of output vertexes

    v4nm32f	srcVertex[vertCount] 					= {0};
    v4nm32f	srcColor[vertCount] 					= {0};
    nm32f  	dstVertex[outputCoordCount] 			= {0};	
    v4nm32f	dstColor[3 * trianglesCount] 			= {0};	

    int mode = NMGL_TRIANGLE_FAN;

	clock_t t1, t2, dt;

	// Act
	int res;
	t1 = clock();
    res = vertexPrimitiveRepack(srcVertex, srcColor,dstVertex, dstColor, mode, vertCount);
	t2 = clock();
	dt = t2 - t1;

    return dt;
}
