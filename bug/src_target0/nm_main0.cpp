#include "nmgl.h"
#include "nmglvs_nmc0.h"


int main()
{
	nmglvsNm0Init();

	nmglClearColor(0, 0, 0.4f, 1.0f);

	nmglMatrixMode(NMGL_PROJECTION);
	nmglOrthof(0, 0.5, 0, 0.5, -1, 1);
	

	while(nmglvsNm0Run()){
		nmglClear(NMGL_COLOR_BUFFER_BIT);
		
		nmglBegin(NMGL_TRIANGLES);
		nmglVertex2f(0.229200006, 0.245519996);
		nmglVertex2f(0.272000015, 0.261280000);
		nmglVertex2f(0.270639986, 0.270880014);
		nmglEnd();

		nmglvsSwapBuffer();
	}
	nmglvsExit_mc12101();
	return 0x600D600D;
}