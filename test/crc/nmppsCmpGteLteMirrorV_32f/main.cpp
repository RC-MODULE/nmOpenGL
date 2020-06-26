#include <cstdio>
#include <cerrno>

#include "nmtype.h"
#include "malloc32.h"
#include "nmplv/vSupport.h"
#include "service.h"
#include "tests.h"

// Performance testing
#include "time.h"

#define SIZE 64
#define DELTA 1.0

void setFlagsRange(nm1 *pVec, int start, int n, int1b value);

int nmppsCmpGteLteMirrorV_32f_valuesAreGreaterThanRange_allFlagsAreZero();
int nmppsCmpGteLteMirrorV_32f_valuesAreLessThanRange_allFlagsAreZero();
int nmppsCmpGteLteMirrorV_32f_valuesAreEqualToTheMax_allFlagsAreOnes();
int nmppsCmpGteLteMirrorV_32f_valuesAreEqualToTheMin_allFlagsAreOnes();
int nmppsCmpGteLteMirrorV_32f_valuesAreInsideTheRange_allFlagsAreOnes();
int nmppsCmpGteLteMirrorV_32f_halfOfTheSizeIs16AndNotMultipleOf32_16MostSignificantFlagsOf32WordAreZero();
int nmppsCmpGteLteMirrorV_32f_halfOfTheSizeIs32AndIsMultipleOf32_NextBitsOfFlagsAreOnesAndNotChanged();
int nmppsCmpGteLteMirrorV_32f_halfOfTheSizeIs32AndIsMultipleOf32_NextBitsOfFlagsAreZeroesAndNotChanged();

// Performance tests
clock_t nmppsCmpGteLteMirrorV_32f_64ValuesAreInsideTheRange_allFlagsAreOnes();

int main(int argc, char **argv)
{
	puts("nmppsCmpGteLteMirrorV_32f tests: ");
	RUN_TEST(nmppsCmpGteLteMirrorV_32f_valuesAreGreaterThanRange_allFlagsAreZero);
	RUN_TEST(nmppsCmpGteLteMirrorV_32f_valuesAreLessThanRange_allFlagsAreZero);
	RUN_TEST(nmppsCmpGteLteMirrorV_32f_valuesAreEqualToTheMax_allFlagsAreOnes);
	RUN_TEST(nmppsCmpGteLteMirrorV_32f_valuesAreEqualToTheMin_allFlagsAreOnes);
	RUN_TEST(nmppsCmpGteLteMirrorV_32f_valuesAreInsideTheRange_allFlagsAreOnes);
	RUN_TEST(nmppsCmpGteLteMirrorV_32f_halfOfTheSizeIs32AndIsMultipleOf32_NextBitsOfFlagsAreOnesAndNotChanged);
	RUN_TEST(nmppsCmpGteLteMirrorV_32f_halfOfTheSizeIs32AndIsMultipleOf32_NextBitsOfFlagsAreZeroesAndNotChanged);
	RUN_TEST(nmppsCmpGteLteMirrorV_32f_halfOfTheSizeIs16AndNotMultipleOf32_16MostSignificantFlagsOf32WordAreZero);

	clock_t dt;
	dt = nmppsCmpGteLteMirrorV_32f_64ValuesAreInsideTheRange_allFlagsAreOnes();

	puts("OK");
	return dt;
}
void setFlagsRange(nm1 *pVec, int start, int n, int1b value)
{
	for (int i = start; i < (start + n); ++i) {
		nmppsPut_1(pVec, i, value);
	}
}

