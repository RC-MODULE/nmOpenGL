#ifndef __TEXTURE_TRIANGLE_H__
#define __TEXTURE_TRIANGLE_H__

#include "nmtype.h"
#include "demo3d_common.h"

/**
\defgroup textureFunctions Функции текстурирования
\brief Функции текстурирования для nmOpenGL.

Функции и структуры данных для реализации текстурирования в составе nmOpenGL.
*/
//! \{

/**
\brief Структура для хранения параметров треугольников
*/
struct Triangles{
    //Поля структуры для коммита 2fbdfcc0 nmOpenGLs
	float* x0; /**< Оконная координата x 0-ой вершины треугольника */
	float* y0; /**< Оконная координата y 0-ой вершины треугольника */
	float* x1; /**< Оконная координата x 1-ой вершины треугольника */
	float* y1; /**< Оконная координата y 1-ой вершины треугольника */
	float* x2; /**< Оконная координата x 2-ой вершины треугольника */
	float* y2; /**< Оконная координата y 2-ой вершины треугольника */
	float* z;  /**< Координата z треугольника (одна на три вершины)*/ //TODO: Needed float z in camera space, so float value
	v4nm32s* colors; /**< Цвет треугольника (один на три вершины) */
	int size;   /**< Количество треугольников */
	int maxSize; /**< Максимальное обрабатываемое количество треугольников */
    
    //TODO: необходимо добавить в структуру Triangles (include\demo3d_nm0.h)
	float* s0; /**< Текстурная координата s 0-ой вершины треугольника */
	float* t0; /**< Текстурная координата t 0-ой вершины треугольника */
	float* s1; /**< Текстурная координата s 1-ой вершины треугольника */
	float* t1; /**< Текстурная координата t 1-ой вершины треугольника */
	float* s2; /**< Текстурная координата s 2-ой вершины треугольника */
	float* t2; /**< Текстурная координата t 2-ой вершины треугольника */
};

/** 
\brief Функция для вычисления значения edge-функции.

Функции edgeFunction вычисляет значение edge-функции для заданных точек.

\param triangles [in] Входной массив параметров треугольников
\param x0 [in] Координата x точки 0
\param y0 [in] Координата x точки 0
\param x1 [in] Координата x точки 1
\param y1 [in] Координата x точки 1
\param x2 [in] Координата x точки 2
\param y2 [in] Координата x точки 2
\param res [out] Результат вычисления функции
\retval void
*/
void edgeFunction(float x0, float y0, float x1, float y1, float x2, float y2, float* res);

/** 
\brief Функция текстурирования треугольника/ов.

Функции texTriangle выполняет текстурирования заданного количества треугольников в соответствии с заданными параметрами текстурирования.

\param patterns [in] Входной массив масок треугольников
\param triangles [in] Входной массив параметров треугольников
\param windows [in] Входной массив параметров размещения треугольников в сегменте
\param pSrcTriangle [in] Входной массив цветов треугольников
\param pDstTriangle [out] Выходной массив цветов треугольников
\param count [in] Число треугольников
\retval void
*/
void textureTriangle(Pattern* patterns, 
                 Triangles* triangles,
                 Rectangle* windows, 
                 nm32s* pSrcTriangle, 
                 nm32s* pDstTriangle, 
                 int count);




//! \}

#endif