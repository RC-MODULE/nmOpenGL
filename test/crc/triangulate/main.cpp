#include <vector>
#include <iostream>
#include <cerrno>
#include <cstdio>

#include "nmtype.h"
#include "service.h"
#include "tests.h"

// Performance testing
#include "time.h"

int triangulate_oneTriangleThreeDivisions_returnsSix();
int triangulate_TwoTrianglesLowOutputSize_treatedCountIsOne();
int triangulate_TwoTriangles_treatedCountIsTwo();
int triangulate_oneTriangleThreeDivisions_outputVertexesAreCorrect();
int triangulate_oneTriangleThreeDivisions_outputColorsAreCorrect();

// Performance tests
clock_t triangulate_oneTriangleOneDivision_returnsTwo();

int main(int argc, char **argv)
{
	puts("triangulate tests: ");

	RUN_TEST(triangulate_oneTriangleThreeDivisions_returnsSix);
	RUN_TEST(triangulate_TwoTrianglesLowOutputSize_treatedCountIsOne);
	RUN_TEST(triangulate_TwoTriangles_treatedCountIsTwo);
	RUN_TEST(triangulate_oneTriangleThreeDivisions_outputVertexesAreCorrect);
	RUN_TEST(triangulate_oneTriangleThreeDivisions_outputColorsAreCorrect);

	clock_t dt;
	dt = triangulate_oneTriangleOneDivision_returnsTwo();

	puts("OK");
	return dt;
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
									2.50, 2.50, 1.50, 1.50, 2.00, 2.00,
									2.00, 2.00,	2.00, 2.00, 1.00, 1.00,
									8.00, 8.00, 5.50, 5.50, 6.50, 6.50,
									2.25, 3.00, 2.00, 2.00, 2.25, 1.50,
									1.50, 1.00, 3.00, 1.00, 1.50, 2.00,
									7.25, 9.00, 7.00, 6.50, 7.25, 5.50,
									1.50, 2.25, 2.50, 1.00, 3.00, 2.25,
									2.00, 1.50, 2.00, 1.00, 1.00, 1.50,
									5.50, 7.25, 8.00, 4.00, 9.00, 7.25
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
										{69, 69, 69, 65},
										{83, 83, 83, 55},
										{96, 96, 96, 30},
										{69, 69, 69, 65},
										{70, 70, 70, 80},
										{83, 83, 83, 55},
										{96, 96, 96, 30},
										{68, 68, 68, 50},
										{69, 69, 69, 65},
										{96, 96, 96, 30},
										{97, 97, 97, 45},
										{124, 124, 124, 10},
										{97, 97, 97, 45},
										{83, 83, 83, 55},
										{70, 70, 70, 80},
										{97, 97, 97, 45},
										{96, 96, 96, 30},
										{83, 83, 83, 55}
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

clock_t triangulate_oneTriangleOneDivision_returnsTwo()
{
	// Arrange
	nm32f srcVertex[] = {
							1,
							1,
							4,
							2,
							2,
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

	clock_t t1, t2, dt;
	//Act
	t1 = clock();
	res = triangulate(srcVertex, srcColor, srcCount, maxWidth, maxHeight, maxDstSize, dstVertex, dstColor, &srcTreatedCount);
	t2 = clock();
	dt = t2 - t1;
	(void) res;

	return dt;
}

