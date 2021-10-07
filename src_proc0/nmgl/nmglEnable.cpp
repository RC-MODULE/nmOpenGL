#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmblas.h"
#include "nmpp.h"
#include "ringbuffer.h"
#include "nmglservice_nm0.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglEnable(NMGLenum cap) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NM_Command command;
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
		cntxt->lightingInfo.isColorMaterial = NMGL_TRUE;
		cntxt->lightingInfo.pMaterialAmbient = &cntxt->currentColor;
		cntxt->lightingInfo.pMaterialDiffuse = &cntxt->currentColor;
		break;		
		
	case NMGL_CULL_FACE:
		cntxt->isCullFace = NMGL_TRUE;
		break;	

	case NMGL_DEPTH_TEST:
		command.instr = NMC1_DEPTH;
		command.params[0] = CommandArgument(NMGL_TRUE);
		NMGL_SetValue(command);
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
		cntxt->normalizeEnabled = NMGL_TRUE;
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
		cntxt->scissorTest.isEnabled = NMGL_TRUE;
		cntxt->currentSegments = &cntxt->scissorTest.segments;
		break;
		
	case NMGL_STENCIL_TEST:
		//code
		break;
	
	case NMGL_SHARED_TEXTURE_PALETTE_EXT:
		cntxt->texState.using_shared_palette = NMGL_TRUE;
		//cntxt->texState.use_shared_palette(cntxt->texState.palette_pointers[0],cntxt->texState.paletts_widths);
		cntxt->texState.use_shared_palette();
		
		//NM_Command command_stp;
		command.instr = NMC1_USE_SHARED_PALETTE;
		command.params[0] = CommandArgument(cntxt->texState.palette_pointers[0]);
		//command_stp.params[1] = CommandArgument(width);
		//command_stp.params[2] = CommandArgument(nm1_name);
		//cntxt->synchro.pushInstr(&command);		
		NMGL_SetValue(command);
		break;

	case NMGL_TEXTURE_2D:
		cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].enabled = NMGL_TRUE;
		cntxt->texState.textureEnabled = cntxt->texState.textureEnabled | ((unsigned int)1 << cntxt->texState.activeTexUnitIndex);
		//NM_Command command;
		command.instr = NMC1_TEXTURE2D;
		command.params[0] = CommandArgument(NMGL_TRUE);
		NMGL_SetValue(command);
		break;
		
	case NMGL_LINE_STIPPLE:
		break;

	case NMGL_POLYGON_STIPPLE:
		break;

	case NMGL_CLIP_PLANE0:
		//code
		break;

	case NMGL_LIGHTING:
		cntxt->lightingInfo.isLighting = NMGL_TRUE;
		break;

	case NMGL_LIGHT0:
	case NMGL_LIGHT1:
	case NMGL_LIGHT2:
	case NMGL_LIGHT3:
	case NMGL_LIGHT4:
	case NMGL_LIGHT5:
	case NMGL_LIGHT6:
	case NMGL_LIGHT7:
		cntxt->lightingInfo.isEnabledLight[cap - NMGL_LIGHT0] = true;
		break;
	}
}
