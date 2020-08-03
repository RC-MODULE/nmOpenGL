#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
//============================================================================================================
void nmglGenTextures (NMGLsizei n, NMGLuint *textures);
//============================================================================================================
void nmglGenTextures (NMGLsizei n, NMGLuint *textures)
{
	int i=0;
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();	
	for(i=0;i<n;i++)
	{
		++cntxt->texState.lastTexName;
		textures[i]=cntxt->texState.lastTexName;
	}
}
