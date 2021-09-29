#include "hal_target.h"
#include "hal.h"
#include <time.h>
#include "stdio.h"
#include "nmpp.h"
#include "nmgl.h"
#include "nmglvs_nmc0.h"
#include "nmprofiler.h"


SECTION(".text_shared0") int main()
{
	
	NMGL_Framebuffer* defaultFramebuffer = nmglvsNm0Init();
	if (defaultFramebuffer == 0) {
		return 0;
	}

	nmglClearColor(0, 0, 0.4f, 0.0f);

	nmglMatrixMode(NMGL_PROJECTION);
	//nmglOrthof(-320, 320, -240, 240, -384, 384);	
	nmglOrthof(-384, 384, -384, 384, -384, 384);	
	nmglMatrixMode(NMGL_MODELVIEW);

	

	while(nmglvsNm0Run()){
		
		nmglClear(NMGL_COLOR_BUFFER_BIT);
		nmglRotatef(1, 1, 0, 0);
		nmglBegin(NMGL_TRIANGLES);
		nmglColor4f(1,0,0,1);
		nmglVertex2f(0,0);
		nmglVertex2f(0,30);
		nmglVertex2f(30,30);
		nmglVertex2f(0,0);
		nmglVertex2f(30,0);
		nmglVertex2f(30,30);
		nmglEnd();
		
		nmglvsSwapBuffer();

	}
	

	nmglvsExit_mc12101();
	return 0x600D600D;
} 
