#ifndef NMGL_TEX_COMMON_H
#define NMGL_TEX_COMMON_H

#include "demo3d_common.h"
#include "nmgltype.h"
//Memory
//------------------------CUSTOMIZE-THIS-ONLY-----------------------------------------------------------------
#define NMGL_MAX_MIPMAP_LVL               10  //square textures only; 1024 width/height
//#define BYTES_IN_CHAR 
#define RGBA_TEXEL_SIZE_UBYTE              4
#define RGB_TEXEL_SIZE_UBYTE               3
#define LUMINANCE_ALPHA_TEXEL_SIZE_UBYTE   2
#define LUMINANCE_TEXEL_SIZE_UBYTE         1
#define ALPHA_TEXEL_SIZE_UBYTE             1                      
#define COLOR_TEXEL_SIZE_UBYTE             1                      
//-------------------------NOT-THIS----------------------------------------------------------------------------
#define NMGL_MAX_TEX_SIDE (0x1<<(NMGL_MAX_MIPMAP_LVL))
#define UBYTES_PER_TEXEL RGBA_TEXEL_SIZE_UBYTE   //each byte of texel is placed in 4-byte var (because char is 4-byte variable)
#define MIPMAP_OBJ_SIZE ((((0x1<<(2*(NMGL_MAX_MIPMAP_LVL+1)))-1)/3)*UBYTES_PER_TEXEL) //mipmap mem size in 4byte words	
#define MIPMAP_MEM_SIZE ((MIPMAP_OBJ_SIZE)*NMGL_MAX_TEX_OBJECTS) //mipmap mem size in NMGLubyte	
//EXT_paletted_texture
#define NMGL_MAX_PALETTE_WIDTH (0x1<<8)

 

//============================================================================================================
int getTexelSizeUbytes(NMGLint format);
//============================================================================================================
//#define DEBUG
#define DEBUG_LEVEL 1
// #include "tests.h"
/*
* Максимальное поддерживаемое количество текстурных модулей
*/
#define NMGL_MAX_TEX_UNITS 1 //TODO: это значение должно возвращаться при вызове функции glGetIntegeri с параметром NMGL_MAX_TEXTURE_UNITS


/**
* Максимальный размер изображения текстуры по ширине или по высоте в пикселях
*/
//#define NMGL_MAX_TEX_SIZE 1024

/**
* Максимальный уровень детализации текстурного изображения
*/
//#define NMGL_MAX_MIPMAP_LEVEL 10
                                 

/**
* Максимальное количество текстурных объектов.
*/
#define NMGL_MAX_TEX_OBJECTS 4


/**
* Максимальный уровень детализации текстуры
*/
#define NMGL_TEX_MIN_LOD -1000

/**
* Минимальный уровень детализации текстуры
*/
#define NMGL_TEX_MAX_LOD 1000

/**
* Минимальное значение аргумента level функции nmglTexImage2D
*/
#define NMGL_TEX_BASE_LEVEL 0

/**
* Максимальное значение аргумента level функции nmglTexImage2D
*/
#define NMGL_TEX_MAX_LEVEL 1000



/**
*  Структура для хранения изображения текстуры, загружаемой функцией nmglTexImage2D
*/
struct TexImage2D {

    /**
    * Внутренний формат представления данных текстурного изображения. Значение аргумента internalformat функции nmglTexImage2D.
    */
    NMGLint internalformat;

    /**
    * Уровень детализации текстурного изображения. Значение аргумента level функции nmglTexImage2D.
    */
    //NMGLint level;

    /**
    * Указатель на данные текстурного изображения, загружаемого с помощью функции nmglTexImage2D.
    */
    void* pixels;

    /**
    * Ширина текстурного изображения. Значение аргумента width функции nmglTexImage2D.
    */
    NMGLsizei width;

    /**
    * Высота текстурного изображения. Значение аргумента height функции nmglTexImage2D.
    */
    NMGLsizei height;

    /**
    * Ширина границы текстурного изображения. Значение аргумента border функции nmglTexImage2D.
    * Должно быть равно 0, поэтому можно, в принципе не использовать.
    */
    //NMGLint border;
};
/**
*   Палитра цветов
*/
struct EXT_palette
{
    NMGLubyte *colors;
    NMGLint width;
};
/**
*   Текстурный объект
*/
struct TexObject{

