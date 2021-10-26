#include "demo3d_nm1.h"



SECTION(".text_demo3d") void NMGL_LineSmoothSet(NMGL_Context_NM1 *context, NM_Command *command){
	context->line.smooth_enabled = command->params[0].b;
}

SECTION(".text_demo3d") void NMGL_PointSmoothSet(NMGL_Context_NM1 *context, NM_Command *command){
	context->point.smooth_enabled = command->params[0].b;
}
