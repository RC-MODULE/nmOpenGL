#pragma once
#include "utility.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	

	/*!
	 *  \ingroup service_api
	 *  \brief Функция нахождения отрицательных смещений
	 *  \author Жиленков Иван
	 *  
	 *  \param src [in] Входной массив
	 *  \param dst [out] Выходной массив
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция вычисляет следующее выражение
	 *  \f[f(x)= 
	 *  \begin{cases}
	 *  |x| & x < 0 \\
	 *  0 & x >= 0
	 *  \end{cases}
	 *  \f]
	 *  
	 */
	void absIfNegElse0(int* src, int* dst, int size);

	/*!
	 *  \ingroup service_api
	 *  \brief Функции массового смещения адресов c одного и того же базового адреса с разными смещениями
	 *  \author Жиленков Иван
	 *  
	 *  \param baseAddr [in] Входной массив адресов
	 *  \param offsets [in] Смещения
	 *  \param ppDst [in] Выходной массив адресов
	 *  \param size [in] Число элементов
	 *  
	 *  
	 */
	 //! \{
	void baseAddrOffs_32s(nm32s* baseAddr, int* offsets, nm32s** ppDst, int size);
	void baseAddrOffs_32u(nm32u* baseAddr, int* offsets, nm32u** ppDst, int size);
	void baseAddrOffs_16s(nm16s* baseAddr, int* offsets, nm16s** ppDst, int size);
	void baseAddrOffs_16u(nm16u* baseAddr, int* offsets, nm16u** ppDst, int size);
	 //! \}

	/*!
	 *  \ingroup service_api
	 *  \brief Функция копирования нескольких кусков.
	 *  \author Жиленков Иван
	 *  \warning Максимальный размер каждого куска не должен превышать 64 32-разрядных слов
	 *  
	 *  \param ppSrc [in] Массив указателей входных данных
	 *  \param ppDst [in] Массив указателей выходных данных
	 *  \param size [in] Массив размеров данных (в 32-разрядных словах)
	 *  \param count [in] Число копирований
	 *  
	 *  
	 */
	void copyPacket_32s(nm32s** ppSrc, nm32s** ppDst, int* size, int count);

	
	/*!
	 *  \ingroup service_api
	 *  \brief Суммирования всех элементов
	 *  \author Жиленков Иван
	 *  
	 *  \param pVec [in] Массив значений
	 *  \param size [in] Число элементов
	 *  \retval Сумма всех заданных чисел
	 *  
	 *  \details Функция считает \f[ f=\Sigma_{i = 0}^n i\f]
	 *  
	 */
	int totalSum(nm32s* pVec, int size);

	/*!
	 *  \ingroup service_api
	 *  \brief Brief description
	 *  
	 *  \param srcArray [in] Description for srcArray
	 *  \param dstArray [in] Description for dstArray
	 *  \param count [in] Description for count
	 *  
	 *  \details Details
	 *  
	 */
	void inverse_v4nm8u(const v4nm8u *srcArray, v4nm8u *dstArray, int count);

#ifdef __cplusplus	
}
#endif //__cplusplus

