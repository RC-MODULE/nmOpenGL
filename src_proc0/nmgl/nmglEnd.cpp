#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmblas.h"

#pragma code_section ".text_nmgl"

SECTION(".data_imu7") Array vertexArrayTmp;
SECTION(".data_imu7") Array normalArrayTmp;
SECTION(".data_imu7") Array colorArrayTmp;
//TEXTURING_PART
SECTION(".data_imu7") Array texcoordArrayTmp; //XXX: Only one texture unit is supported. So one tmpArray.
//TEXTURING_PART

SECTION(".text_nmgl")
void nmglEnd ()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	if (!context->beginEndInfo.inBeginEnd) {
		cntxt->error = NMGL_INVALID_OPERATION;
		return;
	}
	context->beginEndInfo.inBeginEnd = false;

	nmblas_scopy(sizeof32(Array), (float *)&context->vertexArray, 1, (float *)&vertexArrayTmp, 1);
	nmblas_scopy(sizeof32(Array), (float *)&context->normalArray, 1, (float *)&normalArrayTmp, 1);
	nmblas_scopy(sizeof32(Array), (float *)&context->colorArray, 1, (float *)&colorArrayTmp, 1);
	//TEXTURING_PART
	nmblas_scopy(sizeof32(Array), (float*)&cntxt->texState.texcoordArray[0], 1, (float*)&texcoordArrayTmp, 1); //XXX: Only one texture unit is supported.
	NMGLenum clientActiveTexUnitTmp = cntxt->texState.clientActiveTexUnit;
	unsigned int clientActiveTexUnitIndexTmp = cntxt->texState.clientActiveTexUnitIndex;
//TEXTURING_PART

	NMGLboolean arrayEnabled = context->beginEndInfo.vertexCounter != 0;

	context->vertexArray.enabled = arrayEnabled;
	nmglVertexPointer(4, NMGL_FLOAT, 0, context->beginEndInfo.vertex);

	context->normalArray.enabled = arrayEnabled;
	nmglNormalPointerNM(NMGL_FLOAT, 0, context->beginEndInfo.normal);

	context->colorArray.enabled = arrayEnabled;
	nmglColorPointer(4, NMGL_FLOAT, 0, context->beginEndInfo.color);

	//TEXTURING_PART
	//XXX: Only one texture unit is supported.
	cntxt->texState.clientActiveTexUnit = NMGL_TEXTURE0;
	cntxt->texState.clientActiveTexUnitIndex = 0;
	cntxt->texState.texcoordArray[0].enabled = context->vertexArray.enabled = arrayEnabled;
	nmglTexCoordPointer(2, NMGL_FLOAT, 0, context->beginEndInfo.texcoord);
	
//TEXTURING_PART
	
	//printf("vertexCounter=%d\n", cntxt->beginEndInfo.vertexCounter);
	nmglDrawArrays(context->beginEndInfo.mode, 0, context->beginEndInfo.vertexCounter);

	nmblas_scopy(sizeof32(Array), (float *)&vertexArrayTmp, 1, (float *)&context->vertexArray, 1);
	nmblas_scopy(sizeof32(Array), (float *)&normalArrayTmp, 1, (float *)&context->normalArray, 1);
	nmblas_scopy(sizeof32(Array), (float *)&colorArrayTmp, 1, (float *)&context->colorArray, 1);
	//TEXTURING_PART
	nmblas_scopy(sizeof32(Array), (float*)&texcoordArrayTmp, 1,(float*)&cntxt->texState.texcoordArray[0], 1); //XXX: Only one texture unit is supported.
	cntxt->texState.clientActiveTexUnit = clientActiveTexUnitTmp;
	cntxt->texState.clientActiveTexUnitIndex = clientActiveTexUnitIndexTmp;
//TEXTURING_PART
	//printf("end\n\n");
	
}
