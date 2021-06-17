#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmblas.h"

#pragma code_section ".text_nmgl"

SECTION(".data_imu7") Array vertexArrayTmp;
SECTION(".data_imu7") Array normalArrayTmp;
SECTION(".data_imu7") Array colorArrayTmp;
#ifdef TEXTURE_ENABLED
SECTION(".data_imu7") Array texcoordArrayTmp; //XXX: Only one texture unit is supported. So one tmpArray.
#endif //TEXTURE_ENABLED

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
#ifdef TEXTURE_ENABLED
	nmblas_scopy(sizeof32(Array), (float*)&cntxt->texState.texcoordArray[0], 1, (float*)&texcoordArrayTmp, 1); //XXX: Only one texture unit is supported.
	NMGLenum clientActiveTexUnitTmp = cntxt->texState.clientActiveTexUnit;
	unsigned int clientActiveTexUnitIndexTmp = cntxt->texState.clientActiveTexUnitIndex;
#endif //TEXTURE_ENABLED

	NMGLboolean arrayEnabled = cntxt->beginEndInfo.vertexCounter != 0;
	
	cntxt->vertexArray.enabled = arrayEnabled;
	nmglVertexPointer(4, NMGL_FLOAT, 0, cntxt->beginEndInfo.vertex);

	cntxt->normalArray.enabled = arrayEnabled;
	nmglNormalPointerNM(NMGL_FLOAT, 0, cntxt->beginEndInfo.normal);

	cntxt->colorArray.enabled = arrayEnabled;
	nmglColorPointer(4, NMGL_FLOAT, 0, cntxt->beginEndInfo.color);
	
#ifdef TEXTURE_ENABLED
	//XXX: Only one texture unit is supported.
	cntxt->texState.clientActiveTexUnit = NMGL_TEXTURE0;
	cntxt->texState.clientActiveTexUnitIndex = 0;
	cntxt->texState.texcoordArray[0].enabled = cntxt->vertexArray.enabled = arrayEnabled;
	nmglTexCoordPointer(2, NMGL_FLOAT, 0, cntxt->beginEndInfo.texcoord);
	
#endif //TEXTURE_ENABLED
	
	//printf("vertexCounter=%d\n", cntxt->beginEndInfo.vertexCounter);
	nmglDrawArrays(cntxt->beginEndInfo.mode, 0, cntxt->beginEndInfo.vertexCounter);

	nmblas_scopy(sizeof32(Array), (float*)&vertexArrayTmp, 1, (float*)&cntxt->vertexArray, 1);
	nmblas_scopy(sizeof32(Array), (float*)&normalArrayTmp, 1, (float*)&cntxt->normalArray, 1);
	nmblas_scopy(sizeof32(Array), (float*)&colorArrayTmp, 1, (float*)&cntxt->colorArray, 1);
#ifdef TEXTURE_ENABLED
	nmblas_scopy(sizeof32(Array), (float*)&texcoordArrayTmp, 1,(float*)&cntxt->texState.texcoordArray[0], 1); //XXX: Only one texture unit is supported.
	cntxt->texState.clientActiveTexUnit = clientActiveTexUnitTmp;
	cntxt->texState.clientActiveTexUnitIndex = clientActiveTexUnitIndexTmp;
#endif //TEXTURE_ENABLED
	//printf("end\n\n");
	
}
