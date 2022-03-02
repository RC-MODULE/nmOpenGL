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

	typedef v4nm32u abgr32;
	typedef nm16s   rgb565;
	typedef v4nm8s  rgb8888;
	
	/*!
	 *  \ingroup service_api
	 *  \defgroup convertABGR32_RGB565 convertABGR32_RGB565 
	 *  \brief Преобразование формата ABGR32-32-32-32 в формат RGB565 
	 *  \param srcArray [in] Входной массив элементов в формате ABGR32
	 *  \param dstArray [in] Выходной массив элементов в формате RGB565
	 *  \param count [in] Количество элементов во входном массиве для обработки (должно быть кратно 4)
	 *  \return Через параметр возвращает преобразованные элементы в формате RGB565
	 *  \details Входной и выходной массивы должны вмещать N элементов, 
	 *  где N - кратно 4 и не меньше count. Если count не кратно 4, принято решение не отбрасывать элементы. 
	 *  Функция обработает ближайшее к count число, большее и кратное 4 
	 *  (например, если count = 5, то функция прочитает из входного массива, 
	 *  обработает и запишет в выходной массив 8 элементов, 12 - для count = 10)
	 *  
	 *  \par
	 *  \xmlonly
	 *  	<testperf>
	 *   		<param name=" srcArray "> im00 </param>
	 *   		<param name=" dstArray "> im10 </param>
	 *   		<param name=" count "> 4 8 12 16 20 24 28 32 36 40 128 256 512 </param>
	 *  	</testperf>
	 *  	<testperf>
	 *   		<param name=" srcArray "> im00 im10 im20 im30 </param>
	 *   		<param name=" dstArray "> im01 im11 im21 im31 </param>
	 *   		<param name=" count "> 4 </param>
	 *  	</testperf>
	 *  \endxmlonly
	 */
	//! \{
	void convertABGR32_RGB565(const abgr32  *srcArray, rgb565 *dstArray, int count);
	//! \}
	
	/*!
	 *  \ingroup service_api
	 *  \defgroup convertRGB565_RGB8888 convertRGB565_RGB8888 
	 *  \brief Преобразование формата RGB565 в формат RGB8888 
	 *  \details Количество обрабатываемых элементов должно быть кратно 4, в противном случае
	 *  оно округляется до кратного 4 в нижнюю сторону (например, если count = 5, 
	 *  то обработано будет 4 элемента).
	 *  \param srcArray [in] Входной массив элементов в формате RGB565
	 *  \param dstArray [in] Выходной массив элементов в формате RGB8888
	 *  \param count [in] Количество элементов во входном массиве для обработки (должно быть кратно 4)
	 *  
	 *  \return Через параметр возвращает преобразованные элементы в формате RGB8888
	 *  
	 *  \par
	 *  \xmlonly
	 *  	<testperf>
	 *   		<param name=" srcArray "> im00 </param>
	 *   		<param name=" dstArray "> im10 </param>
	 *   		<param name=" count "> 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 36 40 128 256 512 1024 2048 4096 </param>
	 *  	</testperf>
	 *  	<testperf>
	 *   		<param name=" srcArray "> im00 im10 im20 im30 </param>
	 *   		<param name=" dstArray "> im01 im11 im21 im31 </param>
	 *   		<param name=" count "> 4 </param>
	 *  	</testperf>
	 *  \endxmlonly
	 */
	//! \{
	void convertRGB565_RGB8888(const rgb565 *srcArray, rgb8888 *dstArray, int count);
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

