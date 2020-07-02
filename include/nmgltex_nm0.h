#ifndef __NMGLTEX_NM0_H__
#define __NMGLTEX_NM0_H__


struct NMGL_Context_NM0_Texture {
	
	/**
	*  Имя активного текстурного модуля.
	*/
	NMGLenum activeTexUnit;

	/**
	*  Индекс активного текстурного модуля в массиве текстурных модулей.
	*  Используется для доступа к активному текстурному модулю
	*/
	unsigned int activeTexUnitIndex;
	
	
	void init(){
	
		activeTexUnit = NMGL_TEXTURE0;
		activeTexUnitIndex = 0;
	}
	
};

#endif