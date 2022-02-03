#pragma once
#include "utility.h"

extern "C"{

	void abs_32f(float* src, float* dst, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция сложения массива 4-мерных векторов с одиночным 4-мерным вектором
	 *  \author Жиленков Иван
	 *  
	 *  \param pSrcV [in] Входной массив 4-хмерных векторов
	 *  \param pSrcC [in] Указатель на одиночный 4-мерный вектор
	 *  \param pDst [out] Выходной массив
	 *  \param size [in] Число векторов
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="pSrcV"> im0 </param>
	 *          <param name="pSrcC"> im0 </param>
	 *          <param name="pDst"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *          <size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void addC_v4nm32f(v4nm32f* pSrcV, v4nm32f* pSrcC, v4nm32f* pDst, int size);
	 //! \}
	

	/*!
	 *  \ingroup service_api
	 *  \brief Функция вычисления основной части освещения
	 *  \author Жиленков Иван
	 *  
	 *  \param ambient [in] Указатель на амбиентную составляющую источника освещения
	 *  \param n_dot_vp [in] Указатель на продублированный массив скалярного произведения нормали и дистанцией между источником и точкой
	 *  \param diffuse [in] Указатель на амбиентную составляющую источника освещения
	 *  \param n_dot_h_in_srm [in] Указатель на возведенный в степень srm продублированный массив скалярного произведения нормали и вектора h
	 *  \param specular [in]  Указатель на массив specular
	 *  \param dst [out] Выходной массив
	 *  \param count [in] Число векторов
	 *  
	 *  \details Функция вычисляет основную часть освещения по формуле 
	 *  \f[res = a + nvp * d + f(nvp) * nh * s\f], где
	 *  *a* - *ambient*, *nvp* - *n_dot_vp*,  *d* - *diffuse*, *nh* - *n_dot_h_in_srm*, *s* - *specular*
	 *  \f[f(x) = 1, if (x!=0), else 0	\f]
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="ambient"> im0 </param>
	 *          <param name="n_dot_vp"> im0 </param>
	 *          <param name="diffuse"> im0 </param>
	 *          <param name="n_dot_h_in_srm"> im0 im1 </param>
	 *          <param name="specular"> im0 </param>
	 *          <param name="dst"> im0 im1 im2 </param>
	 *          <param name="count"> 128 512 1024 </param>
	 *          <size> count </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void baseLighti(v4nm32f* ambient, v2nm32f* n_dot_vp, v4nm32f* diffuse, v2nm32f* n_dot_h_in_srm, v4nm32f* specular, v4nm32f* dst, int count);
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция приведения чисел к фиксированному диапазону
	 *  \author Жиленков Иван
	 *  
	 *  \param pSrcVec [in] Входной массив
	 *  \param min [in] Минимальный порог диапазона
	 *  \param max [in] Максимальный порог диапазона
	 *  \param pDstVec [out] Выходной массив
	 *  \param size [in] Число элементов массива
	 *  
	 *  \details Функция вычисляет следующее выражение
	 *  \f[
		f(x) = 
		\begin{cases}
		min & x < min\\
		x & min < x < max \\
		max & x > max
		\end{cases} \f]
	 
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="pSrcVec"> im0 </param>
	 *          <param name="min"> 0 </param>
	 *          <param name="max"> 1 </param>
	 *          <param name="pDstVec"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *  		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void clamp_32f(nm32f* pSrcVec, float min, float max, nm32f* pDstVec, int size);
	 //! \}
		
	/*!
	 *  \ingroup service_api
	 *  \brief Функция конвертации двухмерного вектора в четырехмерный 
	 *  с заданными значениями третьей и четвертой компоненты
	 *  \author Жиленков Иван
	 *  
	 *  \param srcVec [in] Входный массив
	 *  \param dstVec [out] Выходной массив
	 *  \param value3 [in] Значение третьей компоненты элемента выходного массива
	 *  \param value4 [in] Значение четвертой компоненты элемента выходного массива
	 *  \param size [in] Размер массива в элементах
	 *  
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="stride"> 0 1 </param>
	 *          <param name="value3"> 1 </param>
	 *          <param name="value4"> 2 </param>
	 *          <param name="dstVec"> im0 im1 </param>
	 *          <param name="countVec"> 128 512 1024 </param>
	 *  		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void cnv32f_v2v4(const v2nm32f* srcVec, v4nm32f* dstVec, float value3, float value4, int size);
	 //! \}
	 
	 /*!
	 *  \ingroup service_api
	 *  \brief Функция конвертации трехмерного вектора в четырехмерный 
	 *  \author Жиленков Иван
	 *  
	 *  \param srcVec [in] Входный массив
	 *  \param dstVec [out] Выходный массив
	 *  \param value4 [in] Значение четвертой компоненты элемента выходного массива
	 *  \param size [in] Размер массива в элементах
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="src_v3nm32f"> </param>
	 *          <param name="dst_v4nm32f"> </param>
	 *          <param name="value4"> </param>
	 *          <param name="size"> </param>
	 *          <size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void cnv32f_v3v4(const nm32f* srcVec, nm32f* dstVec, float value4, int size);
	 //! \}
	
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
	 *  \ingroup service_api
	 *  \brief Функция скалярного произведения массивa векторов и постоянного вектора с последующимся занулением отрицательных значений
	 *  \author Жиленков Иван
	 *  
	 *  \param srcVec [in] Входной массив векторов
	 *  \param srcC [in] Указатель на постоянный вектор
	 *  \param dstValues [out] Выходной массив полученных значений (продублированный)
	 *  \param size [in] Число векторов в массиве
	 *  
	 *  \details Функция считает скалярное произведение двух векторов и проверяет знак. Если результат больше нуля, то пишется результат, если меньше
	 *  нуля, то пишется ноль. В выходном массиве результат пишется как два одинаковых числа, идующих подряд. Это сделано для удобства последующей работы
	 *  с векторным сопроцессором
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="srcC"> im0 </param>
	 *          <param name="dstValues"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *   		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void dotC_gt0_v4nm32f(v4nm32f* srcVec, v4nm32f* srcC, v2nm32f* dstValues, int size);
	 //! \}
	
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция скалярного произведения массивов векторов с последующимся занулением отрицательных значений
	 *  \author Жиленков Иван
	 *  
	 *  \param srcVec1 [in] Первый входной массив
	 *  \param srcVec2 [in] Второй входной массив
	 *  \param dstValues [out] Выходной массив полученных значений (продублированный)
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция считает скалярное произведение двух векторов и проверяет знак. Если результат больше нуля, то пишется результат, если меньше
	 *  нуля, то пишется ноль. В выходном массиве результат пишется в таком виде \f$ (r_0,r_0), (r_1,r_1)... (r_n,r_n) \f$. Это сделано для удобства последующей работы
	 *  с векторным сопроцессором
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec1"> im0 </param>
	 *          <param name="srcVec2"> im0 im1 </param>
	 *          <param name="dstValues"> im0 im1 im2 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *    		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	void dotV_gt0_v4nm32f(v4nm32f* srcVec1, v4nm32f* srcVec2, v2nm32f* dstValues, int size);
	 
	 /*!
	 *  \ingroup service_api
	 *  \brief Функция скалярного произведения массивов векторов
	 *  \author Жиленков Иван
	 *  
	 *  \param srcVec1 [in] Первый входной массив
	 *  \param srcVec2 [in] Второй входной массив
	 *  \param dstValues [out] Выходной массив полученных значений (продублированный)
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция считает скалярное произведение двух векторов в выходной массив получившийся результат. В выходном массиве результат 
	 *  пишется в таком виде \f$ (r_0,r_0), (r_1,r_1)... (r_n,r_n) \f$. Это сделано для удобства последующей работы
	 *  с векторным сопроцессором
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec1"> im0 </param>
	 *          <param name="srcVec2"> im0 im1 </param>
	 *          <param name="dstValues"> im0 im1 im2 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *    		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	void dotV_v4nm32f(v4nm32f* srcVec1, v4nm32f* srcVec2, v2nm32f* dstValues, int size);
	
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция умножения постоянного 4-хмерного вектора на массив констант с прибавлением другого постоянного вектора.
	 *  Массив констант должен быть продублированным
	 *  \author Жиленков Иван
	 *  
	 *  \param srcVec [in] Входной массив констант
	 *  \param mulC [in] Указатель на постоянный вектор-множитель
	 *  \param addC [in] Указатель на постоянный вектор-слагаемое
	 *  \param dst [out] Выходной массив векторов
	 *  \param size [in] Число векторов
	 *  
	 *  \details Массив констант должен быть  продублированным, то есть вида \f$ (c_0,c_0), (c_1,c_1)... (c_n,c_n) \f$
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="mulC"> im0 </param>
	 *          <param name="addC"> im0 </param>
	 *          <param name="dst"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *    		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void dotMulC_AddC_v4nm32f(v2nm32f* srcVec, v4nm32f* mulC, v4nm32f* addC, v4nm32f* dst, int size);
	 //! \}

	 /*!
	 *  \ingroup service_api
	 *  \brief Функция умножения массива 4-хмерных векторов 
	 *  на массив констант
	 *  \author Жиленков Иван
	 *
	 *  \param srcVec [in] Входной массив констант
	 *  \param mulVec [in] Указатель на массив векторов
	 *  \param dst [out] Выходной массив векторов
	 *  \param size [in] Число векторов
	 *  
	 *  \details Массив констант должен быть  продублированным, то есть вида \f$ (c_0,c_0), (c_1,c_1)... (c_n,c_n) \f$
	 *
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="mulVec"> im0 </param>
	 *          <param name="dst"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *  		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void dotMulV_v4nm32f(v2nm32f* srcVec, v4nm32f* mulVec, v4nm32f* dst, int size);
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция умножения постоянного 4-хмерного вектора на массив констант с прибавлением массива других 4-хмерных векторов.
	 *  Массив констант должен быть продублированным
	 *  \author Жиленков Иван
	 *  
	 *  \param srcVec [in] Массив констант
	 *  \param mulC [in] Указатель на умножающийся постоянный вектор
	 *  \param addVec [in] Массив прибавляющихся векторов
	 *  \param dst [out] Выходной массив
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция делает следующую операцию \f$ f(x,y) = C * (x_0, x_1, x_2, x_3) +  (y_0, y_1, y_2, y_3)\f$. Константы C должен быть продублированным, то есть массив
	 *  констант должен быть вида \f$ (c_0,c_0), (c_1,c_1)... (c_n,c_n) \f$
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="mulC"> im0 </param>
	 *          <param name="addVec"> im0 im1 </param>
	 *          <param name="dst"> im0 im1 im2 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *    		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void dotMulC_Add_v4nm32f(v2nm32f* srcVec, v4nm32f* mulC, v4nm32f* addVec, v4nm32f* dst, int size);
	 //! \}

	/*!
	 *  \ingroup service_api
	 *  \brief Функция нахождения отрицательных смещений (с двумя параллельными обработками)
	 *  \author Жиленков Иван
	 *  
	 *  \param src1 [in] Входной массив 1
	 *  \param src2 [in] Входной массив 2
	 *  \param dst1 [in] Выходный массив 1
	 *  \param dst2 [in] Выходный массив 2
	 *  \param size [in] Число обрабатываемых элементов (для обоих массивов одинаковое)
	 *  
	 *  \details Функция вычисляет следующее выражение
	 *  \f[
	 *  f(x)=
	 *  \begin{cases}
	 *  |x| & x < 0 \\
	 *  0 & x >= 0
	 *  \end{cases}
	 *  \f]
	 *  
	 *  Функция берет данные от двух источников и если все массивы распределены по разным банкам памяти, то обрабатывает их параллельно
	 *  
	 */
	void doubleAbsIfNegElse0_32f(float* src1, float* src2, float* dst1, float* dst2, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция производит два параллельных сложения
	 *  \author Жиленков Иван
	 *  
	 *  \param src1 [in] Первый входной массив 1
	 *  \param src2 [in] Первый входной массив 2
	 *  \param srcAdd1 [in] Второй входной массив 1
	 *  \param srcAdd2 [in] Второй входной массив 2
	 *  \param dst1 [in] Выходной массив 1
	 *  \param dst2 [in] Выходной массив 2
	 *  \param size [in] Число обрабатываемых элементов (для обоих массивов одинаковое)
	 *  
	 *  \details Функция паралелльно вычисляет два сложения. *dst1 = src1 + srcAdd1, dst2 = src2 + srcAdd2*
	 *  
	 */
	void doubleAdd_32f(float* src1, float* src2, float* srcAdd1, float* srcAdd2, float* dst1, float* dst2, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция приведения чисел к фиксированному диапазону
	 *  \author Жиленков Иван
	 *  
	 *  \param src1 [in] Входной массив 1
	 *  \param src2 [in] Входной массив 2
	 *  \param min [in] Минимальный порог диапазона
	 *  \param max [in] Максимальный порог диапазона
	 *  \param dst1 [out] Выходной массив 1
	 *  \param dst2 [out] Выходной массив 2
	 *  \param size [in] Число элементов массива
	 *  
	 *  \details Функция осуществляет те же действия что и функия clamp_32f, но параллельно для двух массивов
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="pSrcVec"> im0 </param>
	 *          <param name="min"> 0 </param>
	 *          <param name="max"> 1 </param>
	 *          <param name="pDstVec"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *  		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	void doubleClamp_32f(float* src1, float* src2, float min, float max, float* dst1, float* dst2, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Brief description
	 *  \author Жиленков Иван
	 *  
	 *  \param src1 [in] Description for src1
	 *  \param src2 [in] Description for src2
	 *  \param C1 [in] Description for C1
	 *  \param C2 [in] Description for C2
	 *  \param dst1 [in] Description for dst1
	 *  \param dst2 [in] Description for dst2
	 *  \param size [in] Description for size
	 *  
	 *  \details Details
	 *  
	 */
	void doubleMulC_32f(float* src1, float* src2, float C1, float C2, float* dst1, float* dst2, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция производит два параллельных вычитания
	 *  \author Жиленков Иван
	 *  
	 *  \param src1 [in] Первый входной массив 1
	 *  \param src2 [in] Первый входной массив 2
	 *  \param srcSub1 [in] Второй входной массив 1
	 *  \param srcSub2 [in] Второй входной массив 2
	 *  \param dst1 [in] Выходной массив 1
	 *  \param dst2 [in] Выходной массив 2
	 *  \param size [in] Число обрабатываемых элементов (для обоих массивов одинаковое)
	 *  
	 *  \details Функция паралелльно осуществляет два вычитания. *dst1 = src1 - srcSub1, dst2 = src2 - srcSub2*
	 *  
	 */
	void doubleSub_32f(float* src1, float* src2, float* srcSub1, float* srcSub2, float* dst1, float* dst2, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция производит два параллельных вычитания
	 *  \author Жиленков Иван
	 *  
	 *  \param src1 [in] Первый входной массив 1
	 *  \param src2 [in] Первый входной массив 2
	 *  \param С1 [in] Константа 1
	 *  \param С2 [in] Константа 2
	 *  \param dst1 [in] Выходной массив 1
	 *  \param dst2 [in] Выходной массив 2
	 *  \param size [in] Число обрабатываемых элементов (для обоих массивов одинаковое)
	 *  
	 *  \details Функция паралелльно вычитает два вычитания константы из массива. *dst1 = src1 - С1, dst2 = src2 - С2*
	 *  
	 */
	void doubleSubC_32f(float* src1, float* src2, float C1, float C2, float* dst1, float* dst2, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция быстрого обратного корня
	 *  \author Жиленков Иван
	 *  
	 *  \param srcVec [in] Входной массив
	 *  \param dstVec [in] Выходной массив
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция вычисляет выражение \f[ \frac{1} {\sqrt{x}} \f]. 
	 *  Алгоритм действия функии такой:
	    1. Трактуя 32-битное дробное число как целое, проводит операцию \f$ y_0 = {5F3759DF_{16}}-(x >> 1) \f$ , где >> — битовый сдвиг вправо. Результат снова трактует как 32-битное дробное число.
	    2. Для уточнения проводится одна итерация метода Ньютона: \f$ y_1 = y_0(1,5 - 0,5xy_0^2) \f$ .
		
		Точность функции составляет \f$(+0%, -0.18%) \f$, что для целей компьютерной графики более чем подходит (1/256 \approx 0,39%)
	 */
	void fastInvSqrt(float* srcVec, float* dstVec, int size);
	
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
	 *  \brief Поэлементный поиск минимум и максимума из двух массивов
	 *  \author Жиленков Иван
	 *  
	 *  \param srcVec1 [in] Первый входной массив
	 *  \param srcVec2 [in] Второй входной массив
	 *  \param dstMin [out] Выходной массив минимумов
	 *  \param dstMax [out] Выходной массив максимумов
	 *  \param size [in] Число элементов
	 *  
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec1"> im0 </param>
	 *          <param name="srcVec2"> im0 im1 </param>
	 *          <param name="dstMin"> im0 im1 im2 </param>
	 *          <param name="dstMax"> im0 im1 im2 im3 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *          <size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void findMinMax2(float* srcVec1, float* srcVec2, float* dstMin, float* dstMax, int size);	
	 //! \}

	/*!
	 *  \ingroup service_api
	 *  \brief Поэлементный поиск минимума и максимума из трех массивов
	 *  \author Жиленков Иван
	 *  
	 *  \param src1 [in] Первый входной массив
	 *  \param src2 [in] Второй входной массив
	 *  \param src3 [in] Третий входной массив
	 *  \param dstMin [out] Выходной массив с минимумом
	 *  \param dstMax [out] Выходной массив с максимумом
	 *  \param size [in] Число элементов
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="src1"> im0 </param>
	 *          <param name="src2"> im0 </param>
	 *          <param name="src3"> im0 </param>
	 *          <param name="dstMin"> im0 im1 </param>
	 *          <param name="dstMax"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *    		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void findMinMax3(float* src1, float* src2, float* src3, float* dstMin, float* dstMax, int size);
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция инициализации матрицы единичной матрицей
	 *  \author Жиленков Иван
	 *  
	 *  \param matrix [out] Выходная матрица
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="matrix"> im0 </param>
	 *    		<size> 16 </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void loadIdentify(mat4nm32f* matrix);
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция поиска среднего значения из трех элементов и конвертирования его в целочисленный тип
	 *  \author Жиленков Иван
	 *  
	 *  \param src1 [in] Входной массив первых значений
	 *  \param src2 [in] Входной массив вторых значений
	 *  \param src3 [in] Входной массив третьих значений
	 *  \param result [out] Выходной массив в целочисленном формате
	 *  \param size [in] Число элементов массива
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="src1"> im0 </param>
	 *          <param name="src2"> im0 im1 </param>
	 *          <param name="src3"> im0 im1 im2 </param>
	 *          <param name="result"> im0 im1 im2 im3 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *    		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void meanToInt3(float* src1, float* src2, float* src3, int* result, int size);
	 //! \}
	 
	 /*!
	 *  \ingroup service_api
	 *  \brief Функция поиска среднего значения из двух элементов и конвертирования его в целочисленный тип
	 *  \author Жиленков Иван
	 *  
	 *  \param src1 [in] Входной массив первых значений
	 *  \param src2 [in] Входной массив вторых значений
	 *  \param result [out] Выходной массив
	 *  \param size [in] Число элементов массива
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="src1"> im0 </param>
	 *          <param name="src2"> im0 im1 </param>
	 *          <param name="result"> im0 im1 im2 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *          <size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void meanToInt2(float* src1, float* src2, int* result, int size);
	 //! \}
	
		
	/*!
	 *  \ingroup service_api
	 *  \brief Функция умножение массива на постоянное значение
	 *  \author Жиленков Иван
	 *  
	 *  \param pSrcV [in] Входной массив
	 *  \param pSrcC [in] Указатель на постоянное значение
	 *  \param pDst [out] Выходной массив
	 *  \param size [in] Число элементов
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="pSrcV"> im0 </param>
	 *          <param name="pSrcC"> im0 </param>
	 *          <param name="pDst"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *    		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void mulC_v4nm32f(v4nm32f* pSrcV, v4nm32f* pSrcC, v4nm32f* pDst, int size);
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция умножение матрицы 4х4 на массив 4хмерных векторов. Матрица задается по столбцам
	 *  \author Жиленков Иван
	 *  
	 *  \param srcMat4x4 [in] Входная матрица 4х4 (column-major order)
	 *  \param srcVec4xN [in] Входной массив векторов
	 *  \param dstVec4xN [in] Выходной массив
	 *  \param count [in] Число векторов
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcMat4x4"> im0 </param>
	 *          <param name="srcVec4xN"> im0 im1 </param>
	 *          <param name="dstVec4xN"> im0 im1 im2 </param>
	 *          <param name="count"> 128 512 1024 </param>
	 *    		<size> count </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void mul_mat4nm32f_v4nm32f(mat4nm32f* srcMat4x4,	v4nm32f* srcVec4xN, v4nm32f* dstVec4xN, int count);
	 //! \}

	 /*!
	 *  \ingroup service_api
	 *  \brief Функция массива 4хмерных векторов на матрицу 4х4. Матрица задается по строкам
	 *  \author Жиленков Иван
	 *
	 *  \param srcMat4x4 [in] Входная матрица 4х4 (row-major order)
	 *  \param srcVec4xN [in] Входной массив векторов
	 *  \param dstVec4xN [in] Выходной массив
	 *  \param count [in] Число векторов
	 *
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcMat4x4"> im0 </param>
	 *          <param name="srcVec4xN"> im0 im1 </param>
	 *          <param name="dstVec4xN"> im0 im1 im2 </param>
	 *          <param name="count"> 128 512 1024 </param>
	 *     		<size> count </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void mul_v4nm32f_mat4nm32f(v4nm32f* srcVec4xN, mat4nm32f* srcMat4x4, v4nm32f* dstVec4xN, int count);
	 //! \}
	
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

	/*!
	 *  \ingroup service_api
	 *  \brief Функция замены нуля другим значением
	 *  \author Жиленков Иван
	 *  
	 *  \param srcVec [in] Входной массив
	 *  \param dstVec [out] Выходной массив
	 *  \param size [in] Число элементов в массиве
	 *  \param nReplaceC [in] Втсавляемое число
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="dstVec"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *          <param name="nReplaceC"> 1 </param>
	 *     		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void replaceEq0_32f(float* srcVec, float* dstVec, int size, float nReplaceC);
	 //! \}
	
	

	/*!
	 *  \ingroup service_api
	 *  \brief Функция инициализации элементов массива постоянным значением.
	 *  \author Жиленков Иван
	 *  
	 *  \param dstVec [out] Инициализируемый массив
	 *  \param valueC [in] Значение константы
	 *  \param size [in] Размер вектора в элементах
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="dstVec"> im0 </param>
	 *          <param name="valueC"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *      		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void set_v4nm32f(v4nm32f* dstVec, v4nm32f* valueC, int size);
	 //! \}
	
	

	/*!
	 *  \ingroup service_api
	 *  \brief Функция поэлементной сортировки двухмерных элементов по возрастанию 2-й компоненты
	 *  \author Жиленков Иван
	 *  
	 *  \param srcAxy [in, out] Первый входной массив двухмерных векторов
	 *  \param srcBxy [in, out] Второй входной массив двухмерных векторов
	 *  \param srcCxy [in, out] Третий входной массив двухмерных векторов
	 *  \param size [in] Число элементов
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcAxy"> im0 </param>
	 *          <param name="srcBxy"> im0 im1 </param>
	 *          <param name="srcCxy"> im0 im1 im2 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *      	<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void sortByY3(float* srcAxy, float* srcBxy, float* srcCxy, int size);
	 //! \}
	 
	 /*!
	 *  \ingroup service_api
	 *  \brief Функция поэлементной сортировки двухмерных элементов по возрастанию 2-й компоненты
	 *  \author Жиленков Иван
	 *  
	 *  \param srcXY0 [in, out] Первый входной массив двухмерных векторов
	 *  \param srcXY1 [in, out] Второй входной массив двухмерных векторов
	 *  \param size [in] Число элементов
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcXY0"> im0 </param>
	 *          <param name="srcXY1"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *      	<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	void sortByY2(float* srcXY0, float* srcXY1, int size);

	// TEXTURING PART
	/**
	 *  \brief Функция поэлементной сортировки данных X,Y,S,T,W вершин 0,1,2
	 *  по возрастанию Y. Для i-го входного набора вершин 0,1,2 упорядочивает 
	 *  вершины (данные вершин) в порядке возрастания координаты Y.
	 *  
	 *  \param srcXY0 [in, out] Входной массив двухмерных векторов XY0
	 *  \param srcXY1 [in, out] Входной массив двухмерных векторов XY1
	 *  \param srcXY2 [in, out] Входной массив двухмерных векторов XY2
	 *  \param srcS0  [in, out] Входной массив одномерных векторов S0
	 *  \param srcT0  [in, out] Входной массив одномерных векторов T0
	 *  \param srcS1  [in, out] Входной массив одномерных векторов S1 
	 *  \param srcT1  [in, out] Входной массив одномерных векторов T1 
	 *  \param srcS2  [in, out] Входной массив одномерных векторов S2 
	 *  \param srcT2  [in, out] Входной массив одномерных векторов T2 
	 *  \param srcW0  [in, out] Входной массив одномерных векторов W0 
	 *  \param srcW1  [in, out] Входной массив одномерных векторов W1 
	 *  \param srcW2  [in, out] Входной массив одномерных векторов W2 
	 *  \param size [in] Число элементов в массивах
	 *  \retval Return description
	 */
	void sortByY5(float* srcXY0, float* srcXY1, float* srcXY2,
		float* srcS0, float* srcT0,
		float* srcS1, float* srcT1,
		float* srcS2, float* srcT2,
		float* srcW0, float* srcW1, float* srcW2,
		int size);
	// TEXTURING PART

	/**
	 *  \defgroup split_v4nm32f split_v4nm32f
	 *  \ingroup service_api
	 *  \brief Функция разбивки 4х-мерного вектора на 4 отдельных значения(вектор на значения не бьется)
	 *  \author Жиленков Иван
	 *  
	 *  \param srcVec [in] Входной массив векторов (массив векторов?)
	 *  \param step [in] Шаг чтения входного массива (шаг в чем , в элементах v4nm32f?)
	 *  \param dstX [out] Выходной массив первых компонент вектора
	 *  \param dstY [out] Выходной массив вторых компонент вектора
	 *  \param dstZ [out] Выходной массив третьих компонент вектора
	 *  \param dstW [out] Выходной массив четвертых компонент вектора
	 *  \param countVec [in] Размер массива в элементах (в v4nm32f?)
	 *  \retval Return description (description?)
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="step"  > 2 </param>
	 *          <param name="dstX"  > im0 im1 </param>
	 *          <param name="dstY"  > im0 im1 im2 </param>
	 *          <param name="dstZ"  > im0 im1 im2 im3 </param>
	 *          <param name="dstW"  > im0 im1 im2 im3 im4 </param>
	 *          <param name="countVec"> 128 512 1014 </param>
	 *      	<size> countVec </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void split_v4nm32f(v4nm32f* srcVec, int step, float* dstX, float* dstY, float* dstZ, float* dstW, int countVec);
	 //! \}
	 
	/*!
	 *  \ingroup service_api
	 *  \brief Функция разбиения 2хмерного вектора на 2 отдельных значения
	 *  *  \author Жиленков Иван
	 *  
	 *  \param srcVec [in] Входной массив векторов
	 *  \param step [in] Шаг чтения входного массива
	 *  \param dstX [out] Выходной массив первых компонент вектора
	 *  \param dstY [out] Выходной массив первых компонент вектора
	 *  \param countVec [in] Description for countVec
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="step"> im0 im1 </param>
	 *          <param name="dstX"> im0 im1 im2 </param>
	 *          <param name="dstY"> im0 im1 im2 im3 </param>
	 *          <param name="countVec"> 128 512 1024 </param>
	 *       	<size> countVec </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void split_v2nm32f(v2nm32f* srcVec, int step, float* dstX, float* dstY, int countVec);
	//! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция вычитания вектора из постоянного значения
	 *  \author Жиленков Иван
	 *  
	 *  \param pSrcV [in] Входной массив
	 *  \param pSrcC [in] Указатель на константу
	 *  \param pDst [out] Выходной массив
	 *  \param size [in] Число элементов в массиве
	 *  
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="pSrcV"> im0 </param>
	 *          <param name="pSrcC"> im0 </param>
	 *          <param name="pDst"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *       	<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void subCRev_v4nm32f(v4nm32f* pSrcV, v4nm32f* pSrcC, v4nm32f* pDst, int size);
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Brief description
	 *  \author Жиленков Иван
	 *  
	 *  \param srcFlags [in] Description for srcFlags
	 *  \param srcVec [in] Description for srcVec
	 *  \param valueLeft [in] Description for valueLeft
	 *  \param valueRight [in] Description for valueRight
	 *  \param dstVec [in] Description for dstVec
	 *  \param size [in] Description for size
	 *  \return Return description
	 *  \details Details
	 *  
	 */
	void ternaryLt0_AddC_AddC_32f(nm32f* srcFlags, nm32f* srcVec, float valueLeft, float valueRight, float* dstVec, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция умножения трех массивов на константу
	 *  \author Жиленков Иван
	 *  
	 *  \param src1 [in] Входной массив 1
	 *  \param src2 [in] Входной массив 2
	 *  \param src3 [in] Входной массив 2
	 *  \param C [in] Умножаемая константа
	 *  \param dst1 [in] Выходной массив 1
	 *  \param dst2 [in] Выходной массив 2
	 *  \param dst3 [in] Выходной массив 3
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция умножает три входных массива на одну константу и выгружает результат в три разных массива. Если все массивы лежат в разных банках памяти,
	 *  то умножение каждого массива происходит параллельно друг другу
	 *  
	 */
	void tripleMulC_32f(float* src1, float* src2, float* src3, float C, float* dst1, float* dst2, float* dst3, int size);

	void checkRectanglesOverlaps(v2nm32f* minXY, v2nm32f* maxXY, v2nm32f* upperRightLimit, v2nm32f* lowerLeftLimit, nm1* mask, int size);

}