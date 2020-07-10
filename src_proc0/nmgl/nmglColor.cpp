#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include "nmpp.h"

#pragma code_section ".text_nmgl"

#define CHECK_SIZE()	if (cntxt->beginEndInfo.colorCounter == cntxt->beginEndInfo.maxSize - 1) { \
		cntxt->error = NMGL_OUT_OF_MEMORY;	\
		return;								\
	}

SECTION(".text_nmgl")
void nmglColor4f(NMGLfloat red, NMGLfloat green, NMGLfloat blue, NMGLfloat alpha)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	CHECK_SIZE();
	int i = cntxt->beginEndInfo.colorCounter;
	cntxt->beginEndInfo.color[i].vec[0] = red;
	cntxt->beginEndInfo.color[i].vec[1] = green;
	cntxt->beginEndInfo.color[i].vec[2] = blue;
	cntxt->beginEndInfo.color[i].vec[3] = alpha;
	cntxt->beginEndInfo.colorCounter++;
}

SECTION(".text_nmgl")
void nmglColor4ub(NMGLubyte red, NMGLubyte green, NMGLubyte blue, NMGLubyte alpha)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	CHECK_SIZE();
	int i = cntxt->beginEndInfo.colorCounter;
	cntxt->beginEndInfo.color[i].vec[0] = (float)red;
	cntxt->beginEndInfo.color[i].vec[1] = (float)green;
	cntxt->beginEndInfo.color[i].vec[2] = (float)blue;
	cntxt->beginEndInfo.color[i].vec[3] = (float)alpha;
	cntxt->beginEndInfo.colorCounter++;
}

SECTION(".text_nmgl")
void nmglColor4fv(NMGLfloat *v)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	CHECK_SIZE();
	int i = cntxt->beginEndInfo.colorCounter;
	cntxt->beginEndInfo.color[i].vec[0] = v[0];
	cntxt->beginEndInfo.color[i].vec[1] = v[1];
	cntxt->beginEndInfo.color[i].vec[2] = v[2];
	cntxt->beginEndInfo.color[i].vec[3] = v[3];
	cntxt->beginEndInfo.colorCounter++;
}

SECTION(".text_nmgl")
void nmglColor4ubv(NMGLubyte *v)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	CHECK_SIZE();
	int i = cntxt->beginEndInfo.colorCounter;
	nmppsConvert_32s32f(v, (float*)(cntxt->beginEndInfo.color + i), 4);
	cntxt->beginEndInfo.colorCounter++;
}
