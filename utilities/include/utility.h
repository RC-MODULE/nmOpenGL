#pragma once
#include "nmtype.h"
#include "debugprint.h"

struct Vector2 {
	int x;
	int y;
};

struct Size {
	int width;
	int height;
};

struct Rectangle {
	int x;
	int y;
	int width;
	int height;
};

typedef struct s_mat4nm32f {
	float matr[16];
} mat4nm32f;

typedef struct s_v3nm32f {
	float v0;
	float v1;
	float v2;
} v3nm32f;

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

	int cnvMaskToIndices(nm1* mask, int* indices, int size);

	/*!
	 *  \ingroup service_api
	 *  \brief Функции выборки элементов из массивов
	 *  \author Жиленков Иван
	 *  
	 *  \param srcPointers [in] Указатели входных массивов
	 *  \param indices [in] Массив индексов
	 *  \param dstPointers [in] Указатели выходных массивов
	 *  \param nArrays [in] Число массивов
	 *  \param size [in] Число копируемых элементов
	 *  
	 *  \details Функция производит выборку нужных элементов из входных массивов и кладет их подряд в выходных массивых. Индексы нужных элементов хранятся в массиве indices
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcPointers"> </param>
	 *          <param name="indices"> </param>
	 *          <param name="dstPointers"> </param>
	 *          <param name="nArrays"> </param>
	 *          <param name="size"> </param>
	 *          <size> </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	void copyArraysByIndices(void** srcPointers, int* indices, void** dstPointers, int nArrays, int size);
	
	/*!
	 *  \brief Функция выборки цветов
	 *  \author Жиленков Иван
	 *  
	 *  \param srcColor [in] Входной массив цветных векторов
	 *  \param indices [in] Массив индексов
	 *  \param dstColor [in] Выходной массив цветных векторов
	 *  \param size [in] Число цветных векторов
	 *  
	 *  \details Функция берет элементы цветных векторов с индексами, получаемыми из *indices* и кладет их последовательно в выходной массив
	 *  
	 */
	void copyColorByIndices(v4nm32s* srcColor, int* indices, v4nm32s* dstColor, int size);

#ifdef __cplusplus
}
#endif //__cplusplus