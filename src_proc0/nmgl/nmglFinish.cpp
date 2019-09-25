#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include <math.h>


SECTION(".text_nmgl")
void nmglFinish (){
	while (!halRingBufferIsEmpty(&cntxt.synchro->commandsRB));
}
