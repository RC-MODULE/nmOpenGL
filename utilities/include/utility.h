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


	/*!
	 *  \brief Находит индекс первого ненулевого элемента
	 *  \author Жиленков Иван
	 *  
	 *  \param pSrcVec [in] Входной массив элемент
	 *  \param size [in] Число элементов
	 *  \return Индекс первого ненулевого элемента. Если не находит, то будет равно -1
	 *  
	 */
	int firstNonZeroIndx_32s(int* pSrcVec, int size);

	/*!
	 *  \ingroup service_api
	 *  \brief Функция чтение однобитного массива масок
	 *  \author Жиленков Иван
	 *  
	 *  \param mask [in] Входной массив масок
	 *  \param dstIndices [in] Выходной массив индексов
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция читает входные данные как однобитный массив проверяя значение каждого бита. Если бит маски равен 1, то его порядковый номер 
	 *  записывается в *dstIndices*, иначе он пропускается
	 *  
	 */
	int readMask(nm1* mask, int* dstIndices, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция чтение однобитного массива масок, разделенного на четные и нечетные маски
	 *  \author Жиленков Иван
	 *  
	 *  \param maskEven [in] Входной массив масок четных элементов
	 *  \param maskOdd [in] Входной массив масок нечетных элементов
	 *  \param dstIndices [in] Выходной массив индексов
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция читает входные данные как однобитный массив проверяя значение каждого бита. Если бит маски равен 1, то его порядковый номер вычисляется для массива четных масок 
	 *  как 2i, для нечетных масок как *2i+1*, где *i* - порядковый номер бита в массиве. Затем этот индекс записывается в *dstIndices*
	 *  
	 */
	int readDividedMask(nm1* maskEven, nm1* maskOdd, int* dstIndices, int size);
	
	//int readMaskToLimitDst(nm1* mask, int* dstIndices, int* treated, int size, int maxSize);
	void remap_32u(nm32u* pSrcVec, nm32u* pDstVec, nm32s* pRemapTable, int size);
	
#ifdef __cplusplus
}
#endif //__cplusplus