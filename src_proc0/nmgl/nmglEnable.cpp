#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmblas.h"
#include "nmpp.h"
#include "ringbuffer.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglEnable(NMGLenum cap) {
	switch (cap) {

	case NMGL_ALPHA_TEST:
		//code
		break;

	case NMGL_BLEND:
		//code
		break;
		
	case NMGL_COLOR_LOGIC_OP:
		//code
		break;

	case NMGL_COLOR_MATERIAL:
		//code
		break;		
		
	case NMGL_CULL_FACE:
		cntxt.isCullFace = NMGL_TRUE;
		break;	

	case NMGL_DEPTH_TEST:
		cntxt.synchro.writeInstr(1, NMC1_DEPTH, NMGL_TRUE);
		break;
	
	case NMGL_DITHER:
		//code
		break;
	
	case NMGL_FOG:
		//code
		break;
		
	case NMGL_LINE_SMOOTH:
		//code
		break;
		
	case NMGL_MULTISAMPLE:
		//code
		break;
		
	case NMGL_NORMALIZE:
		cntxt.normalizeEnabled = NMGL_TRUE;
		break;
		
	case NMGL_POINT_SMOOTH:
		//code
		break;
		
/*	case NMGL_POINT_SPRITE_OES:
		//code
		break;*/
		
	case NMGL_POLYGON_OFFSET_FILL:
		//code
		break;
		
	case NMGL_RESCALE_NORMAL:
		//code
		break;
		
	case NMGL_SAMPLE_ALPHA_TO_COVERAGE:
		//code
		break;

	case NMGL_SAMPLE_ALPHA_TO_ONE:
		//code
		break;
		
	case NMGL_SAMPLE_COVERAGE:
		//code
		break;
	
	case NMGL_SCISSOR_TEST:
		//code
		break;
		
	case NMGL_STENCIL_TEST:
		//code
		break;
	
	case NMGL_TEXTURE_2D:
		//code
		break;
	
	case NMGL_CLIP_PLANE0:
		//code
		break;

	case NMGL_LIGHTING:
		cntxt.isLighting = NMGL_TRUE;
		break;

	case NMGL_LIGHT0:
		cntxt.isEnabledLight[0] = 1;
		break;

	case NMGL_LIGHT1:
		cntxt.isEnabledLight[1] = 1;
		break;
	case NMGL_LIGHT2:
		cntxt.isEnabledLight[2] = 1;
		break;
	case NMGL_LIGHT3:
		cntxt.isEnabledLight[3] = 1;
		break;
	case NMGL_LIGHT4:
		cntxt.isEnabledLight[4] = 1;
		break;
	case NMGL_LIGHT5:
		cntxt.isEnabledLight[5] = 1;
		break;
	case NMGL_LIGHT6:
		cntxt.isEnabledLight[6] = 1;
		break;
	case NMGL_LIGHT7:
		cntxt.isEnabledLight[7] = 1;
		break;
	}
}
