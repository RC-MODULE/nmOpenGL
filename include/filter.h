#ifndef __FILTER_H__
#define __FILTER_H__

extern "C" {

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
/*
 * Init function for performance test
 */
static void cmpSrcInit(float* src, int count)
{

  for (int i=0; i<count; i++) {
    src[i] = 1.0;
  }

}

  /*!
   *  \defgroup cmpGtC_f cmpGtC_f  
   *  \ingroup filter_api 
   *  \brief Функция выполняет сравнение элементов входного массива с константой по признаку 'Больше'
   *  \author 
   *  \param src [in] Входной массив элементов для сравнения
   *  \param С[in] Константа для сравнения
   *  \param dstMaskEven [out] Выходная битовая маска. Результат сравнения чётных элементов входного массива с константой
   *  \param dstMaskOdd [out] Выходная битовая маска. Результат сравнения нечётных элементов входного массива с константой
   *  \param size [in] Число элементов входного массива для сравнения
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="src"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="C"> 0.0 </param>
   *          <param name="dstMaskEven"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="dstMaskOdd"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            cmpSrcInit(src, size);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *  \endxmlonly
   */
  //! \{
  //
  void cmpGtC_f(float* src, float C, nm1* dstMaskEven, nm1* dstMaskOdd, int size); 
  //! \}


  /*!
   *  \defgroup cmpLtC_f cmpLtC_f  
   *  \ingroup filter_api 
   *  \brief Функция выполняет сравнение элементов входного массива с константой по признаку 'Меньше'
   *  \author 
   *  \param src [in] Входной массив элементов для сравнения
   *  \param С[in] Константа для сравнения
   *  \param dstMaskEven [out] Выходная битовая маска. Результат сравнения чётных элементов входного массива с константой
   *  \param dstMaskOdd [out] Выходная битовая маска. Результат сравнения нечётных элементов входного массива с константой
   *  \param size [in] Число элементов входного массива для сравнения
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="src"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="C"> 2.0 </param>
   *          <param name="dstMaskEven"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="dstMaskOdd"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            cmpSrcInit(src, size);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *  \endxmlonly
   */
  //! \{
  //
  void cmpLtC_f(float* src, float C, nm1* dstMaskEven, nm1* dstMaskOdd, int size); 
  //! \}


  /*!
   *  \defgroup cmpGteC_f cmpGteC_f  
   *  \ingroup filter_api 
   *  \brief Функция выполняет сравнение элементов входного массива с константой по признаку 'Больше или равно'
   *  \author 
   *  \param src [in] Входной массив элементов для сравнения
   *  \param С[in] Константа для сравнения
   *  \param dstMaskEven [out] Выходная битовая маска. Результат сравнения чётных элементов входного массива с константой
   *  \param dstMaskOdd [out] Выходная битовая маска. Результат сравнения нечётных элементов входного массива с константой
   *  \param size [in] Число элементов входного массива для сравнения
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="src"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="C"> 0.0 </param>
   *          <param name="dstMaskEven"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="dstMaskOdd"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            cmpSrcInit(src, size);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *  \endxmlonly
   */
  //! \{
  //
  void cmpGteC_f(float* src, float C, nm1* dstMaskEven, nm1* dstMaskOdd, int size); 
  //! \}


  /*!
   *  \defgroup cmpLteC_f cmpLteC_f  
   *  \ingroup filter_api 
   *  \brief Функция выполняет сравнение элементов входного массива с константой по признаку 'Меньше или равно'
   *  \author 
   *  \param src [in] Входной массив элементов для сравнения
   *  \param С[in] Константа для сравнения
   *  \param dstMaskEven [out] Выходная битовая маска. Результат сравнения чётных элементов входного массива с константой
   *  \param dstMaskOdd [out] Выходная битовая маска. Результат сравнения нечётных элементов входного массива с константой
   *  \param size [in] Число элементов входного массива для сравнения
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="src"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="C"> 2.0 </param>
   *          <param name="dstMaskEven"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="dstMaskOdd"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            cmpSrcInit(src, size);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *  \endxmlonly
   */
  //! \{
  //
  void cmpLteC_f(float* src, float C, nm1* dstMaskEven, nm1* dstMaskOdd, int size); 
  //! \}

  /*!
   *  \defgroup cmpEqC_f cmpEqC_f  
   *  \ingroup filter_api 
   *  \brief Функция выполняет сравнение элементов входного массива с константой по признаку 'Равно'
   *  \author 
   *  \param src [in] Входной массив элементов для сравнения
   *  \param С[in] Константа для сравнения
   *  \param dstMaskEven [out] Выходная битовая маска. Результат сравнения чётных элементов входного массива с константой
   *  \param dstMaskOdd [out] Выходная битовая маска. Результат сравнения нечётных элементов входного массива с константой
   *  \param size [in] Число элементов входного массива для сравнения
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="src"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="C"> 1.0 </param>
   *          <param name="dstMaskEven"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="dstMaskOdd"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            cmpSrcInit(src, size);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *  \endxmlonly
   */
  //! \{
  //
  void cmpEqC_f(float* src, float C, nm1* dstMaskEven, nm1* dstMaskOdd, int size); 
  //! \}

  /*!
   *  \defgroup cmpNeC_f cmpNeC_f  
   *  \ingroup filter_api 
   *  \brief Функция выполняет сравнение элементов входного массива с константой по признаку 'Не равно'
   *  \author 
   *  \param src [in] Входной массив элементов для сравнения
   *  \param С[in] Константа для сравнения
   *  \param dstMaskEven [out] Выходная битовая маска. Результат сравнения чётных элементов входного массива с константой
   *  \param dstMaskOdd [out] Выходная битовая маска. Результат сравнения нечётных элементов входного массива с константой
   *  \param size [in] Число элементов входного массива для сравнения
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="src"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="C"> 0.0 </param>
   *          <param name="dstMaskEven"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="dstMaskOdd"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <init>
   *            cmpSrcInit(src, size);
   *          </init>
   *          <size> size </size>
   *      </testperf>
   *
   *  \endxmlonly
   */
  //! \{
  //
  void cmpNeC_f(float* src, float C, nm1* dstMaskEven, nm1* dstMaskOdd, int size); 
  //! \}
 
  /*!
   *  \defgroup cmpGtV_f cmpGtV_f  
   *  \ingroup filter_api 
   *  \brief Функция выполняет поэлементное сравнение двух массивов по признаку 'Больше'
   *  \author 
   *  \param src1 [in] Входной массив 1 для сравнения
   *  \param src2 [in] Входной массив 2 для сравнения
   *  \param dstMaskEven [out] Выходная битовая маска. Результат сравнения чётных элементов массивов
   *  \param dstMaskOdd [out] Выходная битовая маска. Результат сравнения нечётных элементов массивов
   *  \param size [in] Количество сравниваемых элементов 
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="src1"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="src2"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="dstMaskEven"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="dstMaskOdd"> im03 im13 im23 im33 im43 im53 im63 im73 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <size> size </size>
   *      </testperf>
   *
   *  \endxmlonly
   */
  //! \{
  //
  void cmpGtV_f(float* src1, float* src2, nm1* dstMaskEven, nm1* dstMaskOdd, int size); 
  //! \}

  /*!
   *  \defgroup cmpGteV_f cmpGteV_f  
   *  \ingroup filter_api 
   *  \brief Функция выполняет поэлементное сравнение двух массивов по признаку 'Больше или равно'
   *  \author 
   *  \param src1 [in] Входной массив 1 для сравнения
   *  \param src2 [in] Входной массив 2 для сравнения
   *  \param dstMaskEven [out] Выходная битовая маска. Результат сравнения чётных элементов массивов
   *  \param dstMaskOdd [out] Выходная битовая маска. Результат сравнения нечётных элементов массивов
   *  \param size [in] Количество сравниваемых элементов 
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="src1"> im00 im10 im30 im30 im40 im50 im60 im70 </param>
   *          <param name="src2"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="dstMaskEven"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="dstMaskOdd"> im03 im13 im23 im33 im43 im53 im63 im73 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <size> size </size>
   *      </testperf>
   *
   *  \endxmlonly
   */
  //! \{
  //
  void cmpGteV_f(float* src1, float* src2, nm1* dstMaskEven, nm1* dstMaskOdd, int size); 
  //! \}

  /*!
   *  \defgroup cmpLtV_f cmpLtV_f  
   *  \ingroup filter_api 
   *  \brief Функция выполняет поэлементное сравнение двух массивов по признаку 'Меньше'
   *  \author 
   *  \param src1 [in] Входной массив 1 для сравнения
   *  \param src2 [in] Входной массив 2 для сравнения
   *  \param dstMaskEven [out] Выходная битовая маска. Результат сравнения чётных элементов массивов
   *  \param dstMaskOdd [out] Выходная битовая маска. Результат сравнения нечётных элементов массивов
   *  \param size [in] Количество сравниваемых элементов 
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="src1"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="src2"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="dstMaskEven"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="dstMaskOdd"> im03 im13 im23 im33 im43 im53 im63 im73 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <size> size </size>
   *      </testperf>
   *
   *  \endxmlonly
   */
  //! \{
  //
  void cmpLtV_f(float* src1, float* src2, nm1* dstMaskEven, nm1* dstMaskOdd, int size); 
  //! \}

  /*!
   *  \defgroup cmpLteV_f cmpLteV_f  
   *  \ingroup filter_api 
   *  \brief Функция выполняет поэлементное сравнение двух массивов по признаку 'Меньше или равно'
   *  \author 
   *  \param src1 [in] Входной массив 1 для сравнения
   *  \param src2 [in] Входной массив 2 для сравнения
   *  \param dstMaskEven [out] Выходная битовая маска. Результат сравнения чётных элементов массивов
   *  \param dstMaskOdd [out] Выходная битовая маска. Результат сравнения нечётных элементов массивов
   *  \param size [in] Количество сравниваемых элементов 
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="src1"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="src2"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="dstMaskEven"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="dstMaskOdd"> im03 im13 im23 im33 im43 im53 im63 im73 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <size> size </size>
   *      </testperf>
   *
   *  \endxmlonly
   */
  //! \{
  //
  void cmpLteV_f(float* src1, float* src2, nm1* dstMaskEven, nm1* dstMaskOdd, int size); 
  //! \}


  /*!
   *  \defgroup cmpEqV_f cmpEqV_f  
   *  \ingroup filter_api 
   *  \brief Функция выполняет поэлементное сравнение двух массивов по признаку 'Равно'
   *  \author 
   *  \param src1 [in] Входной массив 1 для сравнения
   *  \param src2 [in] Входной массив 2 для сравнения
   *  \param dstMaskEven [out] Выходная битовая маска. Результат сравнения чётных элементов массивов
   *  \param dstMaskOdd [out] Выходная битовая маска. Результат сравнения нечётных элементов массивов
   *  \param size [in] Количество сравниваемых элементов 
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="src1"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="src2"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="dstMaskEven"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="dstMaskOdd"> im03 im13 im23 im33 im43 im53 im63 im73 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <size> size </size>
   *      </testperf>
   *
   *  \endxmlonly
   */
  //! \{
  //
  void cmpEqV_f(float* src1, float* src2, nm1* dstMaskEven, nm1* dstMaskOdd, int size); 
  //! \}


  /*!
   *  \defgroup cmpNeV_f cmpNeV_f  
   *  \ingroup filter_api 
   *  \brief Функция выполняет поэлементное сравнение двух массивов по признаку 'Не равно'
   *  \author 
   *  \param src1 [in] Входной массив 1 для сравнения
   *  \param src2 [in] Входной массив 2 для сравнения
   *  \param dstMaskEven [out] Выходная битовая маска. Результат сравнения чётных элементов массивов
   *  \param dstMaskOdd [out] Выходная битовая маска. Результат сравнения нечётных элементов массивов
   *  \param size [in] Количество сравниваемых элементов 
   *  
   *  \par
   *  \xmlonly
   *      <testperf>
   *          <param name="src1"> im00 im10 im20 im30 im40 im50 im60 im70 </param>
   *          <param name="src2"> im01 im11 im21 im31 im41 im51 im61 im71 </param>
   *          <param name="dstMaskEven"> im02 im12 im22 im32 im42 im52 im62 im72 </param>
   *          <param name="dstMaskOdd"> im03 im13 im23 im33 im43 im53 im63 im73 </param>
   *          <param name="size"> 128 256 512 1024 </param>
   *          <size> size </size>
   *      </testperf>
   *
   *  \endxmlonly
   */
  //! \{
  //
  void cmpNeV_f(float* src1, float* src2, nm1* dstMaskEven, nm1* dstMaskOdd, int size); 
  //! \}
}


#endif 
