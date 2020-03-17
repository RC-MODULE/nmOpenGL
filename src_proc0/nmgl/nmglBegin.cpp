#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglBegin(NMGLenum mode)
{
	if(cntxt.beginEndInfo.inBeginEnd){
		cntxt.error = NMGL_INVALID_OPERATION;
		return;
	}

	cntxt.beginEndInfo.inBeginEnd = true;
	cntxt.beginEndInfo.mode = mode;
	cntxt.beginEndInfo.vertexCounter = 0;
	cntxt.beginEndInfo.normalCounter = 0;
	cntxt.beginEndInfo.colorCounter = 0;
	//printf("begin\n");
	
}
