#ifndef NMGL_TEX_COMMON_H
#define NMGL_TEX_COMMON_H

#include "demo3d_common.h"
#include "nmgltype.h"
//#define DEBUG
#define DEBUG_LEVEL 0
// #include "tests.h"

/*
* Максимальное поддерживаемое количество текстурных модулей
*/
#define NMGL_MAX_TEX_UNITS 1 //TODO: это значение должно возвращаться при вызове функции glGetIntegeri с параметром NMGL_MAX_TEXTURE_UNITS


/**
* Максимальный размер изображения текстуры по ширине или по высоте в пикселях
*/
#define NMGL_MAX_TEX_SIZE 1024

/**
* Максимальный уровень детализации текстурного изображения
*/
#define NMGL_MAX_MIPMAP_LEVEL 10
                                 

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
    NMGLint level;

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
    NMGLint border;
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
    TexImage2D texImages2D[NMGL_MAX_MIPMAP_LEVEL+1];


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


#endif
