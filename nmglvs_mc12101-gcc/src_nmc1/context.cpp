#include "context.h"
#include "hal_target.h"

SECTION(".data_imu0") static NMGL_Context *globalContext;


NMGL_Context *NMGL_GetCurrentContext(){
	return globalContext;
}

void NMGL_ContextInit(NMGL_Context *context){
	globalContext = context;
}
