#include "nmgldef.h"
#include "demo3d_nm1.h"

extern "C" DepthCore32* currentDepthTest32;
extern "C" DepthCore16* currentDepthTest16;

extern "C" DepthCore32 depthTestCore32_N_rw;
extern "C" DepthCore32 depthTestCore32_Lt_rw;
extern "C" DepthCore32 depthTestCore32_Gt_rw;
extern "C" DepthCore32 depthTestCore32_A_rw;
extern "C" DepthCore32 depthTestCore32_N_r;
extern "C" DepthCore32 depthTestCore32_Lt_r;
extern "C" DepthCore32 depthTestCore32_Gt_r;
extern "C" DepthCore32 depthTestCore32_A_r;

extern "C" DepthCore16 depthTestCore16_N_rw;
extern "C" DepthCore16 depthTestCore16_Lt_rw;
extern "C" DepthCore16 depthTestCore16_Gt_rw;
extern "C" DepthCore16 depthTestCore16_A_rw;
extern "C" DepthCore16 depthTestCore16_N_r;
extern "C" DepthCore16 depthTestCore16_Lt_r;
extern "C" DepthCore16 depthTestCore16_Gt_r;
extern "C" DepthCore16 depthTestCore16_A_r;

extern int addC4DepthTest;
extern int addC4DepthTest_15s;

void DepthBuffer :: update() {
	switch (maskEnabled)
	{
	case NMGL_TRUE:
		switch (mode)
		{
		case NMGL_NEVER:
			currentDepthTest32 = depthTestCore32_N_rw;
			currentDepthTest16 = depthTestCore16_N_rw;
			break;
		case NMGL_LESS:
			currentDepthTest32 = depthTestCore32_Lt_rw;
			currentDepthTest16 = depthTestCore16_Lt_rw;
			addC4DepthTest = ZBUFF_MAX;
			addC4DepthTest_15s = ZBUFF_MAX_15s;
			break;
		case NMGL_EQUAL:
			break;
		case NMGL_LEQUAL:
			break;
		case NMGL_GREATER:
			currentDepthTest32 = depthTestCore32_Gt_rw;
			currentDepthTest16 = depthTestCore16_Gt_rw;
			addC4DepthTest = 0;
			addC4DepthTest_15s = 0;
			break;
		case NMGL_NOTEQUAL:
			break;
		case NMGL_GEQUAL:
			break;
		case NMGL_ALWAYS:
			currentDepthTest32 = depthTestCore32_A_rw;
			currentDepthTest16 = depthTestCore16_A_rw;
			addC4DepthTest = 0;
			addC4DepthTest_15s = 0;
			break;
		default:
			break;
		}
		break;
	case NMGL_FALSE:
		switch (mode)
		{
		case NMGL_NEVER:
			currentDepthTest32 = depthTestCore32_N_r;
			currentDepthTest16 = depthTestCore16_N_r;
			break;
		case NMGL_LESS:
			currentDepthTest32 = depthTestCore32_Lt_r;
			currentDepthTest16 = depthTestCore16_Lt_r;
			addC4DepthTest = ZBUFF_MAX;
			addC4DepthTest_15s = ZBUFF_MAX_15s;
			break;
		case NMGL_EQUAL:
			break;
		case NMGL_LEQUAL:
			break;
		case NMGL_GREATER:
			currentDepthTest32 = depthTestCore32_Gt_r;
			currentDepthTest16 = depthTestCore16_Gt_r;
			addC4DepthTest = 0;
			addC4DepthTest_15s = 0;
			break;
		case NMGL_NOTEQUAL:
			break;
		case NMGL_GEQUAL:
			break;
		case NMGL_ALWAYS:
			currentDepthTest32 = depthTestCore32_A_r;
			currentDepthTest16 = depthTestCore16_A_r;
			addC4DepthTest = 0;
			addC4DepthTest_15s = 0;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
