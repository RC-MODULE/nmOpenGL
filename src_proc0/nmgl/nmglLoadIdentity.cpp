#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglLoadIdentity() {
	NMGL_Context *context = NMGL_GetCurrentContext();

	loadIdentify(context->currentMatrixStack->top());

	if (context->currentMatrixStack->type == NMGL_MODELVIEW_MATRIX) {
		reverseMatrix3x3in4x4(context->modelviewMatrixStack.top(), &context->normalMatrix);
	}
}
