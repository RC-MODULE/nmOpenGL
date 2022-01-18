#include "demo3d_nm0.h"
#include "nmgl.h"


//#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
NMGLenum nmglGetError() {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	return cntxt->error;
}
