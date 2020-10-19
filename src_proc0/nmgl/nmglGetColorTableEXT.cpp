#include "demo3d_nm0.h"

SECTION(".text_nmgl")

//============================================================================================================
void nmglGetColorTableEXT (NMGLenum target, NMGLenum format, NMGLenum type,void *data);
//============================================================================================================

void nmglGetColorTableEXT (NMGLenum target, NMGLenum format, NMGLenum type,void *data)
{
	int i=0;
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (target !=NMGL_TEXTURE_2D) {cntxt->error=NMGL_INVALID_ENUM;return;}
	if (type !=NMGL_UNSIGNED_BYTE) {cntxt->error=NMGL_INVALID_ENUM;return;}
	
	if(format !=NMGL_RGBA){cntxt->error=NMGL_INVALID_VALUE;return;}
	data=ActiveTexObjectP->palette.colors;
}
