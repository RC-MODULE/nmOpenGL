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

	void checkRectanglesOverlaps(v2nm32f* minXY, v2nm32f* maxXY, v2nm32f* upperRightLimit, v2nm32f* lowerLeftLimit, nm1* mask, int size);

#ifdef __cplusplus
}
#endif //_cplusplus