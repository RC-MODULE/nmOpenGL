#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglNormal3f(NMGLfloat x, NMGLfloat y, NMGLfloat z)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	cntxt->currentNormal.vec[0] = x;
	cntxt->currentNormal.vec[1] = y;
	cntxt->currentNormal.vec[2] = z;
	cntxt->currentNormal.vec[3] = 0;
}

SECTION(".text_nmgl")
void nmglNormal3fv(const NMGLfloat *v)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	cntxt->currentNormal.vec[0] = v[0];
	cntxt->currentNormal.vec[1] = v[1];
	cntxt->currentNormal.vec[2] = v[2];
	cntxt->currentNormal.vec[3] = 0;
}
