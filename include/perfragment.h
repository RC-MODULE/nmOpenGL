#ifndef PERFRAGMENT_H 
#define PERFRAGMENT_H 
//Функции в этом заголовочном файле пока что не имеют реализациии. Этот заголовочный файл описывает интерфейс для возможного улучшения
//процесса растеризации

//FB - framebuffer
//fbrw - framebuffer read-write
//fbr - framebuffer read
//ScissorTest - тест ножниц, отрисовывает полигоны только в заданном прямоугольнике (если включен)
//AlphaTest - тест по значению альфа-компоненты цвета (если включен)
//StencilTest - трафаретный тест (если включен)
//DepthTest - тест глубины (Z-буфер) (если включен)
//Blending - смешивание цветов фрагмента с цветами изображения (если включено)

//Per-Fragment Operations OpenGL

//Fragment -> PixelOwnershipTest -> ScissorTest -> AlphaTest -> StencilTest(fbrw) -> DepthTest(fbrw) -> Blending(fbr) -> 
// -> Dithering -> LogicOp(fbr) -> to FB



//В этой реализации Scissor Test не выполняется, так как предполагается, что он уже сделан на более раннем этапе, 
//а этапы Dithering и LogicOp не обязательны в реализации OpenGL SC 1.0.1, следовательно Blending 
//можно объединить с записью в буфер цветов, поэтому получается следующий алгоритм

//Fragment -> AlphaTest -> StencilTest(fbrw) -> DepthTest(fbrw) -> Blending(fbrw)

#include "demo3d_nm1.h"
extern "C" {
	
	/**
	 *  \brief Общая функция операции с фрагментом
	 *  
	 *  \param patterns [in] Двуразрядные паттерны фигур
	 *  \param ptrnWindowXY [in] Координаты нижнего левого угла необходимого фрагмента паттернов
	 *  \param ptrnWindowSize [in] Размер необходимого фрагмента паттернов
	 *  \param leftBottomPoints [in] Кординаты нижнего левого угла фрагментов в буферах
	 *  \param valuesC [in] Значения цветов фрагментов
	 *  \param valuesZ [in] Значения z-координат фрагментов для теста глубины
	 *  \param count [in] Число паттернов
	 *  \retval Return description
	 *  \details Details
	 */
	void perFragmentOperate(Pattern* patterns, 
							Vector2* ptrnWindowXY, 
							Size* ptrnWindowSize, 
							Vector2* leftBottomPoints,
							rgb8888* valuesC,
							int* valuesZ,
							int count);
							

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
	 *  \details Функция берет заданное окно из двуразрядного паттерна и умножает его на константу. Получившиеся фрагменты располагаются в памяти подряд
	 */
	void ptrnMulC8(Pattern* patterns, Vector2* windowXY, Size* windowSize, int* valuesC, nm8s* dstFragments, int count);
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
	 *  \brief Функция копирования фрагментов цвета в буфер цвета (без смешивания)
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
	
	/**
	 *  \brief Функции сравнения значений трафаретного буфера с референсным значением
	 *  
	 *  \param stencilPoints [in] Адреса нижних левых углов окон трафаретного буфера соответствующих фрагменту
	 *  \param buffStride [in] Ширина трафаретного буфера
	 *  \param stencilFrags [in] Входной массив фрагментов
	 *  \param fragSizes [in] Размеры фрагментов
	 *  \param stencilFailMasks [out] Выходная маска пикселей, не прошедших тест
	 *  \param stencilPassMasks [out] Выходная маска пикселей, прошедших тест
	 *  \param count [in] Число фрагментов
	 *  \retval Return description
	 *  \details Details
	 */
	void stencilComp8 (nm8s**  stencilPoints, int buffStride, nm8s*  stencilFrags, Size* fragSizes, nm8s*  stencilFailMasks, nm8s*  stencilPassMasks, int count);	
	void stencilComp32(nm32s** stencilPoints, int buffStride, nm32s* stencilFrags, Size* fragSizes, nm32s* stencilFailMasks, nm32s* stencilPassMasks, int count);
	
	/**
	 *  \brief Функция трафаретного тестирования без включенного теста глубины
	 *  
	 *  \param stencilPoints [in] Адреса нижних левых углов окон трафаретного буфера соответствующих фрагменту
	 *  \param buffStride [in] Ширина трафаретного буфера
	 *  \param stencilMask [in] Трафаретная маска фрагментов
	 *  \param maskSizes [in] Размеры фрагментов
	 *  \param count [in] Число фрагментов
	 *  \retval Функция модифицирует трефаретный буфер в соответств
	 *  \details Details
	 */
	void stencilTest8 (nm8s**  stencilPoints, int buffStride, nm8s*  stencilFailMask, nm8s*  stencilPassMask, Size* maskSizes, int count);
	void stencilTest32(nm32s** stencilPoints, int buffStride, nm32s* stencilFailMask, nm32s* stencilPassMask, Size* maskSizes, int count);
	
	
	void stencilTestWithDepth8(nm8s** stencilPoints, int buffStride, nm8s*  stencilFailMask, nm8s*  stencilPassMask, nm8s* depthMask, Size* maskSizes, int count);
	void stencilTestWithDepth32(nm32s** stencilPoints, int buffStride, nm32s* stencilFailMask, nm32s* stencilPassMask, nm32s* depthMask, Size* maskSizes, int count);
	
	
	void applyMasks32(nm32s* mainMask, nm32s* alphaMask, nm32s* depthMask, nm32s* stencilMask, nm32s* resultMask, int totalSize);
	void applyMasks8(nm8s* mainMask, nm8s* alphaMask, nm8s* depthMask, nm8s* stencilMask, nm8s* resultMask, int totalSize);

}

#endif