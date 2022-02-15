#ifndef TESTS_H_
#define TESTS_H_

#include <float.h>
#include <math.h>

/* Compare two floats, 1 if are equal, 0 - otherwise */
static int equalf(float a, float b, float epsilon)
{
	if (((a < 0) && (b>=0)) || ((a >= 0) && (b < 0)))
		return 0;

	a = fabs(a);
	b = fabs(b);

	float max = (a > b)? a: b;

	if ((fabs(a - b) <= (epsilon * max))	||
		(fabs(a - b) <= epsilon)){
		return 1;
	} else {
		puts("\n\n");
		printf("fabs(a - b)(%15.12f) <= (epsilon * max)(%15.12f): %i\n", 
				fabs(a - b), epsilon * max, fabs(a-b) <= (epsilon * max));
		printf("fabs(a - b)(%15.12f) <= epsilon(%15.12f): %i\n", 
				fabs(a - b), epsilon, fabs(a-b) <= epsilon);
		return 0;
	}
	
}

#define TEST_ASSERT(x, ...) \
if (!(x)) \
{\
	printf ("\nFAIL Function: %s\n     File:%s\n     Line:%d\n     Cond:%s\n",__FUNCTION__, __FILE__, __LINE__, #x);\
	printf ("     Details: " __VA_ARGS__);\
	printf ("\n\n");\
	return -1;\
}

#define TEST_ASSERT_EQUALF_PRECISION(real, expected, epsilon, ...) \
{ \
	if (!equalf(real, expected, epsilon)){ \
		printf ("\nFAIL Function: %s\n     File:%s\n     Line:%d\n     Cond:%s\n",__FUNCTION__, __FILE__, __LINE__, "real is not equal to expected"); \
		printf ("     Details: " __VA_ARGS__);\
		printf ("\n\n");\
		return -1; \
	} else { \
		/* Do nothing, continue to the next element */ \
	} \
}

#define TEST_ASSERT_EQUALF(real, expected, ...) \
	TEST_ASSERT_EQUALF_PRECISION(real, expected, FLT_EPSILON, __VA_ARGS__)

#define TEST_ASSERT_ACCUM(x) \
if (!(x)) \
	_accum++;\

#define TEST_ARRAYS_EQUAL_PRECISION(array1, array2, size, epsilon, ...) \
{ \
	int __i; \
	for (__i = 0; __i < size; ++__i){ \
		if (!equalf(array1[__i], array2[__i], epsilon)) { \
			printf ("\nFAIL Function: %s\n     File:%s\n     Line:%d\n     Cond:%s\n",__FUNCTION__, __FILE__, __LINE__, "all elements of arrays are equal"); \
			printf ("     Details: " __VA_ARGS__);\
			printf ("\n\n");\
			return -1; \
		} else { \
			/* Do nothing, continue to the next element */ \
		} \
	} \
}

#define TEST_ARRAYS_EQUAL(array1, array2, size, ...) \
	TEST_ARRAYS_EQUAL_PRECISION(array1, array2, size, FLT_EPSILON, __VA_ARGS__)

#define TEST_ARRAYS_NOT_EQUAL_PRECISION(array1, array2, size, epsilon, ...) \
{ \
	int __i; \
	int __res = -1; \
	for (__i = 0; __i < size; ++__i){ \
		if (!equalf(array1[__i], array2[__i], epsilon)) { \
			__res = 0;\
			break;\
		}\
	}\
	if (-1 == __res) { \
		printf ("\nFAIL Function: %s\n     File:%s\n     Line:%d\n     Cond:%s\n",__FUNCTION__, __FILE__, __LINE__, "Arrays are not equal"); \
		printf ("     Details: " __VA_ARGS__);\
		printf ("\n\n");\
		return -1; \
	} else { \
		/* Do nothing, continue to the next element */ \
	} \
}

#define TEST_ARRAYS_NOT_EQUAL(array1, array2, size, ...) \
	TEST_ARRAYS_NOT_EQUAL_PRECISION(array1, array2, size, FLT_EPSILON, __VA_ARGS__)

// Compare if two integer arrays are equal
#define TEST_ARRAYS_EQUALI(array1, array2, size, ...) \
{ \
	int __i; \
	for (__i = 0; __i < size; ++__i){ \
		if (array1[__i] != array2[__i]) { \
			printf ("\nFAIL Function: %s\n     File:%s\n     Line:%d\n     Cond:%s\n",__FUNCTION__, __FILE__, __LINE__, "all elements of arrays are equal"); \
			printf ("     Details: " __VA_ARGS__);\
			printf ("\n\n");\
			return -1; \
		} else { \
			/* Do nothing, continue to the next element */ \
		} \
	} \
}

#define TEST_VEC_ARRAYS_EQUAL_PRECISION(array1, array2, size, epsilon, ...) \
{ \
	volatile int __i; \
	for (__i = 0; __i < size; ++__i){ \
		if ((!equalf(array1[__i].vec[0], array2[__i].vec[0], epsilon)) || \
			(!equalf(array1[__i].vec[1], array2[__i].vec[1], epsilon)) || \
			(!equalf(array1[__i].vec[2], array2[__i].vec[2], epsilon)) || \
			(!equalf(array1[__i].vec[3], array2[__i].vec[3], epsilon))) { \
			printf ("\nFAIL Function: %s\n     File:%s\n     Line:%d\n     Cond:%s\n",__FUNCTION__, __FILE__, __LINE__, "all elements of arrays are equal"); \
			printf ("     Details: " __VA_ARGS__);\
			printf ("\n\n");\
			return -1; \
		} else { \
			/* Do nothing, continue to the next element */ \
		} \
	} \
}

#define TEST_VEC_ARRAYS_EQUAL(array1, array2, size, ...) \
	TEST_VEC_ARRAYS_EQUAL_PRECISION(array1, array2, size, FLT_EPSILON, __VA_ARGS__)

// Compares arrays of struct {unsigned long long vec[1]}
// Implemented to compare RGB8888 arrays
#define TEST_VEC1_ARRAYS_EQUALI(array1, array2, size, ...) \
{ \
	int __i; \
	for (__i = 0; __i < size; ++__i){ \
		if ((array1[__i].vec[0]) != (array2[__i].vec[0])) { \
			printf ("\nFAIL Function: %s\n     File:%s\n     Line:%d\n     Cond:%s\n",__FUNCTION__, __FILE__, __LINE__, "all elements of arrays are equal"); \
			printf ("     Details: " __VA_ARGS__);\
			printf ("\n\n");\
			return -1; \
		} else { \
			/* Do nothing, continue to the next element */ \
		} \
	} \
}

#ifdef DEBUG 
#define DEBUG_PRINT(x) printf x  
#else 
#define DEBUG_PRINT(x) do {} while (0)
#endif
//usage: DEBUG_PRINT(("var1: %d; var2: %d; str: %s\n", var1, var2, str));

//Macro to run test. usage: RUN_TEST(functionMane_condition_result)
#define RUN_TEST(x) if(x() == 0) printf ("OK   Function: %s\n", #x);

//Macro to run performance test. usage: RUN_PERF_TEST(functionMane_condition_result)
#define RUN_PERF_TEST(x) \
{\
	printf ("\nOK   Function: %s\n", #x);\
	x();\
}

//Macro to run test with arguments. usage: RUN_ARG_TEST(functionMane_condition_result,arg)
#define RUN_ARG_TEST(f, arg) \
	if(f(arg) == 0) {\
		printf ("OK   Function: %s(%i)\n", #f, arg);\
	}

//Macro to run test with two arguments. usage: RUN_TEST_2ARGS(functionMane_condition_result,arg1, arg2)
#define RUN_TEST_2ARGS(f, arg1, arg2) \
	if(f(arg1, arg2) == 0) {\
		printf ("OK   Function: %s(%i, %i)\n", #f, arg1, arg2);\
	}

#endif /* TESTS_H_ */
