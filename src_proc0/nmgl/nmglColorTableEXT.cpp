#include "demo3d_nm0.h"
SECTION(".textures_mipmap_mem")  NMGLubyte mem_palette[NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE*NMGL_MAX_TEX_OBJECTS];

SECTION(".text_nmgl")

//============================================================================================================
void nmglColorTableEXT (NMGLenum target, NMGLenum internalformat, NMGLsizei width, NMGLenum format, NMGLenum type, const void *data);
/*
	target = NMGL_TEXTURE_2D    only (Spec. restriction)
	format = internalformat     only (Spec. restriction)
	format = NMGL_RGBA	        only (Implementation restriction)
	type   = NMGL_UNSIGNED_BYTE only (Spec. restriction)
	width  = 2^x >=1 			only (Spec. restriction) //in texels
*/
//============================================================================================================

void nmglColorTableEXT (NMGLenum target, NMGLenum internalformat, NMGLsizei width, NMGLenum format, NMGLenum type, const void *data)
{
	int i=0;
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	if (target !=NMGL_TEXTURE_2D) {cntxt->error=NMGL_INVALID_ENUM;return;}
	if (type !=NMGL_UNSIGNED_BYTE) {cntxt->error=NMGL_INVALID_ENUM;return;}
	if(internalformat != format)  {cntxt->error=NMGL_INVALID_VALUE;return;}
	if(internalformat !=NMGL_RGBA){cntxt->error=NMGL_INVALID_VALUE;return;}
	if(width<=0) {cntxt->error=NMGL_INVALID_VALUE;return;}
	if(width>NMGL_MAX_PALETTE_WIDTH){cntxt->error=NMGL_OUT_OF_MEMORY;return;}
	ActiveTexObjectP->palette.width=width;
	ActiveTexObjectP->palette.colors=(NMGLubyte*)(mem_palette+ActiveTexObjectP->name*NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE);
	cntxt->synchro.writeInstr(1, NMC1_SET_COLOR_PALETTE, (unsigned int)ActiveTexObjectP->palette.colors,(unsigned int)ActiveTexObjectP->palette.width,ActiveTexObjectP->name);
	for(i=0;i<width*RGBA_TEXEL_SIZE_UBYTE;i++)
	{
		*((NMGLubyte*)((NMGLubyte*)ActiveTexObjectP->palette.colors+i))=*((NMGLubyte*)data+i);
	}

}
