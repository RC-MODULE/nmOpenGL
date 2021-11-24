#ifndef __NMGLTEX_NM1_H__
#define __NMGLTEX_NM1_H__
#include "hal.h"
#include "hal_target.h"
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
	TexObject texObjects [(NMGL_MAX_TEX_OBJECTS)];
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
	* Признак активации текстурирования (битовая маска). Значение > 0 означает, что 
	* текстурирование активировано.
	*/
	unsigned int textureEnabled;
/**
	* Массив длин палитр для каждого текстурного объекта
	*/

	unsigned int* paletts_widths_pointers[(NMGL_MAX_TEX_OBJECTS+1)];
	
/*
Указатель на общую палитру цветов
*/
NMGLubyte * palette_pointers[(NMGL_MAX_TEX_OBJECTS+1)];//0-shared

/*
Флаг использования разделяемой палитры
*/
unsigned int palette_is_shared; 



#ifdef TEST_NMGL_TEX_FUNC
	//ref value to check the accessibility of nmpu1
	unsigned int refValue = 0xC0DEC0DE;
#endif //TEST_NMGL_TEX_FUNC
	
	void init(){	

		
		

		palette_is_shared = 0;
		activeTexUnit = NMGL_TEXTURE0;
		activeTexUnitIndex = 0;
		clientActiveTexUnit = NMGL_TEXTURE0;
		clientActiveTexUnitIndex = 0;
		segX0 = 0;
		segY0 = 0;
		segWidth = WIDTH_SEG;
		//texcoordArray
		//curTexCoords
		//unpackAlignment
		textureEnabled = 0;
		
	/*
		for (int i = 0; i < NMGL_MAX_TEX_OBJECTS+1; i++)
		{
			paletts_widths[i]=1;
			if(i == 0)
			{
				palette_pointers[i]=init_mem_palettes();
			}
			else{
				palette_pointers[i]=(NMGLubyte*)palette_pointers[i-1]+NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE;
			}
			for(int j=0;j<3;j++)
			{
				*((NMGLubyte*)palette_pointers[i]+j)=0x1;
			}
		}
	*/
//INIT_PALETTE_MEMORY_POINTERS();

//palette_pointers[0] = (NMGLubyte *)halSyncAddr(0, 0);
//paletts_widths_pointers[0] = (unsigned int*)halSyncAddr(0, 0);


DEBUG_PRINT(("got palette_pointer:0x%x",palette_pointers[0]));
DEBUG_PRINT(("got palette_width pointer:0x%x",paletts_widths_pointers[0]));
for (int i = 1; i < NMGL_MAX_TEX_OBJECTS+1; i++)
{
	palette_pointers[i] = (NMGLubyte*)palette_pointers[i-1]+NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE;
	paletts_widths_pointers[i] = (unsigned int*)((unsigned int*)paletts_widths_pointers[0]+i);
}
INIT_TEX_UNITS();
INIT_TEX_OBJECTS(); 
/*for(int i = 0; i < NMGL_MAX_TEX_OBJECTS; i++) 
{
	 //texObjects[i].palette.setColors(palette_pointers[i+1]);
	 texObjects[i].palette.colors=(NMGLubyte*)0xAAA+i;
}
*/
	}
	NMGLubyte* get_palette_by_name_p(NMGLuint name)
	{
		
		return (NMGLubyte*)(palette_pointers[(name+1)]);	
	}
	
};


namespace nm1_version {
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
	int* c0;  /**< Цвет 0 вершины треугольника. Используется для ShadeModel(SMOOTH).*/
	int* c1;  /**< Цвет 1 вершины треугольника. Используется для ShadeModel(SMOOTH).*/
	int* c2;  /**< Цвет 2 вершины треугольника. Используется для ShadeModel(SMOOTH).*/
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
\param ptrnPoints [in] Входной массив координат левого верхнего угла прямоуг. области, в которую вписан треугольник, в паттерне
\param ptrnSizes [in] Входной массив значений ширины и высоты прямоуг. области, в которую вписан треугольник
\param pSrcTriangle [in] Входной массив цветов треугольников
\param pDstTriangle [out] Выходной массив цветов треугольников
\param count [in] Число треугольников
\retval void
*/
void textureTriangle(Pattern* patterns, 
                 TrianglesInfo* triangles,
                 nm32s** pROI,
				 Vector2* ptrnPoints,
		         Size* ptrnSizes,
                 nm32s* pSrcTriangle, 
                 nm32s* pDstTriangle, 
                 int count);




//! \}
} //end of namespace nm1_version
#endif
