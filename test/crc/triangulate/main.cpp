#include <vector>
#include <iostream>
#include <cerrno>
#include <cstdio>

#include "nmtype.h"
#include "service.h"
#include "tests.h"

int triangulate_oneTriangleThreeDivisions_returnsSix();
int triangulate_TwoTrianglesLowOutputSize_treatedCountIsOne();
int triangulate_TwoTriangles_treatedCountIsTwo();
int triangulate_oneTriangleThreeDivisions_outputVertexesAreCorrect();
int triangulate_oneTriangleThreeDivisions_outputColorsAreCorrect();
int triangulate_inputVertexPointerIsNull_errorIsEINVAL();
int triangulate_inputColorPointerIsNull_errorIsEINVAL();
int triangulate_outputVertexPointerIsNull_errorIsEINVAL();
int triangulate_outputColorPointerIsNull_errorIsEINVAL();
int triangulate_srcTreatedCountPointerIsNull_errorIsEINVAL();


int main(int argc, char **argv)
{
	puts("triangulate tests: ");

	RUN_TEST(triangulate_oneTriangleThreeDivisions_returnsSix);
	RUN_TEST(triangulate_TwoTrianglesLowOutputSize_treatedCountIsOne);
	RUN_TEST(triangulate_TwoTriangles_treatedCountIsTwo);
	RUN_TEST(triangulate_oneTriangleThreeDivisions_outputVertexesAreCorrect);
	RUN_TEST(triangulate_oneTriangleThreeDivisions_outputColorsAreCorrect);

	RUN_TEST(triangulate_inputVertexPointerIsNull_errorIsEINVAL);
	RUN_TEST(triangulate_inputColorPointerIsNull_errorIsEINVAL);
	RUN_TEST(triangulate_outputVertexPointerIsNull_errorIsEINVAL);
	RUN_TEST(triangulate_outputColorPointerIsNull_errorIsEINVAL);
	RUN_TEST(triangulate_srcTreatedCountPointerIsNull_errorIsEINVAL);

	puts("OK");
	return 0;
}

int triangulate_oneTriangleThreeDivisions_returnsSix()
{
	// Arrange
	nm32f srcVertex[] = {
							1,
							1,
							4,
							2,
							3,
							7,
							3,
							1,
							9
						};
	v4nm32f srcColor[3] = {0};
	int srcCount = 1;
	int maxWidth = 1;
	int maxHeight = 1;
	int maxDstSize = 10;
	nm32f *dstVertex = (nm32f *)calloc(maxDstSize * 9, sizeof(nm32f));
	v4nm32f *dstColor = (v4nm32f *) calloc(maxDstSize * 3, sizeof(v4nm32f));
	int srcTreatedCount;
	int res;

	//Act
	res = triangulate(srcVertex, srcColor, srcCount, maxWidth, maxHeight, maxDstSize, dstVertex, dstColor, &srcTreatedCount);
	(void) res;

	//Assert
	TEST_ASSERT(res == 6);

	return 0;
}

int triangulate_TwoTrianglesLowOutputSize_treatedCountIsOne()
{
	// Arrange
	nm32f srcVertex[] =	{
							1,1,
							1,1,
							0,0,
							2,3,
							3,3,
							0,0,
							3,3,
							1,1,
							0,0 
						};
	v4nm32f srcColor[6] = {0};
	int srcCount = 2;
	int maxWidth = 1;
	int maxHeight = 1;
	int maxDstSize = 6;
	nm32f *dstVertex = (nm32f *)calloc(maxDstSize * 9, sizeof(nm32f));
	v4nm32f *dstColor = (v4nm32f *)calloc(maxDstSize * 3, sizeof(v4nm32f));
	int srcTreatedCount;
	int res;

	//Act
	res = triangulate(srcVertex, srcColor, srcCount, maxWidth, maxHeight, maxDstSize, dstVertex, dstColor, &srcTreatedCount);
	(void) res;

	//Assert
	TEST_ASSERT(srcTreatedCount == 1);

	return 0;
}

