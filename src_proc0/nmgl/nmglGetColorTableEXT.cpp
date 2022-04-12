#include "demo3d_nm0.h"
#include "debugprint.h"

SECTION(".text_nmgl")

//============================================================================================================
void nmglGetColorTableEXT (NMGLenum target, NMGLenum format, NMGLenum type,void *data);
//============================================================================================================

void nmglGetColorTableEXT (NMGLenum target, NMGLenum format, NMGLenum type,void *data)
{
	DEBUG_PLOG_FUNCTION();
	int i=0;
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if ((target != NMGL_TEXTURE_2D)&& (target != NMGL_SHARED_TEXTURE_PALETTE_EXT )) {cntxt->error=NMGL_INVALID_ENUM;return;}
	if (type !=NMGL_UNSIGNED_BYTE) {cntxt->error=NMGL_INVALID_ENUM;return;}
	
	if(format !=NMGL_RGBA){cntxt->error=NMGL_INVALID_VALUE;return;}
	
	NMGLint border;
	NMGLubyte* d;

	if(target == NMGL_TEXTURE_2D)
	{
		border =  *ActiveTexObjectP->palette.width*RGBA_TEXEL_SIZE_UBYTE;
		d = ActiveTexObjectP->palette.colors;
	}
	else
	{
		border = *cntxt->texState.paletts_widths_pointers[0]*RGBA_TEXEL_SIZE_UBYTE;

		d = cntxt->texState.palette_pointers[0];
	}
	for(int i=0;i<border;i++)
	{
		*((NMGLubyte*)data+i) = *((NMGLubyte*)d+i);
	}
	
}
