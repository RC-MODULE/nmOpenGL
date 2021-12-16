#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglBegin(NMGLenum mode)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	if(context->beginEndInfo.inBeginEnd){
		cntxt->error = NMGL_INVALID_OPERATION;
		return;
	}

	context->beginEndInfo.inBeginEnd = true;
	context->beginEndInfo.mode = mode;
	context->beginEndInfo.vertexCounter = 0;
	//printf("begin\n");
	
}