int nmppsCmpGteLteMirrorV_32f_valuesAreGreaterThanRange_allFlagsAreZero()
{
		// Arrange
		int size = SIZE;

		nm32f wArray[SIZE] = {0};
		// Range is -1.0 .. 1.0
		for (int i = 0; i < SIZE; ++i) {
			wArray[i] = DELTA;
		}

		nm32f src_greater_than_range[SIZE];
		for (int i = 0; i < SIZE; ++i) {
			src_greater_than_range[i] = DELTA + 0.5;
		}

		nm1 *evenFlags = nmppsMalloc_1(size/2);
		nm1 *oddFlags = nmppsMalloc_1(size/2);
		setFlagsRange(evenFlags, 0, size/2, 1);
		setFlagsRange(oddFlags, 0, size/2, 1);

		nm1 *expectedEvenFlags = nmppsMalloc_1(size/2);
		nm1 *expectedOddFlags = nmppsMalloc_1(size/2);
		setFlagsRange(expectedEvenFlags, 0, size/2, 0);
		setFlagsRange(expectedOddFlags, 0, size/2, 0);

		// Act 
		nmppsCmpGteLteMirrorV_32f(src_greater_than_range, wArray, evenFlags, oddFlags, size);

		// Assert
		nm32u *evenFlags_int = (nm32u *) evenFlags;
		nm32u *expectedEvenFlags_int = (nm32u *) expectedEvenFlags;
		TEST_ARRAYS_EQUALI(evenFlags_int, expectedEvenFlags_int, 1);	
		
		nm32u *oddFlags_int = (nm32u *) oddFlags;
		nm32u *expectedOddFlags_int = (nm32u *) expectedOddFlags;
		TEST_ARRAYS_EQUALI(oddFlags_int, expectedOddFlags_int, 1);	

		nmppsFree(evenFlags);
		nmppsFree(oddFlags);
		nmppsFree(expectedEvenFlags);
		nmppsFree(expectedOddFlags);

		return 0;
}

int nmppsCmpGteLteMirrorV_32f_valuesAreLessThanRange_allFlagsAreZero()
{
		// Arrange
		int size = SIZE;

		nm32f wArray[SIZE] = {0};
		// Range is -1.0 .. 1.0
		for (int i = 0; i < SIZE; ++i) {
			wArray[i] = DELTA;
		}

		nm32f src_less_than_range[SIZE];
		for (int i = 0; i < SIZE; ++i) {
			src_less_than_range[i] = -DELTA - 0.5;
		}

		nm1 *evenFlags = nmppsMalloc_1(size/2);
		nm1 *oddFlags = nmppsMalloc_1(size/2);
		setFlagsRange(evenFlags, 0, size/2, 1);
		setFlagsRange(oddFlags, 0, size/2, 1);

		nm1 *expectedEvenFlags = nmppsMalloc_1(size/2);
		nm1 *expectedOddFlags = nmppsMalloc_1(size/2);
		setFlagsRange(expectedEvenFlags, 0, size/2, 0);
		setFlagsRange(expectedOddFlags, 0, size/2, 0);

		// Act 
		nmppsCmpGteLteMirrorV_32f(src_less_than_range, wArray, evenFlags, oddFlags, size);

		// Assert
		nm32u *evenFlags_int = (nm32u *) evenFlags;
		nm32u *expectedEvenFlags_int = (nm32u *) expectedEvenFlags;
		TEST_ARRAYS_EQUALI(evenFlags_int, expectedEvenFlags_int, 1);	
		
		nm32u *oddFlags_int = (nm32u *) oddFlags;
		nm32u *expectedOddFlags_int = (nm32u *) expectedOddFlags;
		TEST_ARRAYS_EQUALI(oddFlags_int, expectedOddFlags_int, 1);	

		nmppsFree(evenFlags);
		nmppsFree(oddFlags);
		nmppsFree(expectedEvenFlags);
		nmppsFree(expectedOddFlags);

		return 0;
}

int nmppsCmpGteLteMirrorV_32f_valuesAreEqualToTheMax_allFlagsAreOnes()
{
		// Arrange
		int size = SIZE;

		nm32f wArray[SIZE] = {0};
		// Range is -1.0 .. 1.0
		for (int i = 0; i < SIZE; ++i) {
			wArray[i] = DELTA;
		}

		nm32f src_all_are_max_allowed[SIZE];
		for (int i = 0; i < SIZE; ++i) {
			src_all_are_max_allowed[i] = DELTA;
		}

		nm1 *evenFlags = nmppsMalloc_1(size/2);
		nm1 *oddFlags = nmppsMalloc_1(size/2);
		setFlagsRange(evenFlags, 0, size/2, 0);
		setFlagsRange(oddFlags, 0, size/2, 0);

		nm1 *expectedEvenFlags = nmppsMalloc_1(size/2);
		nm1 *expectedOddFlags = nmppsMalloc_1(size/2);
		setFlagsRange(expectedEvenFlags, 0, size/2, 1);
		setFlagsRange(expectedOddFlags, 0, size/2, 1);

		// Act 
		nmppsCmpGteLteMirrorV_32f(src_all_are_max_allowed, wArray, evenFlags, oddFlags, size);

		// Assert
		nm32u *evenFlags_int = (nm32u *) evenFlags;
		nm32u *expectedEvenFlags_int = (nm32u *) expectedEvenFlags;
		TEST_ARRAYS_EQUALI(evenFlags_int, expectedEvenFlags_int, 1);	
		
		nm32u *oddFlags_int = (nm32u *) oddFlags;
		nm32u *expectedOddFlags_int = (nm32u *) expectedOddFlags;
		TEST_ARRAYS_EQUALI(oddFlags_int, expectedOddFlags_int, 1);	

		nmppsFree(evenFlags);
		nmppsFree(oddFlags);
		nmppsFree(expectedEvenFlags);
		nmppsFree(expectedOddFlags);

		return 0;
}

