#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmblas.h"
#include "nmpp.h"
#include "ringbuffer.h"
#include "nmglservice_nm0.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglDisable(NMGLenum cap) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	NM_Command command;
	switch (cap) {

	case NMGL_ALPHA_TEST:
		context->alpha_test.enabled = NMGL_FALSE;
		break;

	case NMGL_BLEND:
		context->blend.enabled = NMGL_FALSE;	
		break;
		
	case NMGL_COLOR_LOGIC_OP:
		//code
		break;

	case NMGL_COLOR_MATERIAL:
		context->lightingInfo.isColorMaterial = NMGL_FALSE;
		context->lightingInfo.materialAmbient = *context->lightingInfo.pMaterialAmbient; // i.e. currentColor
		context->lightingInfo.materialDiffuse = *context->lightingInfo.pMaterialDiffuse; // i.e. currentColor
		context->lightingInfo.pMaterialAmbient = &context->lightingInfo.materialAmbient;
		context->lightingInfo.pMaterialDiffuse = &context->lightingInfo.materialDiffuse;
		break;		
		
	case NMGL_CULL_FACE:
		context->isCullFace = NMGL_FALSE;
		break;	

	case NMGL_DEPTH_TEST:
	{
		context->depth_test.enabled = NMGL_FALSE;
		NM_Command command;
		command.instr = NMC1_DEPTH_UPDATE;
		NMGL_SetValue(command);
		break;
	}
	case NMGL_DITHER:
		//code
		break;
	
	case NMGL_FOG:
		//code
		break;
		
	case NMGL_LINE_SMOOTH:
		context->line.smooth_enabled = NMGL_FALSE;
		break;
		
	case NMGL_MULTISAMPLE:
		//code
		break;
		
	case NMGL_NORMALIZE:
		context->normalizeEnabled = NMGL_FALSE;
		break;
		
	case NMGL_POINT_SMOOTH:
		context->point.smooth_enabled = NMGL_FALSE;
		break;
		
/*	case NMGL_POINT_SPRITE_OES:
		//code
		break;*/
		
	case NMGL_POLYGON_OFFSET_FILL:
		context->polygon.offset_fill_enabled = NMGL_FALSE;
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
		context->scissorTest.isEnabled = NMGL_FALSE;
		cntxt->currentSegments = &cntxt->viewportSegments;
		break;
		
	case NMGL_STENCIL_TEST:
		context->stencil_test.enabled = NMGL_FALSE;
		break;
	
	case NMGL_SHARED_TEXTURE_PALETTE_EXT:
		cntxt->texState.shared_palette_enabled = NMGL_FALSE;
		cntxt->texState.use_local_palettes();
		
		//NM_Command command_stp1;
		command.instr = NMC1_USE_LOCAL_PALETTE;
		command.params[0] = CommandArgument(cntxt->texState.palette_pointers[0]);
		//command_stp1.params[1] = CommandArgument(width);
		//command_stp1.params[2] = CommandArgument(nm1_name);
		//cntxt->synchro.pushInstr(&command_stp1);	
		NMGL_SetValue(command);
		
		break;

	case NMGL_TEXTURE_2D:
		cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].enabled = NMGL_FALSE;
		cntxt->texState.textureEnabled = cntxt->texState.textureEnabled & (~((unsigned int)1 << cntxt->texState.activeTexUnitIndex));

		command.instr = NMC1_TEXTURE2D;
		command.params[0] = CommandArgument(NMGL_FALSE);
		NMGL_SetValue(command);
		//cntxt->synchro.writeInstr(1, NMC1_TEXTURE2D, NMGL_FALSE);
		break;

	case NMGL_LINE_STIPPLE:
		context->line.stipple.enabled = NMGL_FALSE;
		break;

	case NMGL_POLYGON_STIPPLE:
		context->polygon.stipple.enabled = NMGL_FALSE;
		break;

	case NMGL_CLIP_PLANE0:
		//code
		break;

	case NMGL_LIGHTING:
		context->lightingInfo.isLighting = NMGL_FALSE;
		break;

	case NMGL_LIGHT0:
	case NMGL_LIGHT1:
	case NMGL_LIGHT2:
	case NMGL_LIGHT3:
	case NMGL_LIGHT4:
	case NMGL_LIGHT5:
	case NMGL_LIGHT6:
	case NMGL_LIGHT7:
		context->lightingInfo.isEnabledLight[cap - NMGL_LIGHT0] = 0;
		break;
	}
}
