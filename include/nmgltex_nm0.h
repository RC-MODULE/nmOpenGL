#ifndef __NMGLTEX_NM0_H__
#define __NMGLTEX_NM0_H__

#include "demo3d_common.h"
#include "nmgltex_common.h"
struct NMGL_Context_NM0_Texture {
	
	/*
		Указатель на первый свободный байт текстурной памяти	
	*/
	//void* firstFreeTexByte;
	/**
	* Массив текстурных модулей
	*/
	TexUnit texUnits [NMGL_MAX_TEX_UNITS];

	/**
	* Массив текстурных объектов.
	*/
	TexObject texObjects [NMGL_MAX_TEX_OBJECTS];
/**
	* Определяет имена для текстурных объектов, которые можно использовать.
	*	Имена генерируются путём инкрементации данной переменной на 1 для каждого имени
	* 0   - ещё не было сгенерировано ни одного имени;при этом объект и именем 0 создаётся
	* по умолчанию при инициализации контекста
	* n>0 - содержит последнее сгенерированное имя (а также общее количество имён).
	* При этом подразумеваются, что имена генерируются только с помощью функции genTextures
	*/
	unsigned int lastTexName;	
	/**
	* Массивы текстурных координат
	*/
	Array texcoordArray[NMGL_MAX_TEX_UNITS];	
	/**
	* Текущие текстурные координаты
	*/
	TexCoords curTexCoords [NMGL_MAX_TEX_UNITS];
	
	/**
	*  Имя активного текстурного модуля.
	*/
	NMGLenum activeTexUnit;
	/**
	*  Индекс активного текстурного модуля в массиве текстурных модулей.
	*  Используется для доступа к активному текстурному модулю
	*/
	unsigned int activeTexUnitIndex;
	
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
	*  Устанавливается в функции glPixelStore и используется при чтении данных из загруженного изображения текстуры.
	*  По спецификации требуется.
	*/
	unsigned int unpackAlignment;
	
	void init(){
	
		//firstFreeTexByte=NULL;
		activeTexUnit = NMGL_TEXTURE0;
		activeTexUnitIndex = 0;
		clientActiveTexUnit = NMGL_TEXTURE0;
		clientActiveTexUnitIndex = 0;
		lastTexName = 0;
		for (int i = 0; i < NMGL_MAX_TEX_UNITS; i++)
		{
			texcoordArray[i].pointer = 0;
			texcoordArray[i].size = 4;
			texcoordArray[i].stride = 0;
			texcoordArray[i].type = NMGL_FLOAT;
			texcoordArray[i].enabled = NMGL_FALSE;
		}
//==========TEX_UNITS_INIT====================================
		INIT_TEX_UNITS();
//==========TEX_OBJ_INIT====================================
		INIT_TEX_OBJECTS();
		unpackAlignment=4;
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
	float* x0; /**< Оконная координата x 0-ой вершины треугольника */
	float* y0; /**< Оконная координата y 0-ой вершины треугольника */
	float* x1; /**< Оконная координата x 1-ой вершины треугольника */
	float* y1; /**< Оконная координата y 1-ой вершины треугольника */
	float* x2; /**< Оконная координата x 2-ой вершины треугольника */
	float* y2; /**< Оконная координата y 2-ой вершины треугольника */
	float* z0;  /**< Координата w0 треугольника */
	float* z1;  /**< Координата w1 треугольника */
	float* z2;  /**< Координата w2 треугольника */
	float* s0; /**< Текстурная координата s 0-ой вершины треугольника */
	float* t0; /**< Текстурная координата t 0-ой вершины треугольника */
	float* s1; /**< Текстурная координата s 1-ой вершины треугольника */
	float* t1; /**< Текстурная координата t 1-ой вершины треугольника */
	float* s2; /**< Текстурная координата s 2-ой вершины треугольника */
	float* t2; /**< Текстурная координата t 2-ой вершины треугольника */
	v4nm32s* colors; /**< Цвет треугольника (один на три вершины) */
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

\param triangles [in] Входной массив параметров треугольников
\param pDstTriangle [out] Выходной массив цветов треугольников
\param count [in] Число треугольников
\retval void
*/
void textureTriangle(TrianglesInfo* triangles, nm32s* pDstTriangle, int count);


extern "C"{
void abs_32f(float* src, float* dst, int size);
void findMax2(float* src1, float* src2, float* dstMax, int size);
}

//! \}


#endif
