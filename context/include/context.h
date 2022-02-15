#include "nmtype.h"
#include "utility.h"

typedef union 
{
    float* f;
    int* i;
    unsigned int* ui;
    v4nm32f* v4f;
    v3nm32f* v3f;
    v2nm32f* v2f;  
} PoolPointer;


/*!
 *  \brief Структура, хранящая указатели на геометрические координаты, текстурные координаты и цвет вершины
 *  \author Жиленков Иван
 */
typedef struct {
	float *x;
	float *y;
	float *z;
	float *w;
	// TEXTURING PART
	float *s;
	float *t;
	// TEXTURING PART
	v4nm32f* color;
	int dummy;
}  CombinePointers;


/*!
 *  \brief Структура, хранящая три вершины треугольника. Каждая вершина описывается структурой CombinePointers
 *  \author Жиленков Иван
 */
typedef struct {
	CombinePointers v0;
	CombinePointers v1;
	CombinePointers v2;
} TrianglePointers ;

/*!
 *  \brief Структура, хранящая две вершины линии. Каждая вершина описывается структурой CombinePointers
 *  \author Жиленков Иван
 */
typedef struct {
	CombinePointers v0;
	CombinePointers v1;
}  LinePointers;


