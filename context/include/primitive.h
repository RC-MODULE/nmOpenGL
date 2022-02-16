#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

/*!
 *  \brief Структура, хранящая указатели на геометрические координаты, текстурные координаты и цвет вершины
 *  \author Жиленков Иван
 */
struct CombinePointers {
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
};


/*!
 *  \brief Структура, хранящая три вершины треугольника. Каждая вершина описывается структурой CombinePointers
 *  \author Жиленков Иван
 */
struct TrianglePointers {
	CombinePointers v0;
	CombinePointers v1;
	CombinePointers v2;
};

/*!
 *  \brief Структура, хранящая две вершины линии. Каждая вершина описывается структурой CombinePointers
 *  \author Жиленков Иван
 */
struct LinePointers {
	CombinePointers v0;
	CombinePointers v1;
};

#endif //__PRIMITIVE_H__
