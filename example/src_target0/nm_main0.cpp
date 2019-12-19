#include "nmgl.h"
#include "nmglvs_nmc0.h"

float triangles[24]{
	0,0,50, 1,
	0,30,50, 1,
	32,30,50, 1,

	-5,0,25, 1,
	-5,30,25, 1,
	27,30,25, 1

	/*0,0,500,
	30,0,500,
	30,30,500,*/
};

float color[24]{
	1,0,0, 1,
	1,0,0, 1,
	1,0,0, 1,

	0,0,1, 1,
	0,0,1, 1,
	0,0,1, 1,

	/*0,0,500,
	30,0,500,
	30,30,500,*/
};

int main()
{
	nmglvsNm0Init();

	nmglClearColor(0, 0, 0.4f, 1.0f);

	nmglMatrixMode(NMGL_PROJECTION);
	nmglOrthof(-WIDTH_IMAGE/2, WIDTH_IMAGE / 2, -HEIGHT_IMAGE / 2, HEIGHT_IMAGE / 2, 0, 100);
	

	nmglEnable(NMGL_DEPTH_TEST);
	nmglVertexPointer(4, NMGL_FLOAT, 0, triangles);
	nmglColorPointer(4, NMGL_FLOAT, 0, color);
	nmglEnableClientState(NMGL_VERTEX_ARRAY);
	nmglEnableClientState(NMGL_COLOR_ARRAY);
	while(nmglvsNm0Run()){
		nmglClear(NMGL_COLOR_BUFFER_BIT | NMGL_DEPTH_BUFFER_BIT);

		nmglDrawArrays(NMGL_TRIANGLES, 0, 6);

		nmglvsSwapBuffer();
	}
	nmglDisableClientState(NMGL_VERTEX_ARRAY);
	nmglDisableClientState(NMGL_COLOR_ARRAY);
	nmglvsExit_mc12101();
	return 0x600D600D;
}