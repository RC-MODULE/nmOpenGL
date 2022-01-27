#include "context.h"
#include "hal_target.h"

SECTION(".data_imu0") static NMGL_Context *localContext;

void NMGL_ConnectContext(NMGL_ContextConfig *config){
	return &localContext;
}

NMGL_Context *NMGL_GetCurrentContext(){
	return &localContext;
}

void NMGL_MakeCurrentContext(NMGL_Context *context)
{
	globalContext = context;
}
