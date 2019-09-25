#include "nmgldef.h"
#include "demo3d_nm1.h"

extern int currentDepthTest;

extern int* depthTestCore_N_rw;
extern int* depthTestCore_Lt_rw;
extern int* depthTestCore_Gt_rw;
extern int* depthTestCore_A_rw;

extern int* depthTestCore_N_r;
extern int* depthTestCore_Lt_r;
extern int* depthTestCore_Gt_r;
extern int* depthTestCore_A_r;

extern int addC4DepthTest;

void DepthBuffer :: update() {
	switch (maskEnabled)
	{
	case NMGL_TRUE:
		switch (mode)
		{
		case NMGL_NEVER:
			currentDepthTest = (int)&depthTestCore_N_rw;
			break;
		case NMGL_LESS:
			currentDepthTest = (int)&depthTestCore_Lt_rw;
			addC4DepthTest = 0x7FFFFFFF;
			break;
		case NMGL_EQUAL:
			break;
		case NMGL_LEQUAL:
			break;
		case NMGL_GREATER:
			currentDepthTest = (int)&depthTestCore_Gt_rw;
			addC4DepthTest = 0;
			break;
		case NMGL_NOTEQUAL:
			break;
		case NMGL_GEQUAL:
			break;
		case NMGL_ALWAYS:
			currentDepthTest = (int)&depthTestCore_A_rw;
			addC4DepthTest = 0;
			break;
		default:
			break;
		}
		break;
	case NMGL_FALSE:
		switch (mode)
		{
		case NMGL_NEVER:
			currentDepthTest = (int)&depthTestCore_N_r;
			break;
		case NMGL_LESS:
			currentDepthTest = (int)&depthTestCore_Lt_r;
			addC4DepthTest = 0x7FFFFFFF;
			break;
		case NMGL_EQUAL:
			break;
		case NMGL_LEQUAL:
			break;
		case NMGL_GREATER:
			currentDepthTest = (int)&depthTestCore_Gt_r;
			addC4DepthTest = 0;
			break;
		case NMGL_NOTEQUAL:
			break;
		case NMGL_GEQUAL:
			break;
		case NMGL_ALWAYS:
			currentDepthTest = (int)&depthTestCore_A_r;
			addC4DepthTest = 0;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