    /**
    *   Имя текстурного объекта
    */
    NMGLuint name;

    /**
    *   Тип текстуры (может принимать значение только NMGL_TEXTURE_2D) TODOma:возможно, стоит убрать
    */
    NMGLenum target;

    /**
    *   Параметры обработки границ по оси s
    */
    NMGLint texWrapS;

    /**
    *   Параметры обработки границ по оси t
    */
    NMGLint texWrapT;

    /**
    *   Режим фильтрации при уменьшении текстуры
    */
    NMGLint texMinFilter;
        //OpenGL SC 1.0.1 Spec Supported:
    /**
    *   Режим фильтрации при увеличении текстуры
    */
    NMGLint texMagFilter;

    /**
    *   Массив текстурных изображений различных уровней детализации
    */
    TexImage2D texImages2D[NMGL_MAX_MIPMAP_LVL+1];
    /**
    *  Указатель на палитру цветов 
    */
    EXT_palette palette;

    NMGLubyte init_palette[RGBA_TEXEL_SIZE_UBYTE];
    /*
    * Флаг, определяющий, было ли задано изображение (через glTexImage2D):
    * 0 - glTexImage2D ещё не вызывалась
    * 1 - glTexImage2D уже вызывалась
    */
    int imageIsSet; // Флаг =0 - изображение ещё не было загружено =1 - изображение уже было загружено

};

/**
*   Текстурный модуль
*/
struct TexUnit {

    /**
    *   Признак активации текстурного модуля (включен/выключен)
    */
    NMGLboolean enabled;

    /**
    *   Цвет текстурного окружения
    */
    NMGLfloat texEnvColor [4];

    /**
    *  Функция текстурирования (указатель)
    */
    void* texFunction;

    /**
    *  Функция текстурирования (имя)
    */
    NMGLint texFunctionName;


    /**
    *   Привязанный текстурный объект
    */
    TexObject* boundTexObject;
};

/**
*  Структура для хранения значений текстурных координат
*/
struct TexCoords {

    NMGLfloat s;
    NMGLfloat t;
};

/**
*	Структура для передачи значения цвета текстурного окружения c nmpu0 на nmpu1
*/
typedef union Intfloat {
	int i;
	float f;
} Intfloat;
//----------------------------FUNCTIONS----------------------------------------------------------------------------------
#define INIT_TEX_UNITS() for (int i = 0; i < NMGL_MAX_TEX_UNITS; i++) \
{                                                                     \
    texUnits[i].enabled=NMGL_FALSE;                                   \
    texUnits[i].texFunction=NULL;                                     \
    texUnits[i].texFunctionName=0;                                    \
    texUnits[i].boundTexObject=texObjects;                            \
    for (int j = 0; j < 4; j++)                                       \
    {                                                                 \
        texUnits[i].texEnvColor[j]=0.0;                               \
    }                                                                 \
}


#define INIT_TEX_OBJECTS() for(int i = 0; i < NMGL_MAX_TEX_OBJECTS; i++)  \
{                                                                       \
            texObjects[i].name=i;                                       \
			texObjects[i].target=NMGL_TEXTURE_2D;                       \
			texObjects[i].texWrapS=NMGL_REPEAT;                         \
			texObjects[i].texWrapT=NMGL_REPEAT;                         \
			texObjects[i].texMinFilter=NMGL_NEAREST_MIPMAP_LINEAR;      \
			texObjects[i].texMagFilter=NMGL_LINEAR;                     \
			texObjects[i].imageIsSet=0;                                 \
			texObjects[i].init_palette[0]=0x1;                          \
			texObjects[i].init_palette[1]=0x1;                          \
			texObjects[i].init_palette[2]=0x1;                          \
			texObjects[i].init_palette[3]=0x1;                          \
            texObjects[i].palette.width=1;                              \
            texObjects[i].palette.colors=&texObjects[i].init_palette[0];\
			for(int j=0;j<=NMGL_MAX_MIPMAP_LVL;j++)                     \
            {                                                           \
            texObjects[i].texImages2D[j].pixels=NULL;                   \
				texObjects[i].texImages2D[j].internalformat=NMGL_RGBA;  \
				texObjects[i].texImages2D[j].width=0;                   \
                texObjects[i].texImages2D[j].height=0;                  \
                }			                                            \
		}

//===================PRINTING==============================================================

#endif
