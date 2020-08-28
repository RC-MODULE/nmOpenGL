#ifndef __NMGLTEX_NM1_H__
#define __NMGLTEX_NM1_H__

#include "nmgltex_common.h"

struct NMGL_Context_NM1_Texture {

	/**
	 * Координата x левого нижнего угла обрабатываемого сегмента кадрового буфера
	 */
    int segX0;
	
	/**
	 * Координата y левого нижнего угла обрабатываемого сегмента кадрового буфера
	 */
    int segY0;

	/**
	 * Ширина обрабатываемого сегмента кадрового буфера
	 */
 	int segWidth;

	/**
	*  Индекс активного текстурного модуля в массиве текстурных модулей.
	*  Используется для доступа к активному текстурному модулю
	*/
	unsigned int activeTexUnitIndex;    
	/**
	* Массив текстурных модулей
	*/
	TexUnit texUnits [NMGL_MAX_TEX_UNITS];
	/**
	* Массив текстурных объектов.
	*/
	TexObject texObjects [NMGL_MAX_TEX_OBJECTS];
	/**
	* Массивы текущих текстурных координат.
	* Тип Array определен в demo3d_common.h
	*/
	Array texcoordArray [NMGL_MAX_TEX_UNITS];
	/**
	* Текущие текстурные координаты
	*/
	TexCoords curTexCoords [NMGL_MAX_TEX_UNITS];
	/**
	*  Имя активного текстурного модуля.
	*/
	NMGLenum activeTexUnit;
	/**
	*  Имя клиентского активного текстурного модуля.
	*/
	NMGLenum clientActiveTexUnit;
	/**
	*  Индекс клиентского активного текстурного модуля в массиве текстурных модулей.
	*  Используется для доступа к клиентскому активному текстурному модулю
	*/
	unsigned int clientActiveTexUnitIndex;
	/**
	*  Количество байт, по которому выровнены данные в загруженном текстурном изображении.
	*  Устанавливается в функции glPixelStore и испльзуется при чтении данных из загруженного изображения текстуры.
	*  По спецификации требуется.
	*/
	unsigned int unpackAlignment;

	/**
	* Признак активации текстурирования (битовая маска). Значение > 0 означает, что 
	* текстурирование активировано.
	*/
	unsigned int textureEnabled;
	
	void init(){	
		activeTexUnit = NMGL_TEXTURE0;
		activeTexUnitIndex = 0;
		clientActiveTexUnit = NMGL_TEXTURE0;
		clientActiveTexUnitIndex = 0;
		//texcoordArray
		//curTexCoords
		//unpackAlignment
		INIT_TEX_UNITS();
		INIT_TEX_OBJECTS();
	}
	
};

/**
\defgroup textureFunctions Функции текстурирования
\brief Функции текстурирования для nmOpenGL.

Функции и структуры данных для реализации текстурирования в составе nmOpenGL.
*/
//! \{

/**
\brief Структура для хранения параметров треугольников
*/
struct TrianglesInfo{
    //Поля структуры для коммита 2fbdfcc0 nmOpenGLs
	float* x0; /**< Оконная координата x 0-ой вершины треугольника */
	float* y0; /**< Оконная координата y 0-ой вершины треугольника */
	float* x1; /**< Оконная координата x 1-ой вершины треугольника */
	float* y1; /**< Оконная координата y 1-ой вершины треугольника */
	float* x2; /**< Оконная координата x 2-ой вершины треугольника */
	float* y2; /**< Оконная координата y 2-ой вершины треугольника */
	float* z0;  /**< Координата z0 треугольника */ //TODO: Needed float z in camera space, so float value
	float* z1;  /**< Координата z1 треугольника */ //TODO: Needed float z in camera space, so float value
	float* z2;  /**< Координата z2 треугольника */ //TODO: Needed float z in camera space, so float value
	v4nm32s* colors; /**< Цвет треугольника (один на три вершины) */
	int size;   /**< Количество треугольников */
	int maxSize; /**< Максимальное обрабатываемое количество треугольников */
    
    //TODO: необходимо добавить в структуру Triangles (include\demo3d_nm0.h)
	float* s0; /**< Текстурная координата s 0-ой вершины треугольника */
	float* t0; /**< Текстурная координата t 0-ой вершины треугольника */
	float* s1; /**< Текстурная координата s 1-ой вершины треугольника */
	float* t1; /**< Текстурная координата t 1-ой вершины треугольника */
	float* s2; /**< Текстурная координата s 2-ой вершины треугольника */
	float* t2; /**< Текстурная координата t 2-ой вершины треугольника */
};

/** 
\brief Функция для вычисления значения edge-функции.

Функции edgeFunction вычисляет значение edge-функции для заданных точек.

\param triangles [in] Входной массив параметров треугольников
\param x0 [in] Координата x точки 0
\param y0 [in] Координата x точки 0
\param x1 [in] Координата x точки 1
\param y1 [in] Координата x точки 1
\param x2 [in] Координата x точки 2
\param y2 [in] Координата x точки 2
\param res [out] Результат вычисления функции
\retval void
*/
void edgeFunction(float x0, float y0, float x1, float y1, float x2, float y2, float* res);

/** 
\brief Функция текстурирования треугольника/ов.

Функции texTriangle выполняет текстурирования заданного количества треугольников в соответствии с заданными параметрами текстурирования.

\param patterns [in] Входной массив масок треугольников
\param triangles [in] Входной массив параметров треугольников
\param pROI [in] Массив адресов окон треугольников в цветовом буфере
\param windows [in] Входной массив параметров размещения треугольников в сегменте
\param pSrcTriangle [in] Входной массив цветов треугольников
\param pDstTriangle [out] Выходной массив цветов треугольников
\param count [in] Число треугольников
\retval void
*/
void textureTriangle(Pattern* patterns, 
                 TrianglesInfo* triangles,
                 nm32s** pROI,
                 Rectangle* windows, 
                 nm32s* pSrcTriangle, 
                 nm32s* pDstTriangle, 
                 int count);




//! \}

#endif
