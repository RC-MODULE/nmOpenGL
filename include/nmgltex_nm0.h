#ifndef __NMGLTEX_NM0_H__
#define __NMGLTEX_NM0_H__

#include "demo3d_common.h"
#include "nmgltex_common.h"

struct NMGL_Context_NM0_Texture {
	
	/**
	* Массивы текстурных координат
	*/
	Array texcoordArray[NMGL_MAX_TEX_UNITS];
	
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
	
	
	void init(){
	
		activeTexUnit = NMGL_TEXTURE0;
		activeTexUnitIndex = 0;
		clientActiveTexUnit = NMGL_TEXTURE0;
		clientActiveTexUnitIndex = 0;
		
		for (int i = 0; i < NMGL_MAX_TEX_UNITS; i++)
		{
			texcoordArray[i].pointer = 0;
			texcoordArray[i].size = 4;
			texcoordArray[i].stride = 0;
			texcoordArray[i].type = NMGL_FLOAT;
			texcoordArray[i].enabled = NMGL_FALSE;
		}
	}
	
};

#endif