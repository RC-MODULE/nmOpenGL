#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmblas.h"
#include "nmpp.h"
#include "ringbuffer.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglDisable(NMGLenum cap) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	CommandNm1 command;
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
		cntxt->isColorMaterial = NMGL_FALSE;
		cntxt->materialAmbient = *cntxt->pMaterialAmbient;// i.e. currentColor
		cntxt->materialDiffuse = *cntxt->pMaterialDiffuse;// i.e. currentColor
		cntxt->pMaterialAmbient = &cntxt->materialAmbient;
		cntxt->pMaterialDiffuse = &cntxt->materialDiffuse;
		break;		
		
	case NMGL_CULL_FACE:
		cntxt->isCullFace = NMGL_FALSE;
		break;	

	case NMGL_DEPTH_TEST:
		command.instr = NMC1_DEPTH;
		command.params[0] = CommandArgument(NMGL_FALSE);
		cntxt->synchro.pushInstr(&command);
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
		cntxt->normalizeEnabled = NMGL_FALSE;
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
		cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].enabled = NMGL_FALSE;
		cntxt->texState.textureEnabled = cntxt->texState.textureEnabled & (~((unsigned int)1 << cntxt->texState.activeTexUnitIndex));

		command.instr = NMC1_TEXTURE2D;
		command.params[0] = CommandArgument(NMGL_FALSE);
		cntxt->synchro.pushInstr(&command);
		//cntxt->synchro.writeInstr(1, NMC1_TEXTURE2D, NMGL_FALSE);
		break;
	
	case NMGL_CLIP_PLANE0:
		//code
		break;

	case NMGL_LIGHTING:
		cntxt->isLighting = NMGL_FALSE;
		break;

	case NMGL_LIGHT0:
		cntxt->isEnabledLight[0] = 0;
		break;

	case NMGL_LIGHT1:
		cntxt->isEnabledLight[1] = 0;
		break;
	case NMGL_LIGHT2:
		cntxt->isEnabledLight[2] = 0;
		break;
	case NMGL_LIGHT3:
		cntxt->isEnabledLight[3] = 0;
		break;
	case NMGL_LIGHT4:
		cntxt->isEnabledLight[4] = 0;
		break;
	case NMGL_LIGHT5:
		cntxt->isEnabledLight[5] = 0;
		break;
	case NMGL_LIGHT6:
		cntxt->isEnabledLight[6] = 0;
		break;
	case NMGL_LIGHT7:
		cntxt->isEnabledLight[7] = 0;
		break;
	}
}
