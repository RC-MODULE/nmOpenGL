#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".data_imu7") Array vertexArrayTmp;
SECTION(".data_imu7") Array normalArrayTmp;
SECTION(".data_imu7") Array colorArrayTmp;

SECTION(".text_nmgl")
void nmglEnd ()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	if (!cntxt->beginEndInfo.inBeginEnd) {
		cntxt->error = NMGL_INVALID_OPERATION;
		return;
	}
	cntxt->beginEndInfo.inBeginEnd = false;

	nmblas_scopy(sizeof32(Array), (float*)&cntxt->vertexArray, 1, (float*)&vertexArrayTmp, 1);
	nmblas_scopy(sizeof32(Array), (float*)&cntxt->normalArray, 1, (float*)&normalArrayTmp, 1);
	nmblas_scopy(sizeof32(Array), (float*)&cntxt->colorArray, 1, (float*)&colorArrayTmp, 1);

	cntxt->vertexArray.enabled = cntxt->beginEndInfo.vertexCounter != 0;
	nmglVertexPointer(4, NMGL_FLOAT, 0, cntxt->beginEndInfo.vertex);

	cntxt->normalArray.enabled = cntxt->beginEndInfo.normalCounter != 0;
	nmglNormalPointerNM(NMGL_FLOAT, 0, cntxt->beginEndInfo.normal);

	cntxt->colorArray.enabled = cntxt->beginEndInfo.colorCounter != 0;
	nmglColorPointer(4, NMGL_FLOAT, 0, cntxt->beginEndInfo.color);
	
	//printf("vertexCounter=%d\n", cntxt->beginEndInfo.vertexCounter);
	nmglDrawArrays(cntxt->beginEndInfo.mode, 0, cntxt->beginEndInfo.vertexCounter);

	nmblas_scopy(sizeof32(Array), (float*)&vertexArrayTmp, 1, (float*)&cntxt->vertexArray, 1);
	nmblas_scopy(sizeof32(Array), (float*)&normalArrayTmp, 1, (float*)&cntxt->normalArray, 1);
	nmblas_scopy(sizeof32(Array), (float*)&colorArrayTmp, 1, (float*)&cntxt->colorArray, 1);
	//printf("end\n\n");
	
}
