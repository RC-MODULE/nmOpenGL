#include "demo3d_nm0.h"
#include "nmgl.h"
#include "debugprint.h"

#include "nmpp.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglColor4f(NMGLfloat red, NMGLfloat green, NMGLfloat blue, NMGLfloat alpha)
{
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	cntxt->currentColor.vec[0] = red;
	cntxt->currentColor.vec[1] = green;
	cntxt->currentColor.vec[2] = blue;
	cntxt->currentColor.vec[3] = alpha;
}

SECTION(".text_nmgl")
void nmglColor4ub(NMGLubyte red, NMGLubyte green, NMGLubyte blue, NMGLubyte alpha)
{
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	cntxt->currentColor.vec[0] = (float)red / RED_COEFF;
	cntxt->currentColor.vec[1] = (float)green / GREEN_COEFF;
	cntxt->currentColor.vec[2] = (float)blue / BLUE_COEFF;
	cntxt->currentColor.vec[3] = (float)alpha/ALPHA_COEFF;
}

SECTION(".text_nmgl")
void nmglColor4fv(NMGLfloat *v)
{
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	cntxt->currentColor.vec[0] = v[0];
	cntxt->currentColor.vec[1] = v[1];
	cntxt->currentColor.vec[2] = v[2];
	cntxt->currentColor.vec[3] = v[3];
}

SECTION(".text_nmgl")
void nmglColor4ubv(NMGLubyte *v)
{
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	cntxt->currentColor.vec[0] = (float)v[0];
	cntxt->currentColor.vec[1] = (float)v[1];
	cntxt->currentColor.vec[2] = (float)v[2];
	cntxt->currentColor.vec[3] = (float)v[3];
}
