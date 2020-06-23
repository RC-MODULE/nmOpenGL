#ifndef PERFRAGMENT_H 
#define PERFRAGMENT_H 
//Функции в этом заголовочном файле пока что не имеют реализациии. Этот заголовочный файл описывает интерфейс для возможного улучшения
//процесса растеризации

#include "demo3d_nm1.h"
extern "C" {

	/**
	 *  \brief Функция умножения паттерна на константы
	 *  
	 *  \param patterns [in] Входной массив паттернов
	 *  \param windowXY [in] Координаты левого нижнего угла окна фрагмента
	 *  \param windowSize [in] Размеры окна фрагмента
	 *  \param valuesC [in] Значения констант
	 *  \param dstFragments [out] Выходной массив фрагментов
	 *  \param count [in] Число паттернов
	 *  \retval Return description
	 *  \details Функция берет заданное окно из двуразрядного паттерна и умножает его на константу. Получившиеся 32-разрядные фрагменты располагаются в памяти подряд
	 */
	void ptrnMulC32(Pattern* patterns, Vector2* windowXY, Size* windowSize, int* valuesC, nm32s* dstFragments, int count);
	
	/**
	 *  \brief Функция умножение паттерна на вектор цвета
	 *  
	 *  \param patterns [in] Входной массив паттернов
	 *  \param windowXY [in] Координаты левого нижнего угла окна фрагмента
	 *  \param windowSize [in] Размеры окна фрагмента
	 *  \param valuesC [in] Значения векторов цвета
	 *  \param dstFragments [out] Выходной массив фрагментов
	 *  \param count [in] Число паттернов
	 *  \retval Return description
	 *  \details Функция берет заданное окно из двуразрядного паттерна и умножает его на вектор цвета. Получившиеся цветовые фрагменты располагаются в памяти подряд
	 */
	void ptrnMulRgb8888(Pattern* patterns, Vector2* windowXY, Size* windowSize, rgb8888* valuesC, rgb8888* dstFragments, int count);

	/**
	 *  \brief Функция производит альфа-тестирование фрагментов изображения.
	 *  
	 *  \param fragments [in] Входные фрагменты изображения расположенные в памяти подряд
	 *  \param dstAlphaMask [in] Результаты альфа-тестирования(<0 - тест пройден, >=0 - тест не пройден)
	 *  \param totalSize [in] Суммарный размер фрагментов изображения в 32-разрядных словах
	 *  \retval Return description
	 *  \details Функция прогоняет фрагменты изображения в формате RGB8888 через альфа-тест. На выходе получается маска, которую можно будет 
	 *  затем применить на фрагменты изображения. Значения референсного значения альфа и текущий режим тестирования определяется 
	 *  внутренними переменными функции, доступ к которым производится другим способом.
	 */
	void alphaTest(rgb8888* fragments, nm32s* dstAlphaMask, int totalSize);
	
	/**
	 *  \brief Функция производит тест глубины для фрагментов изображения
	 *  
	 *  \param zBuffPoints [in] Адреса нижних левых углов окон буфера глубины оответствующих фрагменту
	 *  \param zBuffStride [in] Ширина буфера глубины
	 *  \param fragments [in] Входной массив фрагментов
	 *  \param fragSizes [in] Размеры фрагментов
	 *  \param dstDepthMask [out] Результаты теста глубины(<0 - тест пройден, >=0 - тест не пройден)
	 *  \param count [in] Число фрагментов
	 *  \retval Return description
	 *  \details Функция сравнивает предварительные фрагменты буфера глубины с самим буфером глубины. На выходе получается маска,
	 *  которую можно будет затем применить на фрагменты изображения. Текущий режим тестирования и возможность перезаписи буфера глубины
	 *  определяется внутренними переменными функции, доступ к которым производится другим способом
	 */
	void depthTest32(nm32s** zBuffPoints, int zBuffStride, nm32s* fragments, Size* fragSizes, nm32s* dstDepthMask, int count);
	
	
	/**
	 *  \brief Функция смешивания фрагментов изображения
	 *  
	 *  \param imagePoints [in] Адреса нижних левых углов окон буфера цветов соответствующих фрагменту
	 *  \param imageStride [in] Ширина изображения
	 *  \param fragments [in] Входной массив фрагментов
	 *  \param fragMask [in] Итоговые маски фрагментов изображения
	 *  \param fragSizes [in] Размеры фрагментов
	 *  \param count [in] Число фрагментов
	 *  \retval Return description
	 *  \details Функция смешивает пиксели фрагментов изображения с соответствующими окнами буфера цветов и с одновременным маскированием
	 *  записывает результат в буфер цветов. Режим смешивания определяется внутренними переменными функции, доступ к которым производится
	 *  другим способом.
	 */
	void blendRgb8888(rgb8888** imagePoints, int imageStride, rgb8888* fragments, nm32s* fragMask, Size* fragSizes, int count);
	
	/**
	 *  \brief Функция копирования фрагментов цвета в буфер цвета
	 *  
	 *  \param imagePoints [in] Адреса нижних левых углов окон буфера цветов соответствующих фрагменту
	 *  \param imageStride [in] Ширина изображения
	 *  \param fragments [in] Входной массив фрагментов
	 *  \param fragMask [in] Итоговые маски фрагментов изображения
	 *  \param fragSizes [in] Размеры фрагментов
	 *  \param count [in] Число фрагментов
	 *  \retval Return description
	 *  \details Функция копирует фрагменты буфера цветов в буфер цветов с наложение маски фрагментов
	 */
	void copyToFrameBufferRgb8888(rgb8888** imagePoints, int imageStride, rgb8888* fragments, nm32s* fragMask, Size* fragSizes, int count);
	
	//void preStencilTest();

	//void applyMasks(nm32s* alphaMask, nm32s* depthMask));

}

#endif