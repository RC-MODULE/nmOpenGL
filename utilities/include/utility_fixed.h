#pragma once
#include "utility.h"
#include "pattern.h"

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

#ifdef __cplusplus	
}
#endif //__cplusplus

