#ifndef __NMGLTEX_NM1_H__
#define __NMGLTEX_NM1_H__

struct NMGL_Context_NM1_Texture {
	
	/**
	*  Индекс активного текстурного модуля в массиве текстурных модулей.
	*  Используется для доступа к активному текстурному модулю
	*/
	unsigned int activeTexUnitIndex;

	
	void init(){
	
		activeTexUnitIndex = 0;
	}
	
};

#endif