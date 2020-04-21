#ifndef TESTS_H_
#define TESTS_H_

#include <float.h>
#include <math.h>

/* Compare two floats, 1 if are equal, 0 - otherwise */
static int equalf(float a, float b)
{
	if (((a < 0) && (b>=0)) || ((a >= 0) && (b < 0)))
		return 0;

	a = fabs(a);
	b = fabs(b);

	float max = (a > b)? a: b;
	if ( fabs(a-b) <= (FLT_EPSILON * max)){
		return 1;
	} else {
		return 0;
	}
}

#define TEST_ASSERT(x) \
if (!(x)) \
{\
	printf ("\nFAIL Function: %s\n     File:%s\n     Line:%d\n     Cond:%s\n\n",__FUNCTION__, __FILE__, __LINE__, #x);\
	return -1;\
}

#define TEST_ASSERT_ACCUM(x) \
if (!x) \
	_accum++;\


#define TEST_ARRAYS_EQUAL(array1, array2, size) \
{ \
	int __i; \
	for (__i = 0; __i < size; ++__i){ \
		if (!equalf(array1[__i], array2[__i])) { \
			printf ("\nFAIL Function: %s\n     File:%s\n     Line:%d\n     Cond:%s\n\n",__FUNCTION__, __FILE__, __LINE__, "all elements of arrays are equal"); \
			return -1; \
		} else { \
			/* Do nothing, continue to the next element */ \
		} \
	} \
}

// Compare if two integer arrays are equal
#define TEST_ARRAYS_EQUALI(array1, array2, size) \
{ \
	int __i; \
	for (__i = 0; __i < size; ++__i){ \
		if (array1[__i] != array2[__i]) { \
			printf ("\nFAIL Function: %s\n     File:%s\n     Line:%d\n     Cond:%s\n\n",__FUNCTION__, __FILE__, __LINE__, "all elements of arrays are equal"); \
			return -1; \
		} else { \
			/* Do nothing, continue to the next element */ \
		} \
	} \
}
#define TEST_VEC_ARRAYS_EQUAL(array1, array2, size) \
{ \
	volatile int __i; \
	for (__i = 0; __i < size; ++__i){ \
		if ((!equalf(array1[__i].vec[0], array2[__i].vec[0])) || \
			(!equalf(array1[__i].vec[1], array2[__i].vec[1])) || \
			(!equalf(array1[__i].vec[2], array2[__i].vec[2])) || \
			(!equalf(array1[__i].vec[3], array2[__i].vec[3]))) { \
			printf ("\nFAIL Function: %s\n     File:%s\n     Line:%d\n     Cond:%s\n\n",__FUNCTION__, __FILE__, __LINE__, "all elements of arrays are equal"); \
			return -1; \
		} else { \
			/* Do nothing, continue to the next element */ \
		} \
	} \
}

// Compares arrays of struct {unsigned long long vec[1]}
// Implemented to compare RGB8888 arrays
#define TEST_VEC1_ARRAYS_EQUALI(array1, array2, size) \
{ \
	int __i; \
	for (__i = 0; __i < size; ++__i){ \
		if ((array1[__i].vec[0]) != (array2[__i].vec[0])) { \
			printf ("\nFAIL Function: %s\n     File:%s\n     Line:%d\n     Cond:%s\n\n",__FUNCTION__, __FILE__, __LINE__, "all elements of arrays are equal"); \
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

#endif /* TESTS_H_ */
