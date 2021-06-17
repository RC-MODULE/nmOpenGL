#include "demo3d_nm1.h"



SECTION(".text_demo3d") void NMGL_SetActiveTexture(NMGL_Context_NM1 *cntxt, CommandNm1 *command) {
	cntxt->texState.activeTexUnitIndex = command->params[0].ui;
}

SECTION(".text_demo3d") void NMGL_SetMipmapLvlPointer(NMGL_Context_NM1 *cntxt, CommandNm1 *command){
	//use DDR only as addresses are sent directly
	cntxt->texState.texObjects[command->params[0].i].texImages2D[command->params[1].i].pixels=command->params[2].p;
	DEBUG_PRINT2(("Step:for texture %d level %d got pointer %x\n",command->params[0],command->params[1],command->params[2]));
}

SECTION(".text_demo3d") void NMGL_BindActiveTexObject(NMGL_Context_NM1 *cntxt, CommandNm1 *command){
	NMGLenum target = command->params[0].ui;
	NMGLuint texture = command->params[1].ui;
    ActiveTexObjectP=&cntxt->texState.texObjects[texture];
}

SECTION(".text_demo3d") void NMGL_SetWhf(NMGL_Context_NM1 *cntxt, CommandNm1 *command){
	#define TexObj cntxt->texState.texObjects
	NMGLuint texture=command->params[0].ui;		
	NMGLsizei width0=command->params[1].i;
	NMGLsizei height0=command->params[2].i;
	NMGLenum format=command->params[3].i;
	int i=0;
	
	for(i=0;i<=NMGL_MAX_MIPMAP_LVL;i++)
	{
		TexObj[texture].texImages2D[i].internalformat=format;
		if(width0>>i >= 1)
		{
			TexObj[texture].texImages2D[i].width=width0>>i;
		}
		else
		{
			TexObj[texture].texImages2D[i].width=1;
		}
		if(height0>>i >= 1)
		{
			TexObj[texture].texImages2D[i].height=height0>>i;
		}
		else
		{
			TexObj[texture].texImages2D[i].height=1;
		}
	}
}

SECTION(".text_demo3d") void NMGL_SetColorPalette(NMGL_Context_NM1 *cntxt, CommandNm1 *command){
	cntxt->texState.texObjects[command->params[2].i].palette.colors=(NMGLubyte*)command->params[0].p;
	cntxt->texState.texObjects[command->params[2].i].palette.width=command->params[1].i;				
}

SECTION(".text_demo3d") void NMGL_SetTexEnvColor(NMGL_Context_NM1 *cntxt, CommandNm1 *command){
	Intfloat temp;
	for (int i = 0; i < 4; i++) {
		temp.i = command->params[i].i;
		cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor[i] = temp.f;
	}
}

SECTION(".text_demo3d") void NMGL_SetTexEnvMode(NMGL_Context_NM1 *cntxt, CommandNm1 *command){
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texFunctionName = command->params[0].i;
}

SECTION(".text_demo3d") void NMGL_SetTexParami(NMGL_Context_NM1 *cntxt, CommandNm1 *command){
	switch ((NMGLenum)command->params[0].i)
	{
		case NMGL_TEXTURE_WRAP_S: 
			cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texWrapS = (NMGLint)command->params[1].i;
			break;
			
		case NMGL_TEXTURE_WRAP_T: 
			cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texWrapT = (NMGLint)command->params[1].i;
			break;
		
		case NMGL_TEXTURE_MIN_FILTER: 
			cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texMinFilter = (NMGLint)command->params[1].i;
			break;
		
		case NMGL_TEXTURE_MAG_FILTER: 
			cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texMagFilter = (NMGLint)command->params[1].i;
			break;
			
		default: 
			break;
	}
}

SECTION(".text_demo3d") void NMGL_Texture2d(NMGL_Context_NM1 *cntxt, CommandNm1 *command){
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].enabled = (NMGLboolean)command->params[0].i;
	if ((NMGLboolean)command->params[0].i == NMGL_TRUE) {
		cntxt->texState.textureEnabled = cntxt->texState.textureEnabled | ((unsigned int)1 << cntxt->texState.activeTexUnitIndex);
	} 
	else {
		cntxt->texState.textureEnabled = cntxt->texState.textureEnabled & (~((unsigned int)1 << cntxt->texState.activeTexUnitIndex));
	}
}