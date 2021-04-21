#include "demo3d_nm1.h"



SECTION(".text_demo3d") void NMGL_SetActiveTexture(NMGL_Context_NM1 *cntxt, CommandNm1 *command) {
	cntxt->texState.activeTexUnitIndex = command->params[0];
}

SECTION(".text_demo3d") void NMGL_SetMipmapLvlPointer(NMGL_Context_NM1 *cntxt, CommandNm1 *command){
	//use DDR only as addresses are sent directly
	cntxt->texState.texObjects[command->params[0]].texImages2D[command->params[1]].pixels=halMapAddrFrom((void*)command->params[2],0);
	DEBUG_PRINT2(("Step:for texture %d level %d got pointer %x\n",command->params[0],command->params[1],command->params[2]));
}

SECTION(".text_demo3d") void NMGL_BindActiveTexObject(NMGL_Context_NM1 *cntxt, CommandNm1 *command){
	NMGLenum target = command->params[0];
	NMGLuint texture = command->params[1];
    ActiveTexObjectP=&cntxt->texState.texObjects[texture];
}

SECTION(".text_demo3d") void NMGL_SetWhf(NMGL_Context_NM1 *cntxt, CommandNm1 *command){
	#define TexObj cntxt->texState.texObjects
	NMGLuint texture=command->params[0];		
	NMGLsizei width0=command->params[1];
	NMGLsizei height0=command->params[2];
	NMGLenum format=command->params[3];
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
	cntxt->texState.texObjects[(int)command->params[2]].palette.colors=(NMGLubyte*)command->params[0];
	cntxt->texState.texObjects[(int)command->params[2]].palette.width=command->params[1];				
}

SECTION(".text_demo3d") void NMGL_SetTexEnvColor(NMGL_Context_NM1 *cntxt, CommandNm1 *command){
	Intfloat temp;
	for (int i = 0; i < 4; i++) {
		temp.i = command->params[i];
		cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor[i] = temp.f;
	}
}

SECTION(".text_demo3d") void NMGL_SetTexEnvMode(NMGL_Context_NM1 *cntxt, CommandNm1 *command){
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texFunctionName = command->params[0];
}

SECTION(".text_demo3d") void NMGL_SetTexParami(NMGL_Context_NM1 *cntxt, CommandNm1 *command){
	switch ((NMGLenum)command->params[0])
	{
		case NMGL_TEXTURE_WRAP_S: 
			cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texWrapS = (NMGLint)command->params[1];
			break;
			
		case NMGL_TEXTURE_WRAP_T: 
			cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texWrapT = (NMGLint)command->params[1];
			break;
		
		case NMGL_TEXTURE_MIN_FILTER: 
			cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texMinFilter = (NMGLint)command->params[1];
			break;
		
		case NMGL_TEXTURE_MAG_FILTER: 
			cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texMagFilter = (NMGLint)command->params[1];
			break;
			
		default: 
			break;
	}
}

SECTION(".text_demo3d") void NMGL_Texture2d(NMGL_Context_NM1 *cntxt, CommandNm1 *command){
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].enabled = (NMGLboolean)command->params[0];
	if ((NMGLboolean)command->params[0] == NMGL_TRUE) {
		cntxt->texState.textureEnabled = cntxt->texState.textureEnabled | ((unsigned int)1 << cntxt->texState.activeTexUnitIndex);
	} 
	else {
		cntxt->texState.textureEnabled = cntxt->texState.textureEnabled & (~((unsigned int)1 << cntxt->texState.activeTexUnitIndex));
	}
}