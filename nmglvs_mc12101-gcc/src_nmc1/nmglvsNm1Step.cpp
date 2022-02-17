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
	//halLed(0xFF);
	NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();
	NM_Command *command = NMGL_ReadNM_Command();	
	//halLed(command->instr >> 16);
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
	case NMC1_POINT_SIZE: 
		NMGL_PointSize(cntxt, command);
		break;
	case NMC1_SET_COLOR: 
		NMGL_SetColor(cntxt, command);
		break;
	case NMC1_SET_DEPTH:
		NMGL_SetDepth(cntxt, command);
		break;
	case NMC1_DEPTH_MASK: 
		NMGL_DepthMask(cntxt, command);
		break;
	case NMC1_DEPTH_FUNC: 
		NMGL_DepthFunc(cntxt, command);
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
	case NMC1_DEPTH: 
		NMGL_Depth(cntxt, command);
		break;
	case NMC1_ALPHA_TEST:
		NMGL_AlphaTestSet(cntxt,command);
		break;
	case NMC1_BLEND:
		NMGL_BlendSet(cntxt,command);
		break;		
	case NMC1_LINE_SMOOTH:
		NMGL_LineSmoothSet(cntxt,command);
		break;			
	case NMC1_POINT_SMOOTH:
		NMGL_PointSmoothSet(cntxt,command);
		break;		
	case NMC1_POLYGON_OFFSET_FILL:
		NMGL_PolygonOffsetFill(cntxt,command);
		break;
	case NMC1_LINE_STIPPLE:
		NMGL_LineStippleSet(cntxt,command);
		break;
	case NMC1_POLYGON_STIPPLE:
		NMGL_PolygonStippleSet(cntxt,command);
		break;
	case NMC1_SET_HINT:
		NMGL_SetHint(cntxt,command);
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
	/*case NMC1_SET_COLOR_PALETTE: 
		NMGL_SetColorPalette(cntxt, command);
		break;*/
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
	case NMC1_SHADE_MODEL: 
		NMGL_ShadeModel(cntxt, command);
		break;
	default:
		break;
	}
	return 0;
} 