int nmppsCmpGteLteMirrorV_32f_valuesAreEqualToTheMin_allFlagsAreOnes()
{
		// Arrange
		int size = SIZE;

		nm32f wArray[SIZE] = {0};
		// Range is -1.0 .. 1.0
		for (int i = 0; i < SIZE; ++i) {
			wArray[i] = DELTA;
		}

		nm32f src_all_are_min_allowed[SIZE];
		for (int i = 0; i < SIZE; ++i) {
			src_all_are_min_allowed[i] = -DELTA;
		}

		nm1 *evenFlags = nmppsMalloc_1(size/2);
		nm1 *oddFlags = nmppsMalloc_1(size/2);
		setFlagsRange(evenFlags, 0, size/2, 0);
		setFlagsRange(oddFlags, 0, size/2, 0);

		nm1 *expectedEvenFlags = nmppsMalloc_1(size/2);
		nm1 *expectedOddFlags = nmppsMalloc_1(size/2);
		setFlagsRange(expectedEvenFlags, 0, size/2, 1);
		setFlagsRange(expectedOddFlags, 0, size/2, 1);

		// Act 
		nmppsCmpGteLteMirrorV_32f(src_all_are_min_allowed, wArray, evenFlags, oddFlags, size);

		// Assert
		nm32u *evenFlags_int = (nm32u *) evenFlags;
		nm32u *expectedEvenFlags_int = (nm32u *) expectedEvenFlags;
		TEST_ARRAYS_EQUALI(evenFlags_int, expectedEvenFlags_int, 1);	
		
		nm32u *oddFlags_int = (nm32u *) oddFlags;
		nm32u *expectedOddFlags_int = (nm32u *) expectedOddFlags;
		TEST_ARRAYS_EQUALI(oddFlags_int, expectedOddFlags_int, 1);	

		nmppsFree(evenFlags);
		nmppsFree(oddFlags);
		nmppsFree(expectedEvenFlags);
		nmppsFree(expectedOddFlags);

		return 0;
}

int nmppsCmpGteLteMirrorV_32f_valuesAreInsideTheRange_allFlagsAreOnes()
{
		// Arrange
		int size = SIZE;

		nm32f wArray[SIZE] = {0};
		// Range is -1.0 .. 1.0
		for (int i = 0; i < SIZE; ++i) {
			wArray[i] = DELTA;
		}

		nm32f src_all_are_inside_the_range[SIZE];
		for (int i = 0; i < SIZE; ++i) {
			src_all_are_inside_the_range[i] = DELTA - 0.5;
		}

		nm1 *evenFlags = nmppsMalloc_1(size/2);
		nm1 *oddFlags = nmppsMalloc_1(size/2);
		setFlagsRange(evenFlags, 0, size/2, 0);
		setFlagsRange(oddFlags, 0, size/2, 0);

		nm1 *expectedEvenFlags = nmppsMalloc_1(size/2);
		nm1 *expectedOddFlags = nmppsMalloc_1(size/2);
		setFlagsRange(expectedEvenFlags, 0, size/2, 1);
		setFlagsRange(expectedOddFlags, 0, size/2, 1);

		// Act 
		nmppsCmpGteLteMirrorV_32f(src_all_are_inside_the_range, wArray, evenFlags, oddFlags, size);

		// Assert
		nm32u *evenFlags_int = (nm32u *) evenFlags;
		nm32u *expectedEvenFlags_int = (nm32u *) expectedEvenFlags;
		TEST_ARRAYS_EQUALI(evenFlags_int, expectedEvenFlags_int, 1);	
		
		nm32u *oddFlags_int = (nm32u *) oddFlags;
		nm32u *expectedOddFlags_int = (nm32u *) expectedOddFlags;
		TEST_ARRAYS_EQUALI(oddFlags_int, expectedOddFlags_int, 1);	

		nmppsFree(evenFlags);
		nmppsFree(oddFlags);
		nmppsFree(expectedEvenFlags);
		nmppsFree(expectedOddFlags);

		return 0;
}

