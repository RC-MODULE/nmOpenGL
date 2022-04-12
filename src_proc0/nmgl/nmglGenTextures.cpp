#include "demo3d_nm0.h"
#include "nmgl.h"
#include "debugprint.h"
//#include <iostream>

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
//============================================================================================================
void nmglGenTextures (NMGLsizei n, NMGLuint *textures);
//============================================================================================================
void nmglGenTextures (NMGLsizei n, NMGLuint *textures)
{
	DEBUG_PLOG_FUNCTION();
	int i=0;
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	//printf("Inside gentextures:context pointer is %x\n",cntxt);	
	for(i=0;i<n;i++)
	{		
		cntxt->texState.lastTexName+=1;
		//printf("Inside gentextures:lastTexName=%d",cntxt->texState.lastTexName);
		*((NMGLuint *)textures +i)=cntxt->texState.lastTexName;
	}
}
