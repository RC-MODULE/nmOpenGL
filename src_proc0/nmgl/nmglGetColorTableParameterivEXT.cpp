#include "demo3d_nm0.h"

SECTION(".text_nmgl")

//============================================================================================================
void nmglGetColorTableParameterivEXT (NMGLenum target, NMGLenum pname, NMGLint *params);

//============================================================================================================

void nmglGetColorTableParameterivEXT (NMGLenum target, NMGLenum pname, NMGLint *params)
{
	int i=0;
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (target !=NMGL_TEXTURE_2D) {cntxt->error=NMGL_INVALID_ENUM;return;}	
	switch(pname)
	{
		case NMGL_COLOR_TABLE_FORMAT_EXT:
				*params=NMGL_COLOR_INDEX8_EXT;
				break;
		case NMGL_COLOR_TABLE_WIDTH_EXT:
				*params=ActiveTexObjectP->palette.width;
				break;
		case NMGL_COLOR_TABLE_RED_SIZE_EXT:
				*params=1;
				break;
		case NMGL_COLOR_TABLE_GREEN_SIZE_EXT:
				*params=1;
				break;
		case NMGL_COLOR_TABLE_BLUE_SIZE_EXT:
				*params=1;
				break;
		case NMGL_COLOR_TABLE_ALPHA_SIZE_EXT:
				*params=1;
				break;
		case NMGL_COLOR_TABLE_LUMINANCE_SIZE_EXT:
				*params=1;
				break;
		case NMGL_COLOR_TABLE_INTENSITY_SIZE_EXT:
				*params=1;
				break;
	}

	
}
