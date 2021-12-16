#include "demo3d_nm1.h"

void DepthUpdate(int mask, int func);

SECTION(".text_demo3d") void NMGL_DepthUpdate(NMGL_Context_NM1 *cntxt, NM_Command *command)
{
	NMGL_Context *context = NMGL_GetCurrentContext();
	DepthUpdate(context->depth_test.mask, context->depth_test.func);
}
