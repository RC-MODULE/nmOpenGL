#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

#define CHECK_SIZE()	if (cntxt->beginEndInfo.normalCounter == cntxt->beginEndInfo.maxSize - 1) { \
		cntxt->error = NMGL_OUT_OF_MEMORY;	\
		return;								\
	}

SECTION(".text_nmgl")
void nmglNormal3f(NMGLfloat x, NMGLfloat y, NMGLfloat z)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	CHECK_SIZE();
	int i = cntxt->beginEndInfo.normalCounter;
	cntxt->beginEndInfo.normal[i].vec[0] = x;
	cntxt->beginEndInfo.normal[i].vec[1] = y;
	cntxt->beginEndInfo.normal[i].vec[2] = z;
	cntxt->beginEndInfo.normal[i].vec[3] = 0;
	cntxt->beginEndInfo.normalCounter++;
}

SECTION(".text_nmgl")
void nmglNormal3fv(const NMGLfloat *v)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	CHECK_SIZE();
	int i = cntxt->beginEndInfo.normalCounter;
	cntxt->beginEndInfo.normal[i].vec[0] = v[0];
	cntxt->beginEndInfo.normal[i].vec[1] = v[1];
	cntxt->beginEndInfo.normal[i].vec[2] = v[2];
	cntxt->beginEndInfo.normal[i].vec[3] = 0;
	cntxt->beginEndInfo.normalCounter++;
}
