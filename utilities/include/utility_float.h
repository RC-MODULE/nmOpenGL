#pragma once
#include "utility.h"

#ifdef __cplusplus
extern "C"{
#endif //_cplusplus

	void abs_32f(float* src, float* dst, int size);

	void absIfNegElse0_32f(float* src, float* dst, int size);

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

	void checkRectanglesOverlaps(v2nm32f* minXY, v2nm32f* maxXY, v2nm32f* upperRightLimit, v2nm32f* lowerLeftLimit, nm1* mask, int size);

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
#ifdef __cplusplus
}
#endif //_cplusplus