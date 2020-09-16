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

	
#ifdef TEST_NMGL_TEX_FUNC
	//ref value to check the accessibility of nmpu1
	unsigned int refValue = 0xC0DEC0DE;
#endif //TEST_NMGL_TEX_FUNC
	
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

#endif
