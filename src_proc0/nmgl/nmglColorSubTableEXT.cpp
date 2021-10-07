#include "demo3d_nm0.h"

SECTION(".text_nmgl")

//============================================================================================================
void nmglColorSubTableEXT (NMGLenum target, NMGLsizei start, NMGLsizei count, NMGLenum format, NMGLenum type, const void *data);
//============================================================================================================

extern NMGLubyte mem_palette[NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE*NMGL_MAX_TEX_OBJECTS];
void nmglColorSubTableEXT (NMGLenum target, NMGLsizei start, NMGLsizei count, NMGLenum format, NMGLenum type, const void *data)
{
	int i=0;
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGLubyte* p;
	NMGLsizei realcount;
	if ((target != NMGL_TEXTURE_2D)&& (target != NMGL_SHARED_TEXTURE_PALETTE_EXT )) {cntxt->error=NMGL_INVALID_ENUM;return;}
	if (type !=NMGL_UNSIGNED_BYTE) {cntxt->error=NMGL_INVALID_ENUM;return;}
	
	if(format !=NMGL_RGBA){cntxt->error=NMGL_INVALID_VALUE;return;}
	if((start >= *ActiveTexObjectP->palette.width)||(start<0)){cntxt->error=NMGL_INVALID_VALUE;return;}
	realcount = count;
	if(target == NMGL_TEXTURE_2D )
	{
		p=cntxt->texState.palette_pointers[ActiveTexObjectP->name+1];
		realcount = *cntxt->texState.paletts_widths_pointers[ActiveTexObjectP->name+1];		
	}
	else
	{
		p=cntxt->texState.palette_pointers[0];
		realcount = *cntxt->texState.paletts_widths_pointers[0];
	}
	if(start + count > realcount)
	{
		realcount = realcount - start;
	}
	else{
			realcount = count;
	}
	for(i=0;i<realcount*RGBA_TEXEL_SIZE_UBYTE;i++)
	{
		//if(start+i/RGBA_TEXEL_SIZE_UBYTE >= *ActiveTexObjectP->palette.width) break;
		*(p+start*RGBA_TEXEL_SIZE_UBYTE+i)=*((NMGLubyte*)data+i);
	}

}
