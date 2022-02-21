#ifndef __UTILITY_H__
#define __UTILITY_H__
#include "nmtype.h"

#ifdef __cplusplus
#include "utility_cpp.h"
#endif //__cplusplus



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
	
/*
 * Init function for performance test
 */
static void maskInit(nm1* maskEven, nm1* maskOdd, int evenValue, int oddValue, int count)
{

  int* maskEvenTmp = (int*)maskEven;
  int* maskOddTmp= (int*)maskOdd;

  for (int i=0; i<count; i++) {
    maskEvenTmp[i] = evenValue;
    maskOddTmp[i] = oddValue;
  }

}

  /*!
   *  \defgroup cnvDividedMaskToIndices cnvDividedMaskToIndices
   *  \ingroup filter_api 
   *  \brief Формирование массива индексов единичных элементов чётной и нечётной масок. Чётная и нечётная маски являются частями объединённой маски и содержат соответственно чётные и нечётные элементы объединённой маски.
   *  \author 
   *  \param maskEven [in] Чётная маска 
   *  \param maskOdd [in]  Нечётная маска
   *  \param indices [out] Выходной массив индексов 
   *  \param size [in] Суммарное количество элементов объединённой маски, которое необходимо проанализировать
   *  
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="maskEven"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="maskOdd"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="indices"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(maskEven, maskOdd, 0x0, 0x0, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *      <testperf>
   *          <param name="maskEven"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="maskOdd"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="indices"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(maskEven, maskOdd, 0xFFFFFFFF, 0xFFFFFFFF, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *      <testperf>
   *          <param name="maskEven"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="maskOdd"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="indices"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(maskEven, maskOdd, 0xFFFFFFFF, 0x00000000, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *      <testperf>
   *          <param name="maskEven"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="maskOdd"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="indices"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(maskEven, maskOdd, 0x00000000, 0xFFFFFFFF, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *  \endxmlonly
   */
  //! \{
  int cnvDividedMaskToIndices(nm1* maskEven, nm1* maskOdd, int* indices, int size);
  //! \}


  /*!
   *  \defgroup cnvDividedMaskToOffsets cnvDividedMaskToOffsets
   *  \ingroup filter_api 
   *  \brief Формирование массива смещений между единичными элементами чётной и нечётной масок. Чётная и нечётная маски являются частями объединённой маски и содержат соответственно чётные и нечётные элементы объединённой маски.
   *  \author 
   *  \param maskEven [in] Чётная маска 
   *  \param maskOdd [in]  Нечётная маска 
   *  \param offsets[out] Выходной массив смещений 
   *  \param size [in] Суммарное количество элементов объединённой маски, которое необходимо проанализировать
   *  
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="maskEven"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="maskOdd"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="offsets"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(maskEven, maskOdd, 0x0, 0x0, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *      <testperf>
   *          <param name="maskEven"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="maskOdd"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="offsets"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(maskEven, maskOdd, 0xFFFFFFFF, 0xFFFFFFFF, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *      <testperf>
   *          <param name="maskEven"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="maskOdd"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="offsets"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(maskEven, maskOdd, 0xFFFFFFFF, 0x00000000, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *      <testperf>
   *          <param name="maskEven"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="maskOdd"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="offsets"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(maskEven, maskOdd, 0x00000000, 0xFFFFFFFF, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *  \endxmlonly
   */
  //! \{
int cnvDividedMaskToOffsets(nm1* maskEven, nm1* maskOdd, int* offsets, int size);
  //! \}


  /*!
   *  \defgroup cnvMaskToIndices cnvMaskToIndices
   *  \ingroup filter_api 
   *  \brief Формирование массива индексов между единичными элементами маски
   *  \author 
   *  \param mask [in] Маска
   *  \param indices[out] Выходной массив индексов 
   *  \param size [in] Количество элементов маски, которое необходимо проанализировать
   *  
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="mask"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="indices"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(mask, mask, 0x0, 0x0, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *      <testperf>
   *          <param name="mask"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="indices"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(mask, mask, 0xFFFFFFFF, 0xFFFFFFFF, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *      <testperf>
   *          <param name="mask"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="indices"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(mask, mask, 0x55555555, 0x55555555, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *      <testperf>
   *          <param name="mask"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="indices"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(mask, mask, 0xAAAAAAAA, 0xAAAAAAAA, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *  \endxmlonly
   */
  //! \{
