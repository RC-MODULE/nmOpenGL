#ifndef __NMGLTEX_NM1_H__
#define __NMGLTEX_NM1_H__

#include "nmgltex_common.h"

struct NMGL_Context_NM1_Texture {
	
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
	* Определяет имена для текстурных объектов, которые можно использовать.
	*	Имена генерируются путём инкрементации данной переменной на 1 для каждого имени
	* 0   - ещё не было сгенерировано ни одного имени;
	* n>0 - содержит последнее сгенерированное имя (а также общее количество имён).
	* При этом подразумеваются, что имена генерируются только с помощью функции genTextures
	*/
	unsigned int lastTexName;
	//TODO:массив использованных имён

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

	
	void init(){
	
		activeTexUnitIndex = 0;
		INIT_TEX_UNITS();
		INIT_TEX_OBJECTS();

	}
	
};

#endif