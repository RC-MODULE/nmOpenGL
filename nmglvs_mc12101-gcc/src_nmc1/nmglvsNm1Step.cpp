#include "hal_target.h"
#include "hal.h"
#include "hal.h"
#include "hal_target.h"
#include "nmpp.h"
#include "demo3d_nm1.h"
#include "stdio.h"
#include "ringbuffer.h"
#include "nmprofiler.h"
#include "cache.h"

#include "command_nm1.h"


SECTION(".text_nmglvs") int nmglvsNm1Step()
{	
	NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();
	NM_Command *command = NMGL_ReadNM_Command();	
	switch (command->instr) {

	case NMC1_CLEAR: 
		NMGL_Clear(cntxt, command);
		break;
	case NMC1_COPY_SEG_FROM_IMAGE:
		NMGL_CopySegmentFromImage(cntxt, command);
		break;
	case NMC1_COPY_SEG_TO_IMAGE:
		NMGL_CopySegmentToImage(cntxt, command);
		break;
	case NMC1_DRAW_TRIANGLES: 
		NMGL_DrawTriangles(cntxt, command);
		break;
	case NMC1_DRAW_LINES: 
		NMGL_DrawLines(cntxt, command);
		break;
	case NMC1_DRAW_POINTS: 
		NMGL_DrawPoints(cntxt, command);
		break;
	case NMC1_SET_COLOR: 
		NMGL_SetColor(cntxt, command);
		break;
	case NMC1_SET_DEPTH:
		NMGL_SetDepth(cntxt, command);
		break;
	case NMC1_SWAP_BUFFER: 
		NMGL_SwapBuffer(cntxt, command);
		break;
	case NMC1_FINISH:
		NMGL_Finish(cntxt, command);
		break;
	case NMC1_EXIT:
		NMGL_Exit(cntxt, command);
		break;
	case NMC1_DEPTH_UPDATE: 
		NMGL_DepthUpdate(cntxt, command);
		break;
		
	case NMC1_SET_ACTIVE_TEXTURE: 
		NMGL_SetActiveTexture(cntxt, command);
		break;
	case NMC1_SET_MIPMAP_LVL_POINTER: 
		NMGL_SetMipmapLvlPointer(cntxt, command);
		break;
	case NMC1_BIND_ACTIVE_TEX_OBJECT: 
		NMGL_BindActiveTexObject(cntxt, command);
		break;
	case NMC1_SET_WHF: 
		NMGL_SetWhf(cntxt, command);
		break;
	case NMC1_USE_SHARED_PALETTE: 
		NMGL_UseSharedPalette(cntxt, command);
		break;
	case NMC1_USE_LOCAL_PALETTE: 
		NMGL_UseLocalPalette(cntxt, command);
		break;	
	case NMC1_SET_TEX_ENV_COLOR: 
		NMGL_SetTexEnvColor(cntxt, command);
		break;
	case NMC1_SET_TEX_ENV_MODE: 
		NMGL_SetTexEnvMode(cntxt, command);
		break;
	case NMC1_SET_TEX_PARAMI: 
		NMGL_SetTexParami(cntxt, command);
		break;    
	case NMC1_TEXTURE2D: 
		NMGL_Texture2d(cntxt, command);
		break;
	default:
		break;
	}
	return 0;
} 

