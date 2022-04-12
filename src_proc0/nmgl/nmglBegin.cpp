#include "demo3d_nm0.h"
#include "nmgl.h"
#include "debugprint.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglBegin(NMGLenum mode)
{
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if(cntxt->beginEndInfo.inBeginEnd){
		cntxt->error = NMGL_INVALID_OPERATION;
		return;
	}

	cntxt->beginEndInfo.inBeginEnd = true;
	cntxt->beginEndInfo.mode = mode;
	cntxt->beginEndInfo.vertexCounter = 0;
	//printf("begin\n");
	
}