int cnvMaskToIndices(nm1* mask, int* indices, int size);
  //! \}


  /*!
   *  \defgroup cnvMaskToOffsets cnvMaskToOffsets
   *  \ingroup filter_api 
   *  \brief Формирование массива смещений между единичными элементами маски
   *  \author 
   *  \param mask [in] Маска 
   *  \param offsets[out] Выходной массив смещений 
   *  \param size [in] Число элементов
   *  
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="mask"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="offsets"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(mask, mask, 0x0, 0x0, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *      <testperf>
   *          <param name="mask"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="offsets"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(mask, mask, 0xFFFFFFFF, 0xFFFFFFFF, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *      <testperf>
   *          <param name="mask"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="offsets"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(mask, mask, 0x55555555, 0x55555555, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *      <testperf>
   *          <param name="mask"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="offsets"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            maskInit(mask, mask, 0xAAAAAAAA, 0xAAAAAAAA, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *  \endxmlonly
   */
  //! \{
int cnvMaskToOffsets(nm1* mask, int* offsets, int size);
  //! \}



/*
 * Init function for performance test
 */
static void offsetsInit(int* offsets, int value, int count)
{

  for (int i=0; i<count; i++) {
    offsets[i] = value;
  }

}

  /*!
   *  \defgroup selectElementsByOffsets selectElementsByOffsets 
   *  \ingroup filter_api 
   *  \brief Функция копирует элементы входного массива, определяемые массивом смещений между элементами, в выходной массив.
   *  \author 
   *  \param src [in] Входной массив элементов 
   *  \param offsets[in] Массив смещений 
   *  \param dst [out] Выходной массив элементов
   *  \param sizeOfElementInt [in] Размер элемента в int'ах
   *  \param size [in] Число элементов
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="src"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="offsets"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="dst"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="sizeOfElementInt"> 1 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            offsetsInit(offsets, 0xFFFFFFFF, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *      <testperf>
   *          <param name="src"> im00 </param>
   *          <param name="offsets"> im01 </param>
   *          <param name="dst"> im02 </param>
   *          <param name="sizeOfElementInt"> 1 2 4 8 16 32</param>
   *          <param name="size"> 1 2 4 8 16 32</param>
   *          <init>
   *            offsetsInit(offsets, 0xFFFFFFFF, size/32);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *  \endxmlonly
   */
  //! \{
  void selectElementsByOffsets(void* src, int* offsets, void* dst, int sizeOfElementInt, int size);
  //! \}


/*
 * Init function for performance test
 */
static void indicesInit(int* indices, int count)
{

  for (int i=0; i<count; i++) {
    indices[i] = i;
  }

}

  /*!
   *  \defgroup selectElementsByIndices selectElementsByIndices 
   *  \ingroup filter_api 
   *  \brief Функция копирует элементы входного массива с заданными индексами в выходной массив
   *  \author 
   *  \param src [in] Входной массив элементов 
   *  \param indices[in] Массив индексов 
   *  \param dst [out] Выходной массив элементов
   *  \param sizeOfElementInt [in] Размер элемента в int'ах
   *  \param size [in] Число элементов
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="src"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="indices"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="dst"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="sizeOfElementInt"> 1 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            indicesInit(indices, size);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *      <testperf>
   *          <param name="src"> im00 </param>
   *          <param name="indices"> im01 </param>
   *          <param name="dst"> im02 </param>
   *          <param name="sizeOfElementInt"> 1 2 4 8 16 32</param>
   *          <param name="size"> 1 2 4 8 16 32</param>
   *          <init>
   *            indicesInit(indices, size);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *  \endxmlonly
   */
  //! \{
  void selectElementsByIndices(void* src, int* indices, void* dst, int sizeOfElementInt, int size);
  //! \}


  /*!
   *  \defgroup deselectElementsByIndices deselectElementsByIndices 
   *  \ingroup filter_api 
   *  \brief Функция копирует элементы входного массива в элементы выходного массива с заданными индексами
   *  \author 
   *  \param src [in] Входной массив элементов 
   *  \param indices[in] Массив индексов 
   *  \param dst [out] Выходной массив элементов
   *  \param sizeOfElementInt [in] Размер элемента в int'ах
   *  \param size [in] Число копируемых элементов
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="src"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="indices"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="dst"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="sizeOfElementInt"> 1 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            indicesInit(indices, size);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *      <testperf>
   *          <param name="src"> im00 </param>
   *          <param name="indices"> im01 </param>
   *          <param name="dst"> im02 </param>
   *          <param name="sizeOfElementInt"> 1 2 4 8 16 32</param>
   *          <param name="size"> 1 2 4 8 16 32</param>
   *          <init>
   *            indicesInit(indices, size);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *  \endxmlonly
   */
  //! \{
  void deselectElementsByIndices(void* src, int* indices, void* dst, int sizeOfElementInt, int size);
  //! \}
    
#ifdef __cplusplus
}
#endif //__cplusplus



#endif //__UTILITY_H__