int triangulate_TwoTriangles_treatedCountIsTwo()
{
	// Arrange
	nm32f srcVertex[] =	{	1,1,
							1,1,
							0,0,
							2,3,
							3,3,
							0,0,
							3,3,
							1,1,
							0,0 
						};
	v4nm32f srcColor[6] = {0};
	int srcCount = 2;
	int maxWidth = 1;
	int maxHeight = 1;
	int maxDstSize = 30;
	nm32f *dstVertex = (nm32f *)calloc(maxDstSize * 9, sizeof(nm32f));
	v4nm32f *dstColor = (v4nm32f *)calloc(maxDstSize * 3, sizeof(v4nm32f));
	int srcTreatedCount;
	int res;

	//Act
	res = triangulate(srcVertex, srcColor, srcCount, maxWidth, maxHeight, maxDstSize, dstVertex, dstColor, &srcTreatedCount);
	(void) res;

	//Assert
	TEST_ASSERT(srcTreatedCount == srcCount);

	return 0;
}

int triangulate_oneTriangleThreeDivisions_outputVertexesAreCorrect()
{
	// Arrange
	nm32f srcVertex[] =	{ 
							1,
							1,
							4,
							2,
							3,
							7,
							3,
							1,
							9
						};
	v4nm32f srcColor[3] = {0};
	int srcCount = 1;
	int maxWidth = 1;
	int maxHeight = 1;
	int maxDstSize = 6;
	nm32f *dstVertex = (nm32f *) calloc(maxDstSize * 9, sizeof(nm32f));
	nm32f expectedDstVertex[] = { 
									1.50, 3.00, 2.00, 1.50, 3.00, 1.00,
									2.00, 1.00,	3.00, 2.00, 1.00, 1.00,
									5.50, 9.00, 7.00, 5.50, 9.00, 4.00,
									2.50, 2.50, 1.50, 2.00, 2.00, 1.50,
									2.00, 2.00, 2.00, 1.00, 1.00, 2.00,
									8.00, 8.00, 5.50, 6.50, 6.50, 5.50,
									2.25, 2.25, 2.50, 2.25, 2.25, 2.00,
									1.50, 1.50, 2.00, 1.50, 1.50, 1.00,
									7.25, 7.25, 8.00, 7.25, 7.25, 6.50
								};
	v4nm32f *dstColor = (v4nm32f *) calloc(maxDstSize * 3, sizeof(v4nm32f));
	int srcTreatedCount;
	int res;

	//Act
	res = triangulate(srcVertex, srcColor, srcCount, maxWidth, maxHeight, maxDstSize, dstVertex, dstColor, &srcTreatedCount);
	(void) res;

	//Assert
	TEST_ARRAYS_EQUAL(dstVertex, expectedDstVertex, 54);

	return 0;
}

int triangulate_oneTriangleThreeDivisions_outputColorsAreCorrect()
{
	// Arrange
	int srcCount = 1;
	nm32f srcVertex[] = {
							1,
							1,
							4,
							2,
							3,
							7,
							3,
							1,
							9
						};
	v4nm32f srcColor[3] = 	{ 
								{124, 124, 124, 10}, 
								{68, 68, 68, 50}, 
								{70, 70, 70, 80} 
							};
	int maxWidth = 1;
	int maxHeight = 1;
	int maxDstSize = 6;
	nm32f *dstVertex = (nm32f *)calloc(maxDstSize * 9, sizeof(nm32f));
	v4nm32f *dstColor = (v4nm32f *)calloc(maxDstSize * 3, sizeof(v4nm32f));
	v4nm32f expectedDstColor[18] =	{
										{96, 96, 96, 30},
										{69, 69, 69, 65},
										{83, 83, 83, 55},
										{70, 70, 70, 80},
										{69, 69, 69, 65},
										{83, 83, 83, 55},
										{68, 68, 68, 50},
										{96, 96, 96, 30},
										{69, 69, 69, 65},
										{96, 96, 96, 30},
										{97, 97, 97, 45},
										{83, 83, 83, 55},
										{70, 70, 70, 80},
										{97, 97, 97, 45},
										{83, 83, 83, 55},
										{124, 124, 124, 10},
										{96, 96, 96, 30},
										{97, 97, 97, 45}
									};
	int srcTreatedCount;
	int res;

	//Act
	res = triangulate(srcVertex, srcColor, srcCount, maxWidth, maxHeight, maxDstSize, dstVertex, dstColor, &srcTreatedCount);
	(void) res;

	//Assert
	TEST_VEC_ARRAYS_EQUAL(dstColor, expectedDstColor, 18);

	return 0;
}