int nmppsCmpGteLteMirrorV_32f_halfOfTheSizeIs32AndIsMultipleOf32_NextBitsOfFlagsAreOnesAndNotChanged()
{

		// Arrange
		int size = SIZE;

		nm32f wArray[SIZE] = {0};
		// Range is -1.0 .. 1.0
		for (int i = 0; i < SIZE; ++i) {
			wArray[i] = DELTA;
		}

		nm32f src_all_are_inside_the_range[SIZE];
		for (int i = 0; i < SIZE; ++i) {
			src_all_are_inside_the_range[i] = DELTA - 0.5;
		}

		nm1 *evenFlags = nmppsMalloc_1(size);
		nm1 *oddFlags = nmppsMalloc_1(size);
		setFlagsRange(evenFlags, 0, size/2, 0);
		setFlagsRange(evenFlags, size/2, size/2, 1);
		setFlagsRange(oddFlags, 0, size/2, 0);
		setFlagsRange(oddFlags, size/2, size/2, 1);

		nm1 *expectedEvenFlags = nmppsMalloc_1(size);
		nm1 *expectedOddFlags = nmppsMalloc_1(size);
		setFlagsRange(expectedEvenFlags, 0, size/2, 1);
		setFlagsRange(expectedEvenFlags, size/2, size/2, 1);
		setFlagsRange(expectedOddFlags, 0, size/2, 1);
		setFlagsRange(expectedOddFlags, size/2, size/2, 1);

		// Act 
		nmppsCmpGteLteMirrorV_32f(src_all_are_inside_the_range, wArray, evenFlags, oddFlags, size);

		// Assert
		nm32u *evenFlags_int = (nm32u *) evenFlags;
		nm32u *expectedEvenFlags_int = (nm32u *) expectedEvenFlags;
		TEST_ARRAYS_EQUALI(evenFlags_int, expectedEvenFlags_int, 2);	
		
		nm32u *oddFlags_int = (nm32u *) oddFlags;
		nm32u *expectedOddFlags_int = (nm32u *) expectedOddFlags;
		TEST_ARRAYS_EQUALI(oddFlags_int, expectedOddFlags_int, 2);	

		nmppsFree(evenFlags);
		nmppsFree(oddFlags);
		nmppsFree(expectedEvenFlags);
		nmppsFree(expectedOddFlags);

		return 0;
}

int nmppsCmpGteLteMirrorV_32f_halfOfTheSizeIs32AndIsMultipleOf32_NextBitsOfFlagsAreZeroesAndNotChanged()
{

		// Arrange
		int size = SIZE;

		nm32f wArray[SIZE] = {0};
		// Range is -1.0 .. 1.0
		for (int i = 0; i < SIZE; ++i) {
			wArray[i] = DELTA;
		}

		nm32f src_all_are_inside_the_range[SIZE];
		for (int i = 0; i < SIZE; ++i) {
			src_all_are_inside_the_range[i] = DELTA - 0.5;
		}

		nm1 *evenFlags = nmppsMalloc_1(size);
		nm1 *oddFlags = nmppsMalloc_1(size);
		setFlagsRange(evenFlags, 0, size/2, 0);
		setFlagsRange(evenFlags, size/2, size/2, 0);
		setFlagsRange(oddFlags, 0, size/2, 0);
		setFlagsRange(oddFlags, size/2, size/2, 0);

		nm1 *expectedEvenFlags = nmppsMalloc_1(size);
		nm1 *expectedOddFlags = nmppsMalloc_1(size);
		setFlagsRange(expectedEvenFlags, 0, size/2, 1);
		setFlagsRange(expectedEvenFlags, size/2, size/2, 0);
		setFlagsRange(expectedOddFlags, 0, size/2, 1);
		setFlagsRange(expectedOddFlags, size/2, size/2, 0);

		// Act 
		nmppsCmpGteLteMirrorV_32f(src_all_are_inside_the_range, wArray, evenFlags, oddFlags, size);

		// Assert
		nm32u *evenFlags_int = (nm32u *) evenFlags;
		nm32u *expectedEvenFlags_int = (nm32u *) expectedEvenFlags;
		TEST_ARRAYS_EQUALI(evenFlags_int, expectedEvenFlags_int, 2);	
		
		nm32u *oddFlags_int = (nm32u *) oddFlags;
		nm32u *expectedOddFlags_int = (nm32u *) expectedOddFlags;
		TEST_ARRAYS_EQUALI(oddFlags_int, expectedOddFlags_int, 2);	

		nmppsFree(evenFlags);
		nmppsFree(oddFlags);
		nmppsFree(expectedEvenFlags);
		nmppsFree(expectedOddFlags);

		return 0;
}

