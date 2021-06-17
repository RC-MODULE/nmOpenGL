#include "demo3d_nm1.h"
#include "myserverdma.h"



SECTION(".text_demo3d") void NMGL_Finish(NMGL_Context_NM1 *context, CommandNm1 *command) {
	msdWaitDma();
}