int triangulate_inputVertexPointerIsNull_errorIsEINVAL()
{
	// Arrange
	errno = 0;
	nm32f *srcVertex = NULL;
	v4nm32f srcColor[3] =	{
								{124, 124, 124, 10},
								{68, 68, 68, 50},
								{70, 70, 70, 80}
							};
	int srcCount = 1;
	int maxWidth = 1;
	int maxHeight = 1;
	int maxDstSize = 6;
	nm32f *dstVertex = (nm32f *)calloc(maxDstSize * 9, sizeof(nm32f));
	v4nm32f *dstColor = (v4nm32f *)calloc(maxDstSize * 3, sizeof(v4nm32f));
	int srcTreatedCount;
	int res;

	// Act
	res = triangulate(srcVertex, srcColor, srcCount, maxWidth, maxHeight, maxDstSize, dstVertex, dstColor, &srcTreatedCount);
	(void) res;

	// Assert
	TEST_ASSERT(EINVAL == errno);

	return 0;
}

int triangulate_inputColorPointerIsNull_errorIsEINVAL()
{
	// Arrange
	errno = 0;
	nm32f srcVertex[] = {0};
	v4nm32f *srcColor = NULL;
	int srcCount = 1;
	int maxWidth = 1;
	int maxHeight = 1;
	int maxDstSize = 6;
	nm32f *dstVertex = (nm32f *)calloc(maxDstSize * 9, sizeof(nm32f));
	v4nm32f *dstColor = (v4nm32f *)calloc(maxDstSize * 3, sizeof(v4nm32f));
	int srcTreatedCount;
	int res;

	// Act
	res = triangulate(srcVertex, srcColor, srcCount, maxWidth, maxHeight, maxDstSize, dstVertex, dstColor, &srcTreatedCount);
	(void) res;

	// Assert
	TEST_ASSERT(EINVAL == errno);

	return 0;
}

int triangulate_outputVertexPointerIsNull_errorIsEINVAL()
{
	// Arrange
	errno = 0;
	nm32f srcVertex[] = {0};
	v4nm32f srcColor[] = {0};
	int srcCount = 1;
	int maxWidth = 1;
	int maxHeight = 1;
	int maxDstSize = 6;
	nm32f *dstVertex = NULL;
	v4nm32f *dstColor = (v4nm32f *)calloc(maxDstSize * 3, sizeof(v4nm32f));
	int srcTreatedCount;
	int res;

	// Act
	res = triangulate(srcVertex, srcColor, srcCount, maxWidth, maxHeight, maxDstSize, dstVertex, dstColor, &srcTreatedCount);
	(void) res;

	// Assert
	TEST_ASSERT(EINVAL == errno);

	return 0;
}

int triangulate_outputColorPointerIsNull_errorIsEINVAL()
{
	// Arrange
	errno = 0;
	nm32f srcVertex[] = {0};
	v4nm32f srcColor[] = {0};
	int srcCount = 1;
	int maxWidth = 1;
	int maxHeight = 1;
	int maxDstSize = 6;
	nm32f *dstVertex = (nm32f *)calloc(maxDstSize * 9, sizeof(nm32f));
	v4nm32f *dstColor = NULL;
	int srcTreatedCount;
	int res;

	// Act
	res = triangulate(srcVertex, srcColor, srcCount, maxWidth, maxHeight, maxDstSize, dstVertex, dstColor, &srcTreatedCount);
	(void) res;

	// Assert
	TEST_ASSERT(EINVAL == errno);

	return 0;
}

int triangulate_srcTreatedCountPointerIsNull_errorIsEINVAL()
{
	// Arrange
	errno = 0;
	nm32f srcVertex[] = {0};
	v4nm32f srcColor[] = {0};
	int srcCount = 1;
	int maxWidth = 1;
	int maxHeight = 1;
	int maxDstSize = 6;
	nm32f *dstVertex = (nm32f *)calloc(maxDstSize * 9, sizeof(nm32f));
	v4nm32f *dstColor = (v4nm32f *)calloc(maxDstSize * 3, sizeof(v4nm32f));
	int *srcTreatedCountPtr = NULL;
	int res;

	// Act
	res = triangulate(srcVertex, srcColor, srcCount, maxWidth, maxHeight, maxDstSize, dstVertex, dstColor, srcTreatedCountPtr);
	(void) res;

	// Assert
	TEST_ASSERT(EINVAL == errno);
	
	return 0;
}