#undef SIZE
#define SIZE 32

int nmppsCmpGteLteMirrorV_32f_halfOfTheSizeIs16AndNotMultipleOf32_16MostSignificantFlagsOf32WordAreZero()
{

		// Arrange
		int size = SIZE;

		nm32f wArray[SIZE] = {0};
		// Range is -1.0 .. 1.0
		for (int i = 0; i < SIZE; ++i) {
			wArray[i] = DELTA;
		}

		nm32f src_all_are_inside_the_range[SIZE];
		for (int i = 0; i < SIZE; ++i) {
			src_all_are_inside_the_range[i] = DELTA - 0.5;
		}

		nm1 *evenFlags = nmppsMalloc_1(size);
		nm1 *oddFlags = nmppsMalloc_1(size);
		setFlagsRange(evenFlags, 0, size/2, 0);
		setFlagsRange(evenFlags, size/2, size/2, 1);
		setFlagsRange(oddFlags, 0, size/2, 0);
		setFlagsRange(oddFlags, size/2, size/2, 1);

		nm1 *expectedEvenFlags = nmppsMalloc_1(size);
		nm1 *expectedOddFlags = nmppsMalloc_1(size);
		setFlagsRange(expectedEvenFlags, 0, size/2, 1);
		setFlagsRange(expectedEvenFlags, size/2, size/2, 0);
		setFlagsRange(expectedOddFlags, 0, size/2, 1);
		setFlagsRange(expectedOddFlags, size/2, size/2, 0);

		// Act 
		nmppsCmpGteLteMirrorV_32f(src_all_are_inside_the_range, wArray, evenFlags, oddFlags, size);

		// Assert
		nm32u *evenFlags_int = (nm32u *) evenFlags;
		nm32u *expectedEvenFlags_int = (nm32u *) expectedEvenFlags;
		TEST_ARRAYS_EQUALI(evenFlags_int, expectedEvenFlags_int, 1);	
		
		nm32u *oddFlags_int = (nm32u *) oddFlags;
		nm32u *expectedOddFlags_int = (nm32u *) expectedOddFlags;
		TEST_ARRAYS_EQUALI(oddFlags_int, expectedOddFlags_int, 1);	

		nmppsFree(evenFlags);
		nmppsFree(oddFlags);
		nmppsFree(expectedEvenFlags);
		nmppsFree(expectedOddFlags);

		return 0;
}

clock_t nmppsCmpGteLteMirrorV_32f_64ValuesAreInsideTheRange_allFlagsAreOnes()
{
		// Arrange
		constexpr int size = 64;

		nm32f wArray[size] = {0};
		// Range is -1.0 .. 1.0
		for (int i = 0; i < size; ++i) {
			wArray[i] = DELTA;
		}

		nm32f src_all_are_inside_the_range[size];
		for (int i = 0; i < size; ++i) {
			src_all_are_inside_the_range[i] = DELTA - 0.5;
		}

		nm1 *evenFlags = nmppsMalloc_1(size/2);
		nm1 *oddFlags = nmppsMalloc_1(size/2);
		setFlagsRange(evenFlags, 0, size/2, 0);
		setFlagsRange(oddFlags, 0, size/2, 0);

		clock_t t1, t2, dt;

		// Act 
		t1 = clock();
		nmppsCmpGteLteMirrorV_32f(src_all_are_inside_the_range, wArray, evenFlags, oddFlags, size);
		t2 = clock();
		dt = t2 - t1;

		// Assert
		nmppsFree(evenFlags);
		nmppsFree(oddFlags);

		return dt;
}

