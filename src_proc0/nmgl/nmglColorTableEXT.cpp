#include "demo3d_nm0.h"
//SECTION(".textures_mipmap_mem")  NMGLubyte mem_palette[NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE*(NMGL_MAX_TEX_OBJECTS+1)];// zero is for shared palette






//TODO: move meme init to cntxt->texState.init()










SECTION(".text_nmgl")
extern int isPowerOf2(NMGLint x);
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
	NMGLubyte *palette_p=0;
	//NMGLuint nm1_name=0;
	if ((target != NMGL_TEXTURE_2D)&& (target != NMGL_SHARED_TEXTURE_PALETTE_EXT )) {cntxt->error=NMGL_INVALID_ENUM;return;}
	if (type !=NMGL_UNSIGNED_BYTE) {cntxt->error=NMGL_INVALID_ENUM;return;}
	if(internalformat != format)  {cntxt->error=NMGL_INVALID_VALUE;return;}
	if(internalformat !=NMGL_RGBA){cntxt->error=NMGL_INVALID_VALUE;return;}
	if(width<=0) {cntxt->error=NMGL_INVALID_VALUE;return;}	
	if(width>NMGL_MAX_PALETTE_WIDTH){cntxt->error=NMGL_OUT_OF_MEMORY;return;}
	if(!isPowerOf2(width)) {cntxt->error=NMGL_INVALID_VALUE;return;}

	//cntxt->texState.palettes_pointer

	if(target == NMGL_TEXTURE_2D )
	{
		*cntxt->texState.paletts_widths_pointers[(ActiveTexObjectP->name+1)]=width;
		palette_p=cntxt->texState.get_palette_by_name_p((ActiveTexObjectP->name));
		//nm1_name=ActiveTexObjectP->name+1;
	}
	else //NMGL_SHARED_TEXTURE_PALETTE_EXT
	{
		*cntxt->texState.paletts_widths_pointers[0]=width;
		//palette_p=(NMGLubyte*)mem_palette;
		palette_p=cntxt->texState.get_shared_palette_p();
		//nm1_name=0;
	}
	/*
	if(cntxt->texState.using_shared_palette = 0)
	{
		cntxt->texState.texObjects[ActiveTexObjectP->name].palette.setColors(cntxt->texState.get_palette_by_name_p(ActiveTexObjectP->name));
		cntxt->texState.texObjects[ActiveTexObjectP->name].palette.setWidth_p(&cntxt->texState.paletts_widths[(ActiveTexObjectP->name+1)]);
	}
	else{
		cntxt->texState.texObjects[ActiveTexObjectP->name].palette.setColors(cntxt->texState.get_shared_palette_p());
		cntxt->texState.texObjects[ActiveTexObjectP->name].palette.setWidth_p(&cntxt->texState.paletts_widths[0]);
	}
	*/
	for(i=0;i<width*RGBA_TEXEL_SIZE_UBYTE;i++)
	{
		*((NMGLubyte*)((NMGLubyte*)palette_p+i))=*((NMGLubyte*)data+i);
	}
	/*
	NM_Command command;

	command.instr = NMC1_SET_COLOR_PALETTE;
	command.params[0] = CommandArgument(palette_p);
	command.params[1] = CommandArgument(width);
	command.params[2] = CommandArgument(nm1_name);
	cntxt->synchro.pushInstr(&command);
	*/
	//cntxt->synchro.writeInstr(1, NMC1_SET_COLOR_PALETTE, (unsigned int)ActiveTexObjectP->palette.colors,(unsigned int)ActiveTexObjectP->palette.width,ActiveTexObjectP->name);


}
