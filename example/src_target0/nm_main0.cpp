#include "nmgl.h"
#include "nmglvs_nmc0.h"

float triangles[12]{
	0,0,
	0,30,
	30,30,

	0,0,
	30,0,
	30,30
};

int main()
{
	nmglvsNm0Init();

	nmglClearColor(0, 0, 0.4f, 1.0f);

	nmglOrthof(-WIDTH_IMAGE/2, WIDTH_IMAGE / 2, -HEIGHT_IMAGE / 2, HEIGHT_IMAGE / 2, 0, 100);
	
	nmglVertexPointer(2, NMGL_FLOAT, 0, triangles);
	nmglEnableClientState(NMGL_VERTEX_ARRAY);
	while(nmglvsNm0Run()){
		nmglClear(NMGL_COLOR_BUFFER_BIT);

		//nmglDrawArrays(NMGL_TRIANGLES, 0, 6);

		nmglvsSwapBuffer();
	}
	nmglDisableClientState(NMGL_VERTEX_ARRAY);
	nmglvsExit_mc12101();
	return 0x600D600D;
}