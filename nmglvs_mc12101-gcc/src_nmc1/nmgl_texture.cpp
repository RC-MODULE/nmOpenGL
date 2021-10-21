#include "demo3d_nm1.h"

#define DEBUG_LEVEL 1

SECTION(".text_demo3d") void NMGL_SetActiveTexture(NMGL_Context_NM1 *cntxt, NM_Command *command) {
	cntxt->texState.activeTexUnitIndex = command->params[0].ui;
}

SECTION(".text_demo3d") void NMGL_SetMipmapLvlPointer(NMGL_Context_NM1 *cntxt, NM_Command *command){
	//use DDR only as addresses are sent directly
	cntxt->texState.texObjects[command->params[0].i].texImages2D[command->params[1].i].pixels=command->params[2].p;
	DEBUG_PRINT2(("Step:for texture %d level %d got pointer %x\n",command->params[0],command->params[1],command->params[2]));
}

SECTION(".text_demo3d") void NMGL_BindActiveTexObject(NMGL_Context_NM1 *cntxt, NM_Command *command){
	NMGLenum target = command->params[0].ui;
	NMGLuint texture = command->params[1].ui;
    ActiveTexObjectP=&cntxt->texState.texObjects[texture];
}

SECTION(".text_demo3d") void NMGL_SetWhf(NMGL_Context_NM1 *cntxt, NM_Command *command){
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

SECTION(".text_demo3d") void NMGL_UseSharedPalette(NMGL_Context_NM1 *cntxt, NM_Command *command){
		cntxt->texState.palette_is_shared = 1;
		/*
		for (int i = 0; i < NMGL_MAX_TEX_OBJECTS+1; i++)
		{
			cntxt->texState.palette_pointers[i]=(NMGLubyte*)((NMGLubyte*)command->params[0].p+i*NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE);
		}
		*/
		for (int i = 0; i < NMGL_MAX_TEX_OBJECTS; i++)
		{	
			//cntxt->texState.texObjects[i].palette.setColors(cntxt->texState.palette_pointers[0]);
			cntxt->texState.texObjects[i].palette.colors = cntxt->texState.palette_pointers[0];
			//cntxt->texState.texObjects[i].palette.setWidth_p(&cntxt->texState.paletts_widths[0]);
			cntxt->texState.texObjects[i].palette.width = cntxt->texState.paletts_widths_pointers[0];
		}				
}
SECTION(".text_demo3d") void NMGL_UseLocalPalette(NMGL_Context_NM1 *cntxt, NM_Command *command){
		cntxt->texState.palette_is_shared = 0;
		/*
		for (int i = 0; i < NMGL_MAX_TEX_OBJECTS; i++)
		{
			cntxt->texState.palette_pointers[i]=(NMGLubyte*)((NMGLubyte*)command->params[0].p+i*NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE);
		}
		*/
		for (int i = 0; i < NMGL_MAX_TEX_OBJECTS; i++)
		{
			//cntxt->texState.texObjects[i].palette.setColors(cntxt->texState.palette_pointers[(i+1)]);
			cntxt->texState.texObjects[i].palette.colors = cntxt->texState.palette_pointers[(i+1)];
			cntxt->texState.texObjects[i].palette.width = cntxt->texState.paletts_widths_pointers[(i+1)];
			//cntxt->texState.texObjects[i].palette.setWidth_p(&cntxt->texState.paletts_widths[(i+1)]);
		}				
}
/*
SECTION(".text_demo3d") void NMGL_SetColorPalette(NMGL_Context_NM1 *cntxt, NM_Command *command){
	
		//cntxt->texState.palette_pointers[command->params[2].i] = (NMGLubyte*)command->params[0].p;
		//cntxt->texState.paletts_widths[command->params[2].i]=command->params[1].i;
		
		DEBUG_PRINT2(("nm1 got width==%d from object:%d\n", command->params[1].i, command->params[2].i));
		if(command->params[2].i >0){
		DEBUG_PRINT2(("READ from object=%d\n", *cntxt->texState.texObjects[command->params[2].i].palette.width));
		}
		
DEBUG_PRINT2(("Pointers/ObjectPointers:\n"));
		DEBUG_PRINT2(("   pointer[0]=0x%x\n", cntxt->texState.palette_pointers[0]));
		for(int i=1;i<NMGL_MAX_TEX_OBJECTS+1;i++)
		{
			DEBUG_PRINT2(("   pointer[%d]=0x%x objpointer=0x%x\n", i,cntxt->texState.paletts_widths_pointers[i],cntxt->texState.texObjects[i-1].palette.colors));
		}
DEBUG_PRINT2(("widthAddress/ObjectWidthPointer:\n"));
		DEBUG_PRINT2(("   widthAddr[0]=0x%x width=%d\n", &cntxt->texState.paletts_widths_pointers[0],cntxt->texState.paletts_widths_pointers[0],*cntxt->texState.paletts_widths[0]));
		for(int i=1;i<NMGL_MAX_TEX_OBJECTS+1;i++)
		{
			DEBUG_PRINT2(("   widthAddr[%d]=0x%x objpointer=0x%x width=%d\n", i,cntxt->texState.paletts_widths_pointers[i],cntxt->texState.texObjects[i-1].palette.width,*cntxt->texState.texObjects[i-1].palette.width));
		}
		
}*/

SECTION(".text_demo3d") void NMGL_SetTexEnvColor(NMGL_Context_NM1 *cntxt, NM_Command *command){
	Intfloat temp;
	for (int i = 0; i < 4; i++) {
		temp.i = command->params[i].i;
		cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor[i] = temp.f;
	}
}

SECTION(".text_demo3d") void NMGL_SetTexEnvMode(NMGL_Context_NM1 *cntxt, NM_Command *command){
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texFunctionName = command->params[0].i;
}

SECTION(".text_demo3d") void NMGL_SetTexParami(NMGL_Context_NM1 *cntxt, NM_Command *command){
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

SECTION(".text_demo3d") void NMGL_Texture2d(NMGL_Context_NM1 *cntxt, NM_Command *command){
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].enabled = (NMGLboolean)command->params[0].i;
	if ((NMGLboolean)command->params[0].i == NMGL_TRUE) {
		cntxt->texState.textureEnabled = cntxt->texState.textureEnabled | ((unsigned int)1 << cntxt->texState.activeTexUnitIndex);
	} 
	else {
		cntxt->texState.textureEnabled = cntxt->texState.textureEnabled & (~((unsigned int)1 << cntxt->texState.activeTexUnitIndex));
	}
}