#include "demo3d_nm1.h"


SECTION(".text_demo3d") void NMGL_LineStippleSet(NMGL_Context_NM1 *context, NM_Command *command){
	context->line.stipple.enabled = command->params[0].b;
}

SECTION(".text_demo3d") void NMGL_PolygonStippleSet(NMGL_Context_NM1 *context, NM_Command *command){
	context->polygon.stipple.enabled = command->params[0].b;
}