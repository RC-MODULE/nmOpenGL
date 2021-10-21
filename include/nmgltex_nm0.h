#ifndef __NMGLTEX_NM0_H__
#define __NMGLTEX_NM0_H__

#include "demo3d_common.h"
#include "nmgltex_common.h"
#include "hal.h"
#include "hal_target.h"

//extern NMGLubyte* init_mem_palettes();
//extern unsigned int* get_palette_width_p(NMGLint name);

extern NMGLubyte* palettes_p; // texture palette memory
extern unsigned int* palettes_widths_p;


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
	* Массив длин палитр для каждого текстурного объекта
	*/

	//unsigned int paletts_widths[(NMGL_MAX_TEX_OBJECTS+1)];
	unsigned int* paletts_widths_pointers[(NMGL_MAX_TEX_OBJECTS+1)];

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
    
	/**
	* Признак активации текстурирования (битовая маска). Значение > 0 означает, что
	* текстурирование активировано.
	*/
	unsigned int textureEnabled;

	/**
	* Flag
	*/
	NMGLboolean using_shared_palette;
	
	//NMGLubyte * palettes_pointer;
	NMGLubyte * palette_pointers[(NMGL_MAX_TEX_OBJECTS+1)];//0-shared


	void init(){
	
		//firstFreeTexByte=NULL;
		//palettes_pointer = init_mem_palettes();
		using_shared_palette = NMGL_FALSE;
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
palette_pointers[0] = palettes_p;
paletts_widths_pointers[0] = palettes_widths_p;

for (int i = 1; i < NMGL_MAX_TEX_OBJECTS+1; i++)
{
	palette_pointers[i] = (NMGLubyte*)palette_pointers[i-1]+NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE;
	paletts_widths_pointers[i] = (unsigned int*)((unsigned int*)paletts_widths_pointers[0]+i);
}
//halSyncAddr(palette_pointers[0], 1);
//halSyncAddr(paletts_widths_pointers[0], 1);

//==========TEX_UNITS_INIT====================================
		INIT_TEX_UNITS();
//==========TEX_OBJ_INIT====================================
		INIT_TEX_OBJECTS();
		unpackAlignment=4;
		textureEnabled = 0;
	}
	void use_shared_palette()
	{
		for (int i = 0; i < NMGL_MAX_TEX_OBJECTS; i++)
		{
			//texObjects[i].palette.setColors(get_shared_palette_p());
			texObjects[i].palette.colors=get_shared_palette_p();
			//texObjects[i].palette.setWidth_p(&paletts_widths[0]);
			texObjects[i].palette.width = paletts_widths_pointers[0];
		}
	}
	void use_local_palettes()
	{
		for (int i = 0; i < NMGL_MAX_TEX_OBJECTS; i++)
		{
			texObjects[i].palette.colors = get_palette_by_name_p(texObjects[i].name);
			texObjects[i].palette.width = paletts_widths_pointers[(texObjects[i].name+1)];
		}
	}
	NMGLubyte* get_shared_palette_p()
	{
		return palette_pointers[0];
	}
	NMGLubyte* get_palette_by_name_p(NMGLuint name)
	{
		
		return (NMGLubyte*)(palette_pointers[(name+1)]);
	}
};

namespace nm0_version {
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
  v4nm32s* colors; /**< Цвет треугольника (один на три вершины). Используется для текстурирования. */
	v4nm32s* c0;  /**< Цвет 0 вершины треугольника. Используется для ShadeModel(SMOOTH).*/
	v4nm32s* c1;  /**< Цвет 1 вершины треугольника. Используется для ShadeModel(SMOOTH).*/
	v4nm32s* c2;  /**< Цвет 2 вершины треугольника. Используется для ShadeModel(SMOOTH).*/
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
} //endof namespace nm0_version

extern "C"{
void abs_32f(float* src, float* dst, int size);
void findMax2(float* src1, float* src2, float* dstMax, int size);
}

//! \}


#endif
