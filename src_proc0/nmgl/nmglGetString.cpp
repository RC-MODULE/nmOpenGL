#include "demo3d_nm0.h"
#include "nmgl.h"
#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
const NMGLubyte* nmglGetString(NMGLenum name) {
	
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	switch (name)
	{
	case NMGL_VENDOR:
		return (NMGLubyte*)"RC MODULE";
		break;
		
	case NMGL_RENDERER:
		return (NMGLubyte*)"Neuromatrix (NMC4)";
		break;

	case NMGL_VERSION:
		return (NMGLubyte*)"OpenGL SC 1.0";
		break;
		
	case NMGL_EXTENSIONS:
		return (NMGLubyte*)"EXT_paletted_texture EXT_shared_texture_palette";
		break;

	default:
		cntxt->error = NMGL_INVALID_ENUM;
		break;
	}
	
	return (NMGLubyte*)" ";
}
