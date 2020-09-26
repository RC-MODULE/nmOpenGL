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

	/**
	* Признак активации текстурирования (битовая маска). Значение > 0 означает, что
	* текстурирование активировано.
	*/
	unsigned int textureEnabled;

	
	void init(){
	
		//firstFreeTexByte=NULL;
		activeTexUnit = NMGL_TEXTURE0;
		activeTexUnitIndex = 0;
		clientActiveTexUnit = NMGL_TEXTURE0;
		clientActiveTexUnitIndex = 0;
		lastTexName = 0;
		unpackAlignment = 4;
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
		
	}
	
};
#endif